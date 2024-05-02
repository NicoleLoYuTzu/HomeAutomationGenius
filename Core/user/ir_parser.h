/** \file	ir_parser.h
	\brief	IR code parser.
*/
#ifndef __IR_PARSER_H
#define __IR_PARSER_H

#include "stdint.h"

#define IR_TICK 	56
#define TIME_ERROR	42

#define Bit(n) (1 << n)

enum IR_State {
	ST_START,
	ST_FRAMETYPE,
	ST_DATAFRAME,
	ST_DATABIT,
	ST_REPEATCODE,
	ST_DONE,
	ST_REPEAT,
};

typedef struct
{
	uint16_t	irCode; 		//!< IR code
	const char 	*keyName;		//!< key name
} IR_CODE;

//	IR code parser
class IR_Parser
{
public:
	IR_Parser();

	IR_State	proc_IRsignal(int time);
	bool	check_IRcode(void);

	uint16_t	GetCode() {return m_ircode;}
	const char *GetBtnName() {return m_pkeyname;}
	void	ResetState() {m_state = ST_START;}

private:
	inline bool about(int nTick) __attribute__((always_inline));

	int		m_pulseWidth;

	IR_State	m_state;		//!< current decoding state
	int		m_bitNo;			//!< current bit number of IR signal
	int		m_byteNo; 			//!< current byte number of IR signal
	uint8_t	m_dataCode[4];		//!< data code of IR signal

	uint16_t	m_ircode;
	const char *m_pkeyname;
};

#endif
