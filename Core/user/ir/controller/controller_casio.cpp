/**	\file	controller_casio.cpp
 * 	\brief	CASIO 投影機遙控器 IR code 定義
 *
 *  Created on: May 18, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

#define	CASIO	0xf484	///< CASIO 投影機遙控器的 custom code

/*
 * 	CASIO (32 bits)
 * 	custom code: 1111 0100 1000 0100	0xf484
 * 	data code:   1111 0100 0000 1011	(byte_2 = ~byte_3)
*/
const IR_CODE casio_codetable[] =	///< CASIO 投影機遙控器 IR code table
{
	{0x0a, "INPUT"},
	{0x0b, "POWER"},
	{0x0c, "MENU"},
	{0x0e, "ESC"},
	{0x1a, "1"},
	{0x1b, "2"},
	{0x1c, "3"},
	{0x1d, "4"},
	{0x4a, "UP"},
	{0x4b, "DOWN"},
	{0x4d, "LEFT"},
	{0x4e, "RIGHT"},
	{0x4c, "ENTER"},
	{0x2a, "KEYSTONE+"},
	{0x2b, "D-ZOOM+"},
	{0x2c, "Vol+"},
	{0x2d, "KEYSTONE-"},
	{0x2e, "D-ZOOM-"},
	{0x2f, "Vol-"},
	{0x3a, "BLANK"},
	{0x3b, "FREEZE"},
	{0x3c, "leaf"},
	{0x5a, "TIMER"},
	{0x5b, "AUTO"},
	{0x5c, "ASPECT"},
	{0x5d, "FUNC"},
};

///	CASIO 投影機遙控器
IR_CodeTable casio("CASIO", CASIO, casio_codetable, LENGTH(casio_codetable));
