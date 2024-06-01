/**	\file	controller_daikin.cpp
 * 	\brief	大金冷氣遙控器 IR code 定義 (不符合 NEC 通信協定)
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

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
const IR_CODE daikin_codetable[] =	///< 大金冷氣遙控器 IR code table
{
	{0x03, "POWER"},
	{0x0b, "OFF"},
	{0x13, "ON"},
	{0x07, "FAN -"},
	{0x0f, "FAN +"},
};
