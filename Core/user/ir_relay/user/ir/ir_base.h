/*
 * ir_base.h
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

	uint16_t	GetCode() {return m_ircode;}
	const char *GetController() {return m_pControllerName;}
	const char *GetBtnName() {return m_pkeyname;}
	void	ResetState() {m_state = ST_START;}

	uint8_t	m_datacode[19];		//!< data code of IR signal
protected:
	IR_State	m_state;		//!< current decoding state
	int		m_bitNo;			//!< current bit number of IR signal
	int		m_byteNo; 			//!< current byte number of IR signal

	uint16_t	m_ircode;
	const char *m_pControllerName;
	const char *m_pkeyname;
};

#endif /* IR_IR_BASE_H_ */
