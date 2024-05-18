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
	void	SendCode(uint32_t code, bool bLoopback = false);

private:
	void	sendFrame();

	bool	m_bTxCode;
	uint32_t	m_code;
};

#endif /* IR_IR_SENDER_H_ */
