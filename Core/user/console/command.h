/** \file	command.h
    \brief	Header file for command processing.
*/
#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>

#define	MAX_CMDLINE		50		// max. length for command line
#define	MAX_CMDWORD		8		// max. length for command word
#define	MAX_PARM_LEN		16		// max. length for parameter

#define BACKSPACE       0x08		// Back space

#define E_OK			0		// Operation OK
#define	E_QUIT			1		// quit
#define E_FAIL			-1		// generic fail
#define E_PARM			-2		// Parameter error
#define E_OBJ			-3		// No such object
#define	E_NOT_PROCESS	-4		// command not processed
#define E_NOT_FLASH		-5		// not Flash ROM

/** \brief  Command table.
*/
typedef struct
{					
	bool	bArg;			//!< accept argument
	bool	bHelp;			//!< detail help implemented
	char const	*pName;		//!< command name
	int	(*pFunc)(void);		//!< pointer to function
	char const 	*pPrompt;	//!< command prompt
} CmdTable;

//	console.cpp
uint8_t	get_command(const char *pPrompt, char *pCmdLine);
char	*get_parm(char *pArg, char *pToken, const int maxLen, bool bCase=false);
int		parm_error(char *pArg);
bool	getINT8U(uint8_t *pByte);
bool	getINT16U(uint16_t *pWord);
bool	str2ul(const char *pStr, uint16_t *pValue);
int		process_command(const CmdTable *pCmdTable, int nEntries);
int		cmd_dispatch(const CmdTable *pCmdTable, int nEntries);
int		show_help(const CmdTable *pCmdTable, const int nEntries, bool bMore);
int		show_more(void);

//	proccmd.cpp
int		proc_task();
void	ShowTask();
int		proc_reset(void);

int		ShowVersion(void);
int		clear_screen(void);
int		show_color(void);
int		halt_system(void);
void	show_binary(uint8_t byte);

extern char	g_cmdLine[];	//!< command line
extern char	g_cmdWord[];	//!< command
extern char	g_parm[];		//!< parameters
extern char	*g_pArg;		//!< pointer to argument

#endif
