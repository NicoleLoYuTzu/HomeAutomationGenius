/**	\file	ir_base.cpp
 * 	\brief	紅外線遙控器通信基礎類別
 *
 *  Created on: May 4, 2024
 *      Author: ysliu
 */

#include "ir_base.h"

// Constructor
IR_Base::IR_Base()
{
	m_state = ST_START;
}
