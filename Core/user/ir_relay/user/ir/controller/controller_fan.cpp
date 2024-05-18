/*
 * controller_fan.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

#define	FAN	0xfe01

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

IR_CodeTable fan("FAN", FAN, fan_codetable, LENGTH(fan_codetable));
