/**	@file	user_usart.h
    @brief	USART communication with FIFO buffers.
	@author Liu, Yi-Sheng
*/

#ifndef __user_usart_H
#define __user_usart_H

#include <usart.h>
#include "fifo.h"

#define	SioBufSize	1024

#define	_CR	13
#define	_LF	10

#define LENGTH(x) ((int) (sizeof(x)/sizeof(x[0])))

/**	\brief	UART communication with FIFO buffers.

UART RX:
\dot
digraph uartRx
{
	node [shape=box, height=0.1, fontname=Verdana, fontsize=10];
	edge [arrowsize=0.8, fontname=Verdana];
	graph[nodesep=0.15, ranksep=0.4];

	usermain [label = "usermain", URL = "\ref usermain"];
	timectrl [label = "timectrl", URL = "\ref timectrl"];
	HAL_UART_Receive_IT [label = "HAL_UART_Receive_IT", URL = "\ref HAL_UART_Receive_IT"];

	usermain -> timectrl -> HAL_UART_Receive_IT;

	HAL_UART_RxCpltCallback [label = "HAL_UART_RxCpltCallback", URL = "\ref HAL_UART_RxCpltCallback"];
	HAL_UART_Receive_IT -> HAL_UART_RxCpltCallback [style = dashed];

	RxCompleted [label = "uart_stlink::RxCompleted", URL = "\ref CUart::RxCompleted"];
	PutData [label = "m_rxBuf::PutData", URL = "\ref FifoBuffer::PutData"];

	{rank = same; PutData; HAL_UART_Receive_IT;}

	HAL_UART_RxCpltCallback -> RxCompleted;
	RxCompleted -> PutData;
	RxCompleted -> HAL_UART_Receive_IT;
}
\enddot

UART TX:
\dot
digraph uartTx
{
	node [shape=box, height=0.1, fontname=Verdana, fontsize=10];
	edge [arrowsize=0.8, fontname=Verdana];
	graph[nodesep=0.15, ranksep=0.4];

	usermain [label = "usermain", URL = "\ref usermain"];
	timectrl [label = "timectrl", URL = "\ref timectrl"];
	PutModbusByte [label = "PutModbusByte", URL = "\ref PutModbusByte"];
	SaveTxData [label = "SaveTxData", URL = "\ref SaveTxData"];

	usermain -> timectrl -> PutModbusByte -> SaveTxData;

	PutData [label = "m_txBuf::PutData", URL = "\ref FifoBuffer::PutData"];
	GetData [label = "m_txBuf::GetData", URL = "\ref FifoBuffer::GetData"];
	HAL_UART_Transmit_IT [label = "HAL_UART_Transmit_IT", URL = "\ref HAL_UART_Transmit_IT"];

	{rank = same; PutData; GetData; HAL_UART_Transmit_IT;}

	SaveTxData -> PutData;
	SaveTxData -> GetData;
	SaveTxData -> HAL_UART_Transmit_IT;

	HAL_UART_TxCpltCallback [label = "HAL_UART_TxCpltCallback", URL = "\ref HAL_UART_TxCpltCallback"];
	TxCompleted [label = "uart_stlink::TxCompleted", URL = "\ref CUart::TxCompleted"];

	HAL_UART_Transmit_IT -> HAL_UART_TxCpltCallback [style = dashed];
	HAL_UART_TxCpltCallback -> TxCompleted;

	TxCompleted -> HAL_UART_Transmit_IT;
	TxCompleted -> GetData;
}
\enddot

*/
class CUart
{
public:
	CUart(USART_TypeDef *uartBase, UART_HandleTypeDef *huart); // , FlashLED *pLed = NULL);

	void	ScheduleRxIT();
	void	RxCompleted();

	void	SaveTxData(uint8_t byte);
	void	TxCompleted();

	void	ChangeBaudrate(uint32_t baudrate, uint32_t parity);

	FifoBuffer m_rxBuf;
	FifoBuffer m_txBuf;

	UART_HandleTypeDef *m_huart;

	static CUart *g_pUarts[];
	static int m_nUarts;
protected:
	USART_TypeDef *m_uartBase;

	uint8_t sio_rx_byte;
	uint8_t sio_tx_byte;

	volatile bool m_bTxIdle;
};

#endif
