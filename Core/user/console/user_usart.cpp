/**	@file	user_usart.cpp
    @brief	USART communication.
	@author Liu, Yi-Sheng

    Use USART3 for data communication.

	- 2020/09/14 Created.
	- 2020/09/22 Add ChangeBaudrate().
	- 2020/11/09 Set global variables as data members to support multiple UARTs.
	- 2022/07/02 Rewrite for code reuse.
*/

#include "user_usart.h"

CUart *CUart::g_pUarts[5];
int	CUart::m_nUarts = 0;

/**	\brief	Constructor.
*/
CUart::CUart(USART_TypeDef *uartBase, UART_HandleTypeDef *huart)
	: m_rxBuf(SioBufSize), m_txBuf(SioBufSize)
{
	m_uartBase = uartBase;
	m_huart = huart;

	m_bTxIdle = true;

	if (m_nUarts < LENGTH(g_pUarts))
	{
		g_pUarts[m_nUarts] = this;
		m_nUarts++;
	}
}

/**	\brief	Schedule to receive data in interrupt mode.
*/
void	CUart::ScheduleRxIT()
{
	HAL_UART_Receive_IT(m_huart, &sio_rx_byte, 1);
}

/**	\brief	Save received data in FIFO.

	Schedule to receive next data byte.
*/
void	CUart::RxCompleted()
{
	m_rxBuf.PutData(sio_rx_byte);

	ScheduleRxIT();
}

/**	\brief	Save data to transmit in TX buffer.

	Schedule to transmit if TX buffer is ready.
*/
void	CUart::SaveTxData(uint8_t byte)
{
	__disable_irq();
	m_txBuf.PutData(byte);
	__enable_irq();

	if (m_bTxIdle)
	{
		__disable_irq();
		sio_tx_byte = m_txBuf.GetData();
		__enable_irq();

		m_bTxIdle = false;

		HAL_UART_Transmit_IT(m_huart, &sio_tx_byte, 1);
	}
}

/**	\brief	Tx completed called by ISR.
*/
void	CUart::TxCompleted()
{
	if (m_txBuf.DataSize() == 0)
		m_bTxIdle = true;
	else
	{
		sio_tx_byte = m_txBuf.GetData();
		HAL_UART_Transmit_IT(m_huart, &sio_tx_byte, 1);
	}
}

/**	\brief	Change baudrate on the fly.
 */
void CUart::ChangeBaudrate(uint32_t baudrate, uint32_t parity)
{
	HAL_UART_AbortReceive_IT(m_huart);

	if (HAL_UART_DeInit(m_huart) != HAL_OK)
	{
		Error_Handler();
	}

	m_huart->Instance = m_uartBase;
	m_huart->Init.BaudRate = baudrate;
	m_huart->Init.WordLength = UART_WORDLENGTH_8B;
	m_huart->Init.StopBits = UART_STOPBITS_1;
	m_huart->Init.Parity = parity;
	m_huart->Init.Mode = UART_MODE_TX_RX;
	m_huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	m_huart->Init.OverSampling = UART_OVERSAMPLING_16;

#if !defined(STM32F439xx)
	m_huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	m_huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
#endif

	if (HAL_UART_Init(m_huart) != HAL_OK)
	{
		Error_Handler();
	}

	ScheduleRxIT();
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	for (int i=0; i<CUart::m_nUarts; i++)
		if (huart == CUart::g_pUarts[i]->m_huart)
			CUart::g_pUarts[i]->RxCompleted();
}

/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	for (int i=0; i<CUart::m_nUarts; i++)
		if (huart == CUart::g_pUarts[i]->m_huart)
			CUart::g_pUarts[i]->TxCompleted();
}

