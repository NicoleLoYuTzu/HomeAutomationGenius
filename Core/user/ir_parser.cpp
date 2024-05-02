/** \file	ir_parser.cpp
	\brief	IR code parser.
*/

#include <math.h>

#include "ir_parser.h"
#include "ircode.h"

#define LENGTH(x) ((int) (sizeof(x)/sizeof(x[0])))

// Constructor
IR_Parser::IR_Parser()
{
	m_state = ST_START;
}

/** \brief	Process IR signal.
	The IR signal is process by a state machine.
*/
IR_State	IR_Parser::proc_IRsignal(int pulseWidth)
{
	m_pulseWidth = pulseWidth;

	switch(m_state)
	{
	case ST_START:
		if (about(16*IR_TICK))			// 9 ms
			m_state = ST_FRAMETYPE;
		break;

	case ST_FRAMETYPE:
		if (about(8*IR_TICK))			// 4.5 ms
		{
			m_bitNo = 0;
			m_byteNo = 0;
			m_state = ST_DATAFRAME;
		}
		else if (about(4*IR_TICK))		// 2.25 ms
			m_state = ST_REPEATCODE;
		else
			m_state = ST_START;
		break;

	case ST_DATAFRAME:
		if (about(IR_TICK)) 			// 0.56 ms
			m_state = ST_DATABIT;
		else
			m_state = ST_START;	// frame error
		break;

	case ST_DATABIT:
		if (about(IR_TICK) || about(3*IR_TICK))	// 0.56 ms or 1.68 ms
		{
			m_dataCode[m_byteNo] >>= 1;
			if (about(IR_TICK))
				m_dataCode[m_byteNo] &= 0x7f;	// bit = 0
			else
				m_dataCode[m_byteNo] |= 0x80;	// bit = 1
			m_state = ST_DATAFRAME;
			if (++m_bitNo == 8)
			{
				m_bitNo = 0;
				if (++m_byteNo == 4)
					m_state = check_IRcode() ? ST_DONE : ST_START;
			}
		}
		else
			m_state = ST_START;
		break;

	case ST_REPEATCODE:
		if (about(IR_TICK)) 			// 0.56 ms
			m_state = ST_REPEAT;
		else
			m_state = ST_START;
		break;

	case ST_DONE:
	case ST_REPEAT:
		break;
	}

	return m_state;
}

/**	\brief	Check pulse width.
 *
 */
bool	IR_Parser::about(int nTick)
{
	return abs(m_pulseWidth - nTick) < TIME_ERROR;
}

/** \brief	Check for IR code.

	The received IR code is checked against a code table that is
	retrieved from a real IR remote controller.
*/
bool	IR_Parser::check_IRcode(void)
{
	if ((m_dataCode[0] + m_dataCode[1] == 0xff) &&
		(m_dataCode[2] + m_dataCode[3] == 0xff))
	{
		m_ircode = ((uint16_t) m_dataCode[0] << 8) + m_dataCode[2];

		for (int i=0; i<LENGTH(codeTable); i++)
		{
			if (m_ircode == codeTable[i].irCode)
			{
				m_pkeyname = codeTable[i].keyName;
				return true;
			}
		}

		m_pkeyname = "Unknown";
		return true;
	}

	return false;
}
