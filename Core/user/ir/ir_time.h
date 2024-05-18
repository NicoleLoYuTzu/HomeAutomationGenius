/*
 * ir_time.h
 *
 *  Created on: Apr 19, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_TIME_H_
#define IR_IR_TIME_H_

#include "stdint.h"

#define IR_TIME_NO	200
#define NEC_TICK 	56

enum IR_STATE {SPACE, MARK};

typedef struct
{
	uint32_t time;
	IR_STATE state;
} IrSignal;

/**	Timer in 10 us resolution.
 *
 */
class IR_Timing
{
public:
	IR_Timing();

	void		put(IR_STATE state);
	IrSignal	get();

protected:
	uint32_t preSignalTick;

	int m_head;
	int m_tail;
	int m_size;

	IrSignal m_timing[IR_TIME_NO];
};

extern uint32_t sys_tick_10us;

#endif /* IR_IR_TIME_H_ */
