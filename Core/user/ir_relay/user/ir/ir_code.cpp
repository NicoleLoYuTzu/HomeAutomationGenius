/*
 * ir_code.cpp
 *
 *  Created on: May 1, 2024
 *      Author: ysliu
 */

#include <string.h>

#include "ir_code.h"

int IR_CodeTable::m_nController = 0;
IR_CodeTable *IR_CodeTable::m_pController[MAX_CONTROLLER];

/**	\brief	Initial a code table.
 *
 */
IR_CodeTable::IR_CodeTable(const char *controllerName, uint16_t customCode, const IR_CODE *pCodeTable, int tableLen)
{
	m_pControllerName = controllerName;
	m_customcode = customCode;
	m_pCodeTable = pCodeTable;
	m_nTableLen = tableLen;

	if (m_nController < MAX_CONTROLLER)
		m_pController[m_nController++] = this;
}

/**	\brief	Get IR code by index.
 *
 */
uint32_t	IR_CodeTable::GetIrCode(int index)
{
	if ((index >= 0) && (index < m_nTableLen))
		return IrCode(m_pCodeTable[index].irCode);

	return 0;
}

/**	\brief	Get IR code by key name.
 *
 */
uint32_t	IR_CodeTable::GetIrCode(const char *pKeyName)
{
	for (int i=0; i<m_nTableLen; i++)
		if (strcmp(pKeyName, m_pCodeTable[i].keyName) == 0)
			return IrCode(m_pCodeTable[i].irCode);

	return 0;
}

/**	\brief	Build IR code.
 *
 */
uint32_t	IR_CodeTable::IrCode(const uint16_t irCode)
{
	uint16_t invIrCode = ~irCode;

	return (invIrCode << 24) | (irCode << 16) | m_customcode;
}

/**	\brief	Search all controllers for received IR code.
 *
 */
bool	IR_CodeTable::FindKey(uint16_t customcode, uint8_t datacode, const char **ppControllerName, const char **ppKeyname)
{
	for (int i=0; i<m_nController; i++)
		if (customcode == m_pController[i]->m_customcode)
			return m_pController[i]->FindKey(datacode, ppControllerName, ppKeyname);

	return false;
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
