/*
 * ir_time.cpp
 *
 *  Created on: Apr 19, 2024
 *      Author: ysliu
 */

#include "tim.h"
#include "ir_time.h"

uint32_t sys_tick_10us = 0;

/**	\brief Constructor.
 *
 */
IR_Timing::IR_Timing()
{
	m_head = 0;
	m_tail = 0;
	m_size = 0;
	preSignalTick = 0;
}

/**	\brief	Save state and time in queue.
 *
 */
void	IR_Timing::put(IR_STATE state)
{
	put(state, sys_tick_10us);
}

void	IR_Timing::put(IR_STATE state, uint32_t time_10us)
{
	if (m_size >= IR_TIME_NO)
		Error_Handler();

	m_timing[m_tail].time = time_10us;
	m_timing[m_tail].state = state;

	__disable_irq();
	m_size++;
	if (++m_tail >= IR_TIME_NO)
		m_tail = 0;
	__enable_irq();

}

/**	\brief	Get state and time from queue.
 *
 */
IrSignal IR_Timing::get()
{
	const IrSignal nul = {0, SPACE};

	if (m_size == 0)
		return nul;

	IrSignal time = m_timing[m_head];

	__disable_irq();
	m_size--;
	if (++m_head >= IR_TIME_NO)
		m_head = 0;
	__enable_irq();

	return time;
}
