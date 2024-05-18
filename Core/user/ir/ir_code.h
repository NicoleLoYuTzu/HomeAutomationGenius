/*
 * ir_code.h
 *
 *  Created on: Apr 27, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_CODE_H_
#define IR_IR_CODE_H_

#include "stdint.h"

enum CustomCode
{
	MOD = 0xbebe,
	FAN = 0xfe01,
	HD_PLAYER = 0xff00,
	ToshibaTV = 0x40bf,
	ToshibaVCR = 0x44bb,
	PioneerDVD = 0xa35c,
};

/**	IR code and key name.
 *
 */
typedef struct
{
	uint16_t	irCode; 		//!< IR code
	const char 	*keyName;		//!< key name
} IR_CODE;

/**	IR code table.
 *
 */
class IR_CodeTable
{
public:
	IR_CodeTable(const char *controllerName, uint16_t customCode, const IR_CODE *pCodeTable, int tableLen);

	bool	FindKey(uint8_t datacode, const char **ppControllerName, const char **ppKeyname);

private:
	const char *m_pControllerName;
	uint16_t	m_customcode;
	const IR_CODE *m_pCodeTable;
	int			m_nTableLen;
};

/*
 * 	MOD (32 bits)
 * 	custom code: 0111 1101 0111 1101	0xbebe
 * 	data code:   0000 1110 1111 0001	(byte_2 = ~byte_3)
*/
const IR_CODE mod_codetable[] =
{
	{0x70, "POWER"},
	{0x20, "0"},
	{0x21, "1"},
	{0x22, "2"},
	{0x23, "3"},
	{0x24, "4"},
	{0x25, "5"},
	{0x26, "6"},
	{0x27, "7"},
	{0x28, "8"},
	{0x29, "9"},
	{0x7a, "origin"},
	{0x76, "language"},
	{0x77, "home"},
	{0x2f, "TV"},
	{0x7b, "up"},
	{0x7c, "down"},
	{0x7d, "left"},
	{0x7e, "right"},
	{0x7f, "choose"},
	{0x78, "back"},
	{0x3b, "search"},
	{0x74, "info"},
	{0x36, "vol+"},
	{0x37, "vol-"},
	{0x34, "ch+"},
	{0x35, "ch-"},
	{0x2e, "menu"},
	{0x63, "mute"},
	{0x40, "hamivideo"},
	{0x41, "netflix"},
	{0x2d, "B"},
	{0x2b, "G"},
	{0x2c, "Y"},
	{0x2a, "R"},
};

/*
 * 	FAN (32 bits)
 * 	custom code: 1000 0000 0111 1111	0xfe01
 * 	data code:   1111 0000 0000 1111	(byte_2 = ~byte_3)
*/
const IR_CODE fan_codetable[] =
{
	{0x03, "POWER"},
	{0x0b, "OFF"},
	{0x13, "ON"},
	{0x07, "FAN -"},
	{0x0f, "FAN +"},
};

/*
 * 	DAIKIN (152 bits)
 * 	custom code:
 * 	1000 1000 0101 1011
 * 	1110 0100 0000 0000
 * 	0000 0000 0000 1100
 * 	0110 1100 0000 0000
 * 	1111 0101 0000 0000
 * 	0000 0000 0000 0000
 * 	0000 0000 0000 0000
 * 	0000 0000 0000 0011
 * 	0000 0000 0000 0000
 * 	1110 0111
 * 	data code:  (byte_2 = ~byte_3)
*/
const IR_CODE daikin_codetable[] =
{
	{0x03, "POWER"},
	{0x0b, "OFF"},
	{0x13, "ON"},
	{0x07, "FAN -"},
	{0x0f, "FAN +"},
};

/*
 *	HD player (32 bits)
 *	custom code: 0000 0000 1111 1111	0xff00
 *	data code:   0110 1000 1001 0111
 *
 */
const IR_CODE hdrplayer_codeTable[] =	//!< IR code table
{
	// Gray Remote Controller
	{0x16, "POWER"},
	{0x03, "1"},
	{0x01, "2"},
	{0x06, "3"},
	{0x09, "4"},
	{0x1d, "5"},
	{0x1f, "6"},
	{0x0d, "7"},
	{0x19, "8"},
	{0x1b, "9"},
	{0x15, "0"},
	{0x17, "Favorite"},
	{0x4d, "FullScrn"},
	{0x48, "View"},
	{0x40, "Rewind"},
	{0x12, "Forward"},
	{0x04, "R.List"},
	{0x0f, "Text"},
	{0x1c, "EPG"},
	{0x10, "Mute"},
	{0x05, "CH+"},
	{0x02, "CH-"},
	{0x0a, "Vol-"},
	{0x1e, "Vol+"},
	{0x14, "Play"},
	{0x11, "REC"},
	{0x1a, "Stop"},
	{0x4c, "Pause"},
	{0x54, "Capture"},
	{0x0c, "Cancel"},
	{0x00, "Tab"},
	{0x0e, "Recall"},
};

/**
 *	Toshiba TV (32 bits)
 */
const IR_CODE toshibatv_codeTable[] =	//!< IR code table
{
	{0x12, "POWER"},		// Toshiba TV
	{0x1c, "RECALL"},
	{0x15, "TIMER"},
	{0x0f, "TV/VIDEO"},
	{0x18, "TV/CATV"},
	{0x0b, "RF1/2"},
	{0x10, "MUTE"},
	{0x17, "CH RTN"},
	{0x13, "MTS"},
	{0x00, "0"},
	{0x01, "1"},
	{0x02, "2"},
	{0x03, "3"},
	{0x04, "4"},
	{0x05, "5"},
	{0x06, "6"},
	{0x07, "7"},
	{0x08, "8"},
	{0x09, "9"},
	{0x0a, "100"},
	{0x48, "CH LOC"},
	{0x1a, "VOL+"},
	{0x1e, "VOL-"},
	{0x1b, "CH+"},
	{0x1f, "CH-"},
	{0x0c, "RESET"},
	{0x19, "CTRL->"},
	{0x1d, "CTRL<-"},
	{0x0e, "PICT"},
	{0x0d, "AUDIO"},
};

/*
 * 	Toshiba VCR
 */
const IR_CODE toshibavcr_codeTable[] =	//!< IR code table
{
	{0x12, "POWER"},		// Toshiba VCR
	{0x1b, "CH+"},
	{0x1f, "CH-"},
	{0x11, "TV/VCR"},
	{0x18, "REC"},
	{0x10, "PAUSE"},
	{0x0d, "SLOW"},
	{0x19, "REW"},
	{0x15, "PLAY"},
	{0x13, "FF"},
	{0x14, "STOP"},
};

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

#endif /* IR_IR_CODE_H_ */
