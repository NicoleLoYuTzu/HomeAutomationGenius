/*
 * ir_sender.h
 *
 *  Created on: May 4, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_SENDER_H_
#define IR_IR_SENDER_H_

#include "ir_base.h"

//	IR code sender.
class IR_Sender : public IR_Base
{
public:
	IR_Sender();
	void	processing();
	void	SendCode(uint32_t code);

private:
	void	SetIR(bool bOn);

	IrSignal m_signal;
};

extern IR_Sender ir_sender;

#endif /* IR_IR_SENDER_H_ */
