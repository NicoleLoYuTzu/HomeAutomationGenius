/**	\file	ir_sender.cpp
 * 	\brief	紅外線傳輸
 *  \author ysliu
 *  \date May 4, 2024
 *
 *	- 要傳送 IR code 時，呼叫 IR_Sender::SendCode() 以安排 IR code 的傳送時間。
 *	- IR_Sender::SendCode() 的參數 (code) 包含 custom code, data code 共 32 bits。
 *	- IR_Sender::SendCode() 以 IrSignal 結構將狀態及時間置入 IR_Timing 的 FIFO (IR_Timing::m_timing[]) 中。
 *	- TIM6 callback 函式定期呼叫 IR_Sender::processing() 以處理傳送事宜。
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
 * 	\param[in]	code	Custom code (16 bits) + data code (8 bits) + -data code (8 bits)
 *
 * 	預排傳輸狀態及時間到 IR_Timing 的 queue 中
 *
 */
void	IR_Sender::SendCode(uint32_t code)
{
	uint32_t timing = sys_tick_10us;

	/// - 前導碼 = 16T + 8T
	put(MARK, timing); timing += 16*NEC_TICK;
	put(SPACE, timing); timing += 8*NEC_TICK;

	/// - 32 bits data : T + T 或 T + 3T
	for (int i=0; i<32; i++)
	{
		put(MARK, timing); timing += NEC_TICK;
		put(SPACE, timing); timing += (code & 1) ? 3*NEC_TICK : NEC_TICK;
		code >>= 1;
	}
	/// - End of frame = T
	put(MARK, timing); timing += NEC_TICK;
	put(SPACE, timing);
}

/**	\brief	Process IR transmission.
 *
 *	傳送 queue 中預排的 IR 信號
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
 *	\param[in]	bOn	信號狀態
 *
 *	- IRTX 連接到 TIM3_ETR，用於控制 TIM3_CH3 的 PWM 信號要正常輸出或關閉。
 *	- IRTX2 為 IRTX 的反向，已可不用。
 */
void	IR_Sender::SetIR(bool bOn)
{
	HAL_GPIO_WritePin(IRTX_GPIO_Port, IRTX_Pin, bOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(IRTX2_GPIO_Port, IRTX2_Pin, bOn ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
