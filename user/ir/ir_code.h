/**	\file	ir_code.h
 * 	\brief	紅外線遙控器 IR code 定義及查詢 (標頭檔)
 *
 *  Created on: Apr 27, 2024
 *      Author: ysliu
 */

#ifndef IR_IR_CODE_H_
#define IR_IR_CODE_H_

#include "stdint.h"

#define LENGTH(x) ((int) (sizeof(x)/sizeof(x[0])))

#define	MAX_CONTROLLER	10

/**	\brief	IR code and key name.
 *
 */
typedef struct
{
	uint16_t	irCode; 		///< IR code
	const char 	*keyName;		///< key name
} IR_CODE;

/**	\brief	IR code table.
 *
 */
class IR_CodeTable
{
	friend class IR_Receiver;
public:
	IR_CodeTable(const char *controllerName, uint16_t customCode, const IR_CODE *pCodeTable, int tableLen);

	static bool	SearchControllers(uint16_t customcode, uint8_t datacode, const char **ppControllerName, const char **ppKeyname);

	bool	FindKey(uint8_t datacode, const char **ppControllerName, const char **ppKeyname);
	uint32_t	GetIrCode(int index);
	uint32_t	GetIrCode(const char *pKeyName);
	uint32_t	IrCode(const uint16_t irCode);
private:
	static int m_nController;	///< 遙控器個數
	static IR_CodeTable *m_pController[];		///< 遙控器清單

	const char *m_pControllerName;	///< 遙控器名稱
	uint16_t	m_customcode;		///< 遙控器 custom code
	const IR_CODE *m_pCodeTable;		///< Pointer to IR code table
	int			m_nTableLen;			///< 遙控器按鈕個數
};

#endif /* IR_IR_CODE_H_ */
