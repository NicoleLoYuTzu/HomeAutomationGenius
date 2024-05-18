/*
 * ir_sender.cpp
 *
 *  Created on: May 4, 2024
 *      Author: ysliu
 */

#include "gpio.h"

#include "ir_sender.h"

GPIO_PinState mark = GPIO_PIN_SET;
GPIO_PinState space = GPIO_PIN_RESET;

IR_Sender::IR_Sender()
{
	m_bTxCode = false;
}

/**	\brief	Process IR transmission.
 *
 */
void	IR_Sender::processing()
{
	if (m_bTxCode)
		sendFrame();
}

/**	\brief	Send IR code.
 *
 */
void	IR_Sender::SendCode(uint32_t code, bool bLoopback)
{
	mark = bLoopback ? GPIO_PIN_RESET : GPIO_PIN_SET;
	space = bLoopback ? GPIO_PIN_SET : GPIO_PIN_RESET;

	if (!m_bTxCode)
	{
		m_code = code;
		m_state = ST_IDLE;
		preSignalTick = sys_tick_10us;
		m_bTxCode = true;
	}
}

/**	\brief	Send IR code.
 *
 */
void	IR_Sender::sendFrame()
{
	uint32_t currTick = sys_tick_10us;
	uint32_t pulseWidth = currTick - preSignalTick;

	switch(m_state)
	{
	case ST_IDLE:
		HAL_GPIO_WritePin(IRDA_GPIO_Port, IRDA_Pin, mark);
		preSignalTick = currTick;
		m_state = ST_START;
		break;

	case ST_START:
		if (pulseWidth >= 16*NEC_TICK)
		{
			HAL_GPIO_WritePin(IRDA_GPIO_Port, IRDA_Pin, space);
			preSignalTick = currTick;
			m_state = NEC_LEADING;
		}
		break;

	case NEC_LEADING:
		if (pulseWidth >= 8*NEC_TICK)
		{
			HAL_GPIO_WritePin(IRDA_GPIO_Port, IRDA_Pin, mark);
			preSignalTick = currTick;
			m_bitNo = 0;
			m_state = NEC_DATAFRAME;
		}
		break;

	case NEC_DATAFRAME:
		if (pulseWidth >= NEC_TICK)
		{
			HAL_GPIO_WritePin(IRDA_GPIO_Port, IRDA_Pin, space);
			preSignalTick = currTick;
			m_state = NEC_DATABIT;
		}
		break;

	case NEC_DATABIT:
		if (pulseWidth >= ((m_code & 1) ? (3*NEC_TICK) : NEC_TICK))
		{
			HAL_GPIO_WritePin(IRDA_GPIO_Port, IRDA_Pin, mark);
			preSignalTick = currTick;

			m_code >>= 1;
			m_state = (++m_bitNo == 32) ? NEC_DONE : NEC_DATAFRAME;
		}
		break;

	case NEC_DONE:
		if (pulseWidth >= NEC_TICK)
		{
			HAL_GPIO_WritePin(IRDA_GPIO_Port, IRDA_Pin, space);
			m_bTxCode = false;
		}
		break;

	default:
		break;
	}
}
