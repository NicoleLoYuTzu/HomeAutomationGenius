/*
 * ir_sender.cpp
 *
 *  Created on: May 4, 2024
 *      Author: ysliu
 */

#include "gpio.h"
#include "tim.h"

#include "ir_sender.h"

IR_Sender::IR_Sender()
{
	m_state = ST_IDLE;
}

/**	\brief	Call this function to start sending an IR code.
 *
 */
void	IR_Sender::SendCode(uint32_t code)
{
	uint32_t timing = sys_tick_10us;

	put(MARK, timing); timing += 16*NEC_TICK;
	put(SPACE, timing); timing += 8*NEC_TICK;

	for (int i=0; i<32; i++)
	{
		put(MARK, timing); timing += NEC_TICK;
		put(SPACE, timing); timing += (code & 1) ? 3*NEC_TICK : NEC_TICK;
		code >>= 1;
	}
	put(MARK, timing); timing += NEC_TICK;
	put(SPACE, timing);
}

/**	\brief	Process IR transmission.
 *
 */
void	IR_Sender::processing()
{
	switch(m_state)
	{
	case ST_IDLE:
		if (m_size == 0)
			break;

		m_signal = get();
		m_state = ST_START;

	case ST_START:
		if (sys_tick_10us >= m_signal.time)
		{
			SetIR(m_signal.state);
			m_state = ST_IDLE;
		}
		break;

	default:
		break;
	}
}

/**	\brief	Set IR LED and loopback pin.
 *
 */
void	IR_Sender::SetIR(bool bOn)
{
	HAL_GPIO_WritePin(IRTX_GPIO_Port, IRTX_Pin, bOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IRTX2_GPIO_Port, IRTX2_Pin, bOn ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
