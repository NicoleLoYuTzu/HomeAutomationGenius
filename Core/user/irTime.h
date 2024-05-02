/*
 * irTime.h
 *
 *  Created on: Apr 19, 2024
 *      Author: ysliu
 */

#ifndef IRTIME_H_
#define IRTIME_H_

#include "stm32f7xx_hal.h"
#include "tim.h"

#define IR_TIME_NO	100
#define MAX_STATE_TIME	1000

enum IR_STATE {SPACE, MARK};

typedef struct
{
	uint32_t time;
	IR_STATE state;
} IR_EVENT;

/**	Timer in 10 us resolution.
 *
 */
class IrTiming
{
public:
	IrTiming();

	void	put(IR_STATE state);
	IR_EVENT	get();
	bool	hasEvent() {return m_size > 0;}

private:
	int m_head;
	int m_tail;
	int m_size;

	IR_EVENT m_timing[IR_TIME_NO];
};

#endif /* IRTIME_H_ */
