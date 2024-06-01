/**	\file	ir_sender.h
 * 	\brief	紅外線傳輸 (標頭檔)
 *
 *  Created on: May 4, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_SENDER_H_
#define IR_IR_SENDER_H_

#include "ir_base.h"

/**	\brief	IR code sender.
 *
 */
class IR_Sender : public IR_Base
{
public:
	IR_Sender();
	void	processing();
	void	SendCode(uint32_t code);

private:
	void	SetIR(bool bOn);

	IrSignal m_signal;	///< 等待傳送的信號
};

extern IR_Sender ir_sender;

#endif /* IR_IR_SENDER_H_ */
