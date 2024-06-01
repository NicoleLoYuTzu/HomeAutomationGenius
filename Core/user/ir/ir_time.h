/**	\file	ir_time.h
 * 	\brief	紅外線時序 (標頭檔)
 *
 *  Created on: Apr 19, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_TIME_H_
#define IR_IR_TIME_H_

#include "stdint.h"

#define IR_TIME_NO	400

#define NEC_TICK 	56
#define TIME_ERROR	42

#define	DAIKIN_TICK	44
#define	DAIKIN_ERR	30

enum IR_STATE {SPACE, MARK};

/**	\brief	IR 信號時間與狀態
 *
 */
typedef struct
{
	uint32_t time;	///< 信號時間 (單位: 10us)
	IR_STATE state;	///< 信號狀態
} IrSignal;

/**	\brief	Timer in 10 us resolution.
 *
 */
class IR_Timing
{
public:
	IR_Timing();

	void		put(IR_STATE state);
	void		put(IR_STATE state, uint32_t time_10us);
	IrSignal	get();

protected:
	uint32_t preSignalTick;	///< 前一個狀態的時間

	int m_head;		///< Head pointer to queue
	int m_tail;		///< Tail pointer to queue
	int m_size;		///< Number of elements in queue

	IrSignal m_timing[IR_TIME_NO];	///< Timing queue
};

extern uint32_t sys_tick_10us;

#endif /* IR_IR_TIME_H_ */
