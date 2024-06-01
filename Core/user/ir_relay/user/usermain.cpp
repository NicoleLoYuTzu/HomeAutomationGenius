/*
 * usermain.cpp
 *
 *  Created on: Mar 16, 2024
 *      Author: ysliu
 */

#include <stdio.h>

#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"

#include "../../ssd1306/ssd1306.h"
#include "ir_receiver.h"
#include "../ir_sender.h"
#include "../ir_code.h"

#include "usermain.h"

#define	BUSY_WAIT	0

IR_STATE g_waitState;
IR_Receiver ir_receiver;
IR_Sender ir_sender;

extern IR_CodeTable hdrplayer;

/**	\brief usermain() in C++.
 *
 *	called by main() in C.
 */
void usermain()
{
	// start timer with 10us interrupt interval.
	HAL_TIM_Base_Start_IT(&htim6);

	// initial next wait state
	g_waitState = (HAL_GPIO_ReadPin(IRRX_GPIO_Port, IRRX_Pin) == GPIO_PIN_SET) ? MARK : SPACE;
//	g_bEnableRelay = (HAL_GPIO_ReadPin(ENABLE_RELAY_GPIO_Port, ENABLE_RELAY_Pin) == GPIO_PIN_RESET) ? true : false;

	init_oled();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

	while(1)
	{
		flash_LD1();				// flashing LD1 in 1 Hz
		ir_receiver.processing();	// receive IR signals
	}
}

/**	\brief	Timer period elapsed callback function.
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6)
	{
		sys_tick_10us++;
		ir_sender.processing();		// transmit IR code
	}
}

/**	\brief	EXTI handler for IR, user button, and Relay setting.
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case IRRX_Pin:
		ir_receiver.put(g_waitState);
		g_waitState = (g_waitState == SPACE) ? MARK : SPACE;
		break;

	case BTN_Pin:
		ir_sender.SendCode(hdrplayer.GetIrCode("POWER"));
		break;

//	case ENABLE_RELAY_Pin:
//		g_bEnableRelay = (HAL_GPIO_ReadPin(ENABLE_RELAY_GPIO_Port, ENABLE_RELAY_Pin) == GPIO_PIN_RESET) ? true : false;
//		showRelayState();
//		ssd1306_UpdateScreen(&oled_i2c);
//		break;
	}
}

/**	\brief	Initial OLED display.
 *
 */
void	init_oled()
{
	// Init lcd using one of the stm32HAL i2c typedefs
	ssd1306_Init(&oled_i2c);

	// Write data to local screenbuffer
	ssd1306_printf(Font_5x7, White, "IR code Test");

	char ch=' ';
	for (int y=2; y<8; y++)
	{
		for (int x=0; x<21; x++)
		{
			ssd1306_SetCursor(x*Font_5x7.FontWidth, y*Font_5x7.FontHeight);
			ssd1306_WriteChar(ch++, Font_5x7, White);
		}
	}

	showRelayState();

	// Copy all data from local screenbuffer to the screen
	ssd1306_UpdateScreen(&oled_i2c);
}

/**	\brief	Flashing LD1 in 1 Hz.
 *
 */
void	flash_LD1()
{
	const uint32_t HALF_INTERVAL = 500L;
	static uint32_t lastLedToggleTime = 0;

	// Toggle LD1 per 0.5 sec.
#if BUSY_WAIT
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	HAL_Delay(500L);
#else
	uint32_t sysTick = HAL_GetTick();
	if ((sysTick - lastLedToggleTime) >= HALF_INTERVAL)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		lastLedToggleTime = sysTick;
	}
#endif
}

/**	\brief Retarget to USART3.
 *
 */
extern "C" int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3, (uint8_t *) &ch, 1, HAL_MAX_DELAY);

	return 0;
}
