/*
 * ir_code.cpp
 *
 *  Created on: May 1, 2024
 *      Author: ysliu
 */

#include "ir_code.h"

/**	\brief	Initial a code table.
 *
 */
IR_CodeTable::IR_CodeTable(const char *controllerName, uint16_t customCode, const IR_CODE *pCodeTable, int tableLen)
{
	m_pControllerName = controllerName;
	m_customcode = customCode;
	m_pCodeTable = pCodeTable;
	m_nTableLen = tableLen;
}

/**	\brief	Search IR code in code table.
 *
 */
bool	IR_CodeTable::FindKey(uint8_t datacode, const char **ppControllerName, const char **ppKeyname)
{
	*ppControllerName = m_pControllerName;

	for (int i=0; i<m_nTableLen; i++)
	{
		if (datacode == m_pCodeTable[i].irCode)
		{
			*ppKeyname = m_pCodeTable[i].keyName;
			return true;
		}
	}

	*ppKeyname = "Unknown";
	return true;
}
