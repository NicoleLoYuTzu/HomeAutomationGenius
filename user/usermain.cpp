/**	\file	usermain.cpp
 * 	\brief	C++ 主程式
 *
 *  Created on: Mar 16, 2024
 *      Author: ysliu
 */

#include <stdio.h>

#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include <cstring>

#include "ssd1306.h"
#include "ir_receiver.h"
#include "ir_sender.h"
#include "ir_code.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "usermain.h"

IR_STATE g_waitState;		///< IR 接收功能中，預期引發下一個 interrupt 的狀態
IR_Receiver ir_receiver;	///< IR_Receiver 的物件 (全域變數) 用於處理 IR 接收
IR_Sender ir_sender;		///< IR_Sender 的物件 (全域變數) 用於處理 IR 傳送

extern IR_CodeTable hdrplayer;	///< 按下 USER 按鈕時，透過 hdrplayer 傳送遙控器的「電源開關」碼

static osThreadAttr_t task_attr =
{
	.stack_size = 128 * 4,
	.priority = osPriorityNormal,
};

__IO uint32_t OsStatus = 0;

extern uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static uint8_t SSD1306_Buffer2[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/**	\brief	Create tasks and flashing LD1 in 1 Hz.
 *
 */
void	StartDefaultTask(void *argument)
{
	// create and start task usermain()
	task_attr.name = "usermain";
	task_attr.priority = osPriorityNormal1;
	osThreadId_t threadHandle1 = osThreadNew(usermain, NULL, (const osThreadAttr_t*) &task_attr);
    OsStatus = osThreadResume(threadHandle1);

	// create and start task console()
#if 1
	task_attr.name = "console";
	task_attr.priority = osPriorityBelowNormal6;
	osThreadId_t threadHandle2 = osThreadNew(taskConsole, NULL, (const osThreadAttr_t*) &task_attr);
    OsStatus = osThreadResume(threadHandle2);
#endif

	// create and start task StartLcdTask()
	task_attr.name = "LcdTask";
	task_attr.priority = osPriorityBelowNormal7;
	osThreadId_t threadHandle3 = osThreadNew(StartLcdTask, SSD1306_Buffer2, (const osThreadAttr_t*) &task_attr);
    OsStatus = osThreadResume(threadHandle3);

    while(1)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		osDelay(500);
	}
}

/**	\brief usermain()
 *
 */
void usermain(void *argument)
{
	/// - Start timer 6 with 10us interrupt interval.
	HAL_TIM_Base_Start_IT(&htim6);

	/// - 設定下一個預期的 IR 信號狀態
	/// - Get ENABLE_RELAY state
	g_waitState = (HAL_GPIO_ReadPin(IRRX_GPIO_Port, IRRX_Pin) == GPIO_PIN_SET) ? MARK : SPACE;
//	g_bEnableRelay = (HAL_GPIO_ReadPin(ENABLE_RELAY_GPIO_Port, ENABLE_RELAY_Pin) == GPIO_PIN_RESET) ? true : false;

	/// - 啟動 TIM3_CH3 輸出 38 KHz, Duty cycle 1/3 的 PWM 信號
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

	while(1)
	{
		ir_receiver.processing();	/// - Process received IR signals
		osDelay(1);
	}
}

/**	\brief	OLED display updater.
 *
 */
void StartLcdTask(void *argument)
{
	uint8_t *pBuffer = (uint8_t *) argument;

	/// - Initial OLED display
	init_oled();

	while(1)
	{
	    uint8_t i;
	    uint8_t command;

	    memcpy(pBuffer, SSD1306_Buffer, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
	    for (i = 0; i < 8; i++)
	    {
	        command = 0xB0 + i;
	        HAL_I2C_Mem_Write(&oled_i2c, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, 10);
	        command = 0x00;
	        HAL_I2C_Mem_Write(&oled_i2c, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, 10);
	        command = 0x10;
	        HAL_I2C_Mem_Write(&oled_i2c, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, 10);

	        HAL_I2C_Mem_Write(&oled_i2c, SSD1306_I2C_ADDR, 0x40, 1, &pBuffer[SSD1306_WIDTH * i], SSD1306_WIDTH, 100);
	    }
		osDelay(50);
	}
}

/**	\brief 「計時到期」中斷的 callback 函式
 *
 * -# TIM6 : 10 us 中斷週期，呼叫 ir_sender.processing() 以處理 IR code 的傳送
 */
void user_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6)
	{
		sys_tick_10us++;
		ir_sender.processing();		// transmit IR code
	}
}

/**	\brief 外部輸入 (EXTI) 的 callback 函式
 *
 * -# IR 接收 : 用於紀錄 IR on/off 的時間 (假設 on/off 都無丟失)。
 * -# User button : 按下時，傳輸測試的 IR code，用於開發過程中的測試。
 * -# Relay setting : 用於設定是否啟用 relay 功能，此 I/O 設定的狀態會顯示在 OLED 上。
 *  -# 有必要量測在 OLED 顯示所花費的時間，如果超過 10 us，程式需要進行調整。
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case IRRX_Pin:
		ir_receiver.put(g_waitState);
		g_waitState = (g_waitState == SPACE) ? MARK : SPACE;
		break;

#if defined(STM32F439xx) || defined(STM32F756xx)
	case USER_Btn_Pin:
#elif defined(STM32H723xx) || defined(STM32H753xx)
	case B1_Pin:
#elif defined(STM32H755xx)
	case BTN_Pin:
#endif
		ir_sender.SendCode(hdrplayer.GetIrCode("POWER"));
		break;

//	case ENABLE_RELAY_Pin:
//		g_bEnableRelay = (HAL_GPIO_ReadPin(ENABLE_RELAY_GPIO_Port, ENABLE_RELAY_Pin) == GPIO_PIN_RESET) ? true : false;
		showRelayState();
//		ssd1306_UpdateScreen(&oled_i2c);
		break;
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
	ssd1306_printf(Font_5x7, White, "IR FreeRTOS");

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
//	ssd1306_UpdateScreen(&oled_i2c);
}
