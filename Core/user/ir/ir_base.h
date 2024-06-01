/**	\file	ir_base.h
 * 	\brief	紅外線遙控器通信基礎類別 (標頭檔)
 *
 *  Created on: May 4, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_BASE_H_
#define IR_IR_BASE_H_

#include "ir_time.h"

enum IR_State {
	ST_IDLE,
	ST_START,
	NEC_LEADING,
	NEC_DATAFRAME,
	NEC_DATABIT,
	NEC_REPEATCODE,
	NEC_DONE,
	NEC_DONE_REPEAT,

	DAIKIN_LEADING,
	DAIKIN_DATAFRAME,
	DAIKIN_DATABIT,
	DAIKIN_DONE,
};

/** \brief	Base class to process IR signal.
 *
 */
class IR_Base : public IR_Timing
{
public:
	IR_Base();

	uint16_t	GetCode() {return m_ircode;}	//!< 傳回 IR data code
	const char *GetController() {return m_pControllerName;}	//!< 傳回遙控器名稱
	const char *GetBtnName() {return m_pkeyname;}			//!< 傳回收到 IR code 的按鈕名稱
	void	ResetState() {m_state = ST_START;}

	uint8_t	m_datacode[19];		//!< data code of IR signal
protected:
	IR_State	m_state;		//!< current decoding state
	int		m_bitNo;			//!< current bit number of IR signal
	int		m_byteNo; 			//!< current byte number of IR signal

	uint16_t	m_ircode;		//!< IR data code
	const char *m_pControllerName;	//!< Pointer to controller name
	const char *m_pkeyname;			//!< Pointer to key name
};

#endif /* IR_IR_BASE_H_ */
