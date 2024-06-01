/** \file	console.cpp
    \brief	Interactive command parser.

	Author	: Liu Yi-Sheng

		Date     Description
	----------  ---------------------------------------------
	2005/04/08  Created.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmsis_os2.h"
#include "ansi.h"
#include "user_usart.h"
#include "command.h"

#define	Bit(n) (1 << n)

#define	putchar	__io_putchar

enum
{
	FLG_SBC_LED = Bit(0),			//!< flash on board LED
	FLG_STS_LED = Bit(1),			//!< flash status LED
	FLG_ERR_LED = Bit(2),			//!< flash error LED
	FLG_LED_ON	= Bit(3),
	FLG_LED_OFF	= Bit(4),
	FLG_TOGGLE	= Bit(5),
};

osEventFlagsId_t g_pLedFlags;

CUart	uart(USART3, &huart3);

char	g_cmdLine[MAX_CMDLINE];		//!< command line
char	g_cmdWord[MAX_CMDWORD];		//!< command
char	g_parm[MAX_PARM_LEN];		//!< parameters
char	*g_pArg;						//!< pointer to argument

const CmdTable MainCommand[] = //!< command table
{
//	Arg Hlp
	{0, 0, "ver",	ShowVersion,    "Show version"		},
	{1, 1, "task",	proc_task, 		"Task management"	},
	{1, 1, "rst",	proc_reset, 	"Reset system"		},
	{0, 0, "more",	show_more,		"Show more commands"},
};

const CmdTable moreCmd[] =		//!< more available commands
{
	{0, 0, "cls",	clear_screen,   "Clean screen"		},
	{0, 0, "color", show_color, 		"Show ANSI color"	},
	{0, 0, "halt",	halt_system,    "Halt system"		},
};

/**	\brief	Console task.
 *
 */
void	taskConsole(void *argument)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	CUart::g_pUarts[0]->ScheduleRxIT();

	ansi_setColor(_BLACK, _GRAY);
	ansi_clearScreen();
	ShowVersion();
	ansi_setColor(_GREEN, _GRAY);
	printf("*** Type '?' for help\n");

	while (1)
	{
		if (get_command("$", g_cmdLine) > 0)
			process_command(MainCommand, LENGTH(MainCommand));
	}
}

/**	\brief	Get a character.
 *
 */
uint8_t	GetChar()
{
	while(1)
	{
		if (CUart::g_pUarts[0]->m_rxBuf.DataSize() > 0)
			return CUart::g_pUarts[0]->m_rxBuf.GetData();

		osDelay(1);
	}
}

/**	\brief Retarget to USART3.
 *
 */
extern "C" int __io_putchar(int ch)
{
	if (ch == _LF)			// line feed ?
		CUart::g_pUarts[0]->SaveTxData(_CR);	// send CR first
	CUart::g_pUarts[0]->SaveTxData(ch);

	return 0;
}

/** \brief  Get a string from command interface.

    \param[in]	*pPrompt    Command prompt.
    \param[in]	*pCmdLine   Command buffer.
    \return length of command string.
*/
uint8_t	get_command(const char *pPrompt, char *pCmdLine)
{
	uint8_t index;

	ansi_setColor(_LIGHT_RED, _GRAY);
	printf("%c ", *pPrompt);
	ansi_setColor(_LIGHT_BLUE, _GRAY);

	index = 0;
	while (1)
	{
		uint8_t byte = GetChar();

		osEventFlagsSet(g_pLedFlags, FLG_SBC_LED);

		if (byte == _CR)
		{
			pCmdLine[index] = '\0';
			putchar(_LF);
			ansi_setColor(_BLACK, _GRAY);
			return index;
		}
		else if (byte == BACKSPACE)
		{
			if (index > 0)
			{
				putchar(BACKSPACE);
				putchar(' ');
				putchar(BACKSPACE);
				index--;
			}
		}
		else if (isprint(byte))	// valid characters
		{
			if (index < MAX_CMDLINE - 1)
			{
				putchar(byte);
				pCmdLine[index++] = byte;
			}
		}
		else			// control characters
		{
			printf("'%02x'", (int) byte);
			osDelay(500);
			printf("\010\010\010\010    \010\010\010\010");
		}
	}
}

/** \brief  Get a token from command string.

    \param[in]	*pArg	Input argument.
    \param[in]	*pToken	Token retrieve from argument.
    \param[in]	maxLen	Maximum length of token.
*/
char	*get_parm(char *pArg, char *pToken, const int maxLen, bool bCase)
{
	static int	len;

	if (pArg != NULL)
	{
		while (*pArg != '\0' && isspace(*pArg))
			pArg++;                          // skip leading space

		len = 0;
		while ((*pArg != '\0') && !isspace(*pArg) && (len < maxLen - 1))
		{
			*pToken++ = bCase ? *pArg++ : tolower(*pArg++);	// get non-space characters
			len++;
		}
	}

	*pToken = '\0';

	return(pArg);
}

/** \brief  Parameter error.

    \param[in]	*pArg	Parameter that parsing error occurred.
*/
int	parm_error(char *pArg)
{
	if (pArg == NULL || *pArg == '\0')
		printf("No parameter specified\n");
	else
		printf("Parameter error: '%s'\n", pArg);

	return E_PARM;
}

/** \brief Get a 8-bits value from command line.

    \param[out]	*pByte	Return data.

	\return true if a validated b-bits value is read.
*/
bool getINT8U(uint8_t *pByte)
{
	static uint16_t	temp;

	if (str2ul(g_parm, &temp))
	{
		if (temp < 256)
		{
			*pByte = (uint8_t) temp;
			return true;
		}
	}

	return false;
}

/** \brief	Get a 16-bits value from command line.

    \param[out]	*pWord	Return data.

	\return true if a validated b-bits value is read.
*/
bool getINT16U(uint16_t *pWord)
{
	return str2ul(g_parm, pWord);
}

/** \brief  Convert string to unsigned integer.

    \param[in]	*pStr	String to parse.
    \param[out]	*pValue	Output value.
	\retval Return true if success.
*/
bool str2ul(const char *pStr, uint16_t *pValue)
{
	static char *endptr;
	static uint16_t v;

	if (*pStr == '\0')
		return false;

	v = (uint16_t) strtoul(pStr, &endptr, 0);

	if (!(*endptr == '\0' || isspace(*endptr)))
		return false;
	else
	{
		*pValue = v;
		return true;
	}
}

/** \brief  Process command by parsing command string.

    \param[in]	*pCmdTable   Pointer to command table.
    \param[in]	nEntries    Number of commands in command table.
*/
int		process_command(const CmdTable *pCmdTable, int nEntries)
{
	static int result;

	g_pArg = get_parm(g_cmdLine, g_cmdWord, MAX_CMDWORD);

	if (g_cmdWord[0] == '\0')
		return 0;

	ansi_setColor(_BLACK, _GRAY);

	g_pArg = get_parm(g_pArg, g_parm, MAX_PARM_LEN);
	result = cmd_dispatch(pCmdTable, nEntries);
	if (result == E_NOT_PROCESS)
		result = cmd_dispatch(moreCmd, LENGTH(moreCmd));

	if (result == E_NOT_PROCESS)
		printf("Unknown command: %s\n",
		(strcmp(g_cmdWord, "?")==0) ? g_parm : g_cmdWord);

	return result;
}

/** \brief  Process command.

    \param[in]	*pCmdTable   Pointer to command table.
    \param[in]	nEntries    Number of commands in command table.

    - Print message for functions that are not implement yet.
    - Show message if argument specified for those functions that not accept argument.
    - Show simple help message for functions that detail help is not implement.
*/
int		cmd_dispatch(const CmdTable *pCmdTable, int nEntries)
{
	static uint8_t	i;

	for (i=0; i<nEntries; i++)
	{
		if ((strcmp(g_cmdWord, pCmdTable[i].pName) == 0))
		{
			if (pCmdTable[i].pFunc == NULL)
			{
				printf("Command not implement yet: %s\n", g_cmdWord);
				return E_OBJ;
			}
			else if (strcmp(g_parm, "?") == 0 && !pCmdTable[i].bHelp)
			{
				printf("%-7s : %s\n", g_cmdWord, pCmdTable[i].pPrompt);
				return E_OBJ;
			}
			else if (!pCmdTable[i].bArg && g_parm[0] != '\0')
			{
				printf("'%s' doesn't accept parameter\n", g_cmdWord);
				return E_PARM;
			}
			else
				return pCmdTable[i].pFunc();
		}
	}

	if (strcmp(g_cmdWord, "?") == 0)
		return show_help(pCmdTable, nEntries, true);

	return E_NOT_PROCESS;
}

/** \brief  Show help or information.

    \param[in]	*pCmdTable   Command table.
    \param[in]	nEntries    Number of entry in command table.
    \param[in]	bMore	    Show ? [cmd]

    - ?	SHow help in command table.
    - ? [cmd]	Show help for [cmd].
*/
int		show_help(const CmdTable *pCmdTable, const int nEntries, bool bMore)
{
	static uint8_t	i;

	if (g_parm[0] == '\0')
	{						// show simple help
		for (i=0; i<nEntries; i++)
			printf("%-7s : %s\n", pCmdTable[i].pName, pCmdTable[i].pPrompt);
		if (bMore)
			printf("? [cmd] : Show help about [cmd]\n");
		return E_OK;
	}
	else
	{
		for (i=0; i<nEntries; i++)
		{
			if ((strcmp(g_parm, pCmdTable[i].pName) == 0))
			{
				if (pCmdTable[i].pFunc == NULL)
				{
					printf("Command not implement yet: %s\n", g_parm);
					return E_OBJ;
				}
				else if (!pCmdTable[i].bHelp)
				{
					printf("%-7s : %s\n", g_parm, pCmdTable[i].pPrompt);
					return E_OBJ;
				}
				else
				{
					strcpy(g_parm, "?");
					pCmdTable[i].pFunc();	// show detail help for command
					return E_OK;
				}
			}
		}
	}

	return E_NOT_PROCESS;
}

/** \brief  Show more commands.

    Show more commands.
*/
int		show_more(void)
{
	return show_help(moreCmd, LENGTH(moreCmd), false);
}
