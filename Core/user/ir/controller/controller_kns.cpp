/**	\file	controller_kns.cpp
 *	\brief	吸頂風扇遙控器 IR code 定義
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

#define	KNS	0x7e80	///< 吸頂扇遙控器的 custom code

/*
 * 	KNS (32 bits)
 * 	custom code: 0111 1110 1000 0000	0x7e80
 * 	data code:   1111 0011 0000 1100	(byte_2 = ~byte_3)
*/
const IR_CODE kns_codetable[] =		///< 吸頂扇遙控器 IR code table
{
	{0x0e, "POWER"},
	{0x44, "OFF"},
	{0x0c, "turn"},
};

/// 吸頂扇遙控器
IR_CodeTable kns("KNS", KNS, kns_codetable, LENGTH(kns_codetable));
