/*
 * controller_kns.cpp
 *
 *  Created on: May 13, 2024
 *      Author: ysliu
 */

#include "../ir_code.h"

#define	KNS	0x7e80

/*
 * 	KNS (32 bits)
 * 	custom code: 0111 1110 1000 0000	0x7e80
 * 	data code:   1111 0011 0000 1100	(byte_2 = ~byte_3)
*/
const IR_CODE kns_codetable[] =
{
	{0x0e, "POWER"},
	{0x44, "OFF"},
	{0x0c, "turn"},
};

IR_CodeTable kns("KNS", KNS, kns_codetable, LENGTH(kns_codetable));
