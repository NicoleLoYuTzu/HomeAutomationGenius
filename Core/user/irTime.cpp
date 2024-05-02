/*
 * irTime.cpp
 *
 *  Created on: Apr 19, 2024
 *      Author: ysliu
 */

#include "irTime.h"

uint32_t sys_tick_10us = 0;

/**	\brief Constructor.
 *
 */
IrSignalTime::IrSignalTime()
{
	m_head = 0;
	m_tail = 0;
	m_size = 0;
}

/**	\brief	Save state and time in queue.
 *
 *	called by ISR only.
 */
void IrSignalTime::put(IR_STATE state)
{
	if (m_size >= IR_TIME_NO)
		Error_Handler();

	m_timing[m_tail].time = sys_tick_10us;
	m_timing[m_tail].state = state;

//	__disable_irq();
	m_size++;
	if (++m_tail >= IR_TIME_NO)
		m_tail = 0;
//	__enable_irq();
}

/**	\brief	Get state and time from queue.
 *
 */
IrSignal IrSignalTime::get()
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

/**	\brief	Timer period elapsed callback function.
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6)
	{
		sys_tick_10us++;
	}
}

