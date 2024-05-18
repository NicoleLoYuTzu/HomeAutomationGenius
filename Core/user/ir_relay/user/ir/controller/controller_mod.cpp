/*
 * controller_mod.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

#define	MOD	0xbebe

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
	{0x36, "Vol+"},
	{0x37, "Vol-"},
	{0x34, "CH+"},
	{0x35, "CH-"},
	{0x2e, "menu"},
	{0x63, "Mute"},
	{0x40, "hamivideo"},
	{0x41, "netflix"},
	{0x2d, "B"},
	{0x2b, "G"},
	{0x2c, "Y"},
	{0x2a, "R"},
};

IR_CodeTable mod("MOD", MOD, mod_codetable, LENGTH(mod_codetable));

