/**	@file	ansi.h
    @brief	ANSI display control.
	@author Liu, Yi-Sheng
*/
#ifndef __ANSI_H
#define __ANSI_H

enum {_BLACK, _RED, _GREEN, _BROWN,	//!< text colors
      _BLUE, _PURPLE, _CYAN, _GRAY,
      _DARK_GRAY, _LIGHT_RED, _LIGHT_GREEN, _YELLOW,
      _LIGHT_BLUE, _LIGHT_PURPLE, _LIGHT_CYAN, _WHITE
};

void	ansi_clearScreen(void);
void	ansi_setColor(int color, int bgColor);
void	ansi_gotoxy(int x, int y);
void	ansi_cursorUp(int n);
void	ansi_cursorDown(int n);
void	ansi_cursorRight(int n);
void	ansi_cursorLeft(int n);
void	ansi_saveCursor(void);
void	ansi_restoreCursor(void);

#endif
