/*
 * ir_code.h
 *
 *  Created on: Apr 27, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_CODE_H_
#define IR_IR_CODE_H_

#include "stdint.h"

#define LENGTH(x) ((int) (sizeof(x)/sizeof(x[0])))

#define	MAX_CONTROLLER	10

/**	IR code and key name.
 *
 */
typedef struct
{
	uint16_t	irCode; 		//!< IR code
	const char 	*keyName;		//!< key name
} IR_CODE;

/**	IR code table.
 *
 */
class IR_CodeTable
{
	friend class IR_Receiver;
public:
	IR_CodeTable(const char *controllerName, uint16_t customCode, const IR_CODE *pCodeTable, int tableLen);

	static bool	FindKey(uint16_t customcode, uint8_t datacode, const char **ppControllerName, const char **ppKeyname);

	bool	FindKey(uint8_t datacode, const char **ppControllerName, const char **ppKeyname);
	uint32_t	GetIrCode(int index);
	uint32_t	GetIrCode(const char *pKeyName);
	uint32_t	IrCode(const uint16_t irCode);
private:
	static int m_nController;
	static IR_CodeTable *m_pController[];

	const char *m_pControllerName;
	uint16_t	m_customcode;
	const IR_CODE *m_pCodeTable;
	int			m_nTableLen;
};

#endif /* IR_IR_CODE_H_ */
