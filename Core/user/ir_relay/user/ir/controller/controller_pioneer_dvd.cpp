/*
 * controller_pioneer_dvd.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "../ir_code.h"

#define	PioneerDVD	0xa35c

/*
 * 	Pioneer DVD (32 bits)
 */
const IR_CODE pioneerdvd_codeTable[] =	//!< IR code table
{
	{0xafbc, "POWER"},
	{0xaff6, "LASTMEM"},
	{0xafb1, "CONDMEM"},
	{0xafb6, "OPEN/CLOSE"},
	{0xafbe, "AUDIO"},
	{0xaf36, "SUBTITLE"},
	{0xafb5, "ANGLE"},
	{0xafe4, "REPEAT"},
	{0xafe8, "REPEAT A-B"},
	{0xafb0, "SETUP"},
	{0xafb9, "MENU"},
	{0xafb4, "TOP MENU"},
	{0xafe3, "DISPLAY"},
	{0xaff4, "RETURN"},
	{0xafef, "ENTER"},
	{0xaff2, "UP"},
	{0xaff3, "DOWN"},
	{0xaf63, "LEFT"},
	{0xaf64, "RIGHT"},
	{0xafb8, "STEP"},
	{0xafb7, "SLOW"},
	{0xafa1, "1"},
	{0xafa2, "2"},
	{0xafa3, "3"},
	{0xafa4, "4"},
	{0xafa5, "5"},
	{0xafa6, "6"},
	{0xafa7, "7"},
	{0xafa8, "8"},
	{0xafa9, "9"},
	{0xafa0, "0"},
	{0xafbf, "+10"},
	{0xafe5, "CLEAR"},
	{0xaffe, "RANDOM"},
	{0xafec, "PROGRAM"},
	{0xafb3, "SEARCH"},

	{0xa399, "Pioneer"},	// Pioneer DVD
	{0xa39d, "PREV"},
	{0xa39c, "NEXT"},
	{0xa398, "STOP"},
	{0xa39e, "PLAY"},
	{0xa39f, "PAUSE"},
	{0xa39b, "REV"},
	{0xa39a, "FWD"},
};

IR_CodeTable pioneerdvd("Pioneer DVD", PioneerDVD, pioneerdvd_codeTable, LENGTH(pioneerdvd_codeTable));
