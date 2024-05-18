/*
 * controller_toshiba_vcr.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

#define	ToshibaVCR	0x44bb

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

IR_CodeTable tochibavcr("Toshiba VCR", ToshibaVCR, toshibavcr_codeTable, LENGTH(toshibavcr_codeTable));
