/*
 * controller_toshiba_tv.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "../ir_code.h"

#define	ToshibaTV 0x40bf


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

IR_CodeTable tochibatv("Toshiba TV", ToshibaTV, toshibatv_codeTable, LENGTH(toshibatv_codeTable));

