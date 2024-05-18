/** \file	ir_receiver.h
	\brief	IR code receiver.
*/
#ifndef IR_IR_RECEIVER_H_
#define IR_IR_RECEIVER_H_

#include "ir_base.h"

#define TIME_ERROR	42

#define	DAIKIN_TICK	44
#define	DAIKIN_ERR	30
#define MAX_STATE_TIME	1000

#define Bit(n) (1 << n)
#define LENGTH(x) ((int) (sizeof(x)/sizeof(x[0])))

/** \brief	IR code receiver
 *
 */
class IR_Receiver : public IR_Base
{
public:
	void	processing();
	IR_State	proc_IRsignal(int time);
	void	ShowIrCode(IR_State state);
	bool	check_IRcode(void);

	uint16_t	GetCode() {return m_ircode;}
	const char *GetController() {return m_pControllerName;}
	const char *GetBtnName() {return m_pkeyname;}
	void	ResetState() {m_state = ST_START;}

	inline bool about(int nTick) __attribute__((always_inline));

private:
	int		m_pulseWidth;
};

#endif
