/** \file	ir_receiver.h
	\brief	紅外線接收 (標頭檔)
*/
#ifndef IR_IR_RECEIVER_H_
#define IR_IR_RECEIVER_H_

#include "ir_base.h"

#define MAX_STATE_TIME	1000

#define Bit(n) (1 << n)

#if defined(STM32H723xx)
#define	LD1_GPIO_Port LED_GREEN_GPIO_Port
#define	LD2_GPIO_Port LED_YELLOW_GPIO_Port
#define	LD3_GPIO_Port LED_RED_GPIO_Port
#define	LD1_Pin LED_GREEN_Pin
#define	LD2_Pin LED_YELLOW_Pin
#define	LD3_Pin LED_RED_Pin
#endif

/** \brief	IR code receiver
 *
 */
class IR_Receiver : public IR_Base
{
public:
	IR_Receiver();
	void	processing();
	IR_State	proc_IRsignal(int time);
	void	ShowIrCode(IR_State state);
	int		check_IRcode(void);

	inline bool about(int nTick) __attribute__((always_inline));

private:
	int		m_pulseWidth;	///< 只是為了方便呼叫 about() 時使用

	bool	m_bWaitToRelay;	///< 安排中繼碼傳輸中
	const char *m_pRelayBtnName;	///< 要傳輸的按鈕名稱
};

void	showRelayState();

extern bool g_bEnableRelay;

#endif
