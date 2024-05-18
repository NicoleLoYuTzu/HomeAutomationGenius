/** \file	ir_receiver.h
	\brief	IR code receiver.
*/
#ifndef IR_IR_RECEIVER_H_
#define IR_IR_RECEIVER_H_

#include "ir_base.h"

#define MAX_STATE_TIME	1000

#define Bit(n) (1 << n)

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
	int		m_pulseWidth;

	bool	m_bWaitToRelay;
	const char *m_pRelayBtnName;
};

void	showRelayState();

extern bool g_bEnableRelay;

#endif
