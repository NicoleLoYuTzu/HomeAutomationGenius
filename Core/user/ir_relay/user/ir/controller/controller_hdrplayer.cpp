/*
 * controller_hdrplayer.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

#define	HDR_PLAYER	0xff00

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

IR_CodeTable hdrplayer("HDR player", HDR_PLAYER, hdrplayer_codeTable, LENGTH(hdrplayer_codeTable));
