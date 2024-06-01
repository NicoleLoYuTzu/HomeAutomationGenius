/**	@file	ansi.cpp
	@brief	ANSI display control.
	@author Liu, Yi-Sheng

	- 2024/05/30 Porting from BSP.
	- 2024/05/31 To work properly, call setvbuf(stdout, NULL, _IONBF, 0) to disable stdio buffer.
*/

#include <stdio.h>

#include "ansi.h"

/** \brief	Clear screen.
*/
void	ansi_clearScreen(void)
{
	printf("\033[2J");
}

/** \brief	Set text color.

	\param[in]	color	Foreground color.
	\param[in]	bgColor Background color.
*/
void	ansi_setColor(int color, int bgColor)
{
	printf("\033[%d;%d;%dm", color/8, color % 8 + 30, (bgColor & 7) + 40);
}

/** \brief	Move cursor to (x, y).

	\param[in]	x	X coordinate.
	\param[in]	y	Y coordinate.
*/
void	ansi_gotoxy(int x, int y)
{
	printf("\033[%d;%dH", x, y);
}

/** \brief	Move cursor up # lines.

	\param[in]	n	Number of lines to move.
*/
void	ansi_cursorUp(int n)
{
	printf("\033[%dA", n);
}

/** \brief	Move cursor down # lines.

	\param[in]	n	Number of lines to move.
*/
void	ansi_cursorDown(int n)
{
	printf("\033[%dB", n);
}

/** \brief	Move cursor forward # spaces.

	\param[in]	n	Number of characters to move.
*/
void	ansi_cursorRight(int n)
{
	printf("\033[%dC", n);
}

/** \brief	Move cursor back # spaces.

	\param[in]	n	Number of characters to move.
*/
void	ansi_cursorLeft(int n)
{
	printf("\033[%dD", n);
}

/** \brief	Save cursor position.
*/
void	ansi_saveCursor(void)
{
	printf("\033[s");
}

/** \brief	Restore cursor.
*/
void	ansi_restoreCursor(void)
{
	printf("\033[u");
}
