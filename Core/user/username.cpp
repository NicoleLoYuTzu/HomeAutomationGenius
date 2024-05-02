/*
 * username.cpp
 *
 *  Created on: Mar 16, 2024
 *      Author: ysliu
 */

#include "stm32f7xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"

#include "stdio.h"
#include "../../ssd1306/ssd1306.h"
#include "irTime.h"
#include "ir_parser.h"

extern "C" void Reset_Handler();
extern "C" void Default_Handler();
extern uint32_t g_pfnVectors;
extern uint32_t g_pfnVectorsEnd;

void	flash_LD1();
void	ShowIrCode(IR_State state);
void	hw2_uid();

IR_STATE waitState;

uint32_t preSignalTick = 0;
IrSignalTime irSignal;
IR_Parser ir_parser;

/**	usermain() in c++.
 *
 */
extern "C" void usermain()
{
	// start timer with 10us interrupt interval.
	HAL_TIM_Base_Start_IT(&htim6);

	// initial next wait state
	waitState = (HAL_GPIO_ReadPin(IRDA_GPIO_Port, IRDA_Pin) == GPIO_PIN_SET) ? MARK : SPACE;

	// Init lcd using one of the stm32HAL i2c typedefs
	ssd1306_Init(&hi2c1);

	// Write data to local screenbuffer
	for (int y=0; y<70; y+=10)
	{
		ssd1306_SetCursor(0, y);
		ssd1306_WriteString("ABCDEFGHIJKLMNOPQR", Font_7x10, White);
	}

	// Copy all data from local screenbuffer to the screen
	ssd1306_UpdateScreen(&hi2c1);

	while(1)
	{
		// flashing LD1 in 1 Hz
		flash_LD1();

		//	process signal from remote controller
		if (irSignal.signalIn())
		{
			// toggle LD2
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

			IrSignal signal = irSignal.get();

			// if pulse width > 10 ms, it may mean a new code
			uint32_t pulseWidth = signal.time - preSignalTick;
			if (pulseWidth > MAX_STATE_TIME)
			{
				pulseWidth = 0;
				printf("\r\n");
			}

			// falling edge = end of SPACE state
			// rising edge = end of MARK state
			// a line = 17 bytes, take about 1.62 ms for 115200 bps
			if (signal.state == SPACE)
				printf("%3lu (M)\r\n", pulseWidth);
			else
				printf("%3lu (S) ", pulseWidth);

			IR_State state = ir_parser.proc_IRsignal(pulseWidth);
			if ((state == ST_DONE) || (state == ST_REPEAT))
				ShowIrCode(state);

			preSignalTick = signal.time;
		}
	}
}

//	\brief	Flashing LD1 in 1 Hz.
void	flash_LD1()
{
	const uint32_t HALF_INTERVAL = 500L;
	static uint32_t lastLedToggleTime = 0;

	// Flashing LD1 in 1 Hz
#if 0
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

/**	\brief	Show received IR code.
 *
 */
void	ShowIrCode(IR_State state)
{
	static int nBtn = 0;
	static int nRepeatChar = 0;
	static int nRepeatLine = 1;
	static char repeatChar = '.';

	if (state == ST_DONE)
	{
		uint16_t irCode = ir_parser.GetCode();
		const char *pBtnName = ir_parser.GetBtnName();

		char msg[28];
		sprintf(msg, "%03d %04X %-10s", ++nBtn, irCode, pBtnName);

		ssd1306_Fill(Black);
		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString(msg, Font_7x10, White);

		nRepeatChar = 0;
		nRepeatLine = 1;
		repeatChar = '.';
	}
	else
	{
		ssd1306_SetCursor(7*nRepeatChar, 10*nRepeatLine);
		ssd1306_WriteChar(repeatChar, Font_7x10, White);

		if (++nRepeatChar >= 18)
		{
			nRepeatChar = 0;
			if (++nRepeatLine > 5)
			{
				nRepeatChar = 0;
				nRepeatLine = 1;
				repeatChar++;
			}
		}
	}

	ssd1306_UpdateScreen(&hi2c1);

	ir_parser.ResetState();
}

/**	UART retarget.
 *
 */
extern "C" int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3, (uint8_t *) &ch, 1, HAL_MAX_DELAY);
	return 0;
}

/**	\brief	EXTI handler for IR.
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == IRDA_Pin)
	{
		irSignal.put(waitState);
		waitState = (waitState == SPACE) ? MARK : SPACE;
	}
}

/** \brief	homework 2: retarget, show UID.
 *
 */
void hw2_uid()
{
	static uint32_t uid[3] = {0, 0, 0};

	uid[0] = *(uint32_t *) UID_BASE;
	uid[1] = *(uint32_t *) (UID_BASE + 4);
	uid[2] = *(uint32_t *) (UID_BASE + 8);

	uint8_t *pLot = ((uint8_t *) &uid[1]);
	uint8_t *waf = ((uint8_t *) &uid[1]);
	uint16_t *x = ((uint16_t *) &uid[0]) + 1;
	uint16_t *y = ((uint16_t *) &uid[0]);

	printf("School ID: 11n61xx\r\n");
	printf("SBC Model: NUCLEO-F755ZI-Q\r\n");
	printf("SBC S/N: A192301649\r\n");
	printf("MCU UID = %08lX %08lX %08lX\r\n", uid[0], uid[1], uid[2]);
	printf("LOT-WAF-X-Y = %c%c%c%c%c%c%c-%02X-%04X-%04X\r\n", pLot[7], pLot[6], pLot[5], pLot[4],
			pLot[3], pLot[2], pLot[1], *waf, *x, *y);

#if 1
	const uint32_t toggleOnTime = 1L;
	const uint32_t toggleOffTime = 30L;
	int count = 0;
	uint32_t *pVector = &g_pfnVectors;
	for (int nVector = 0; pVector < &g_pfnVectorsEnd; nVector++)
	{
		if (*pVector == 0x0000)
			printf("Vector %3d = %08lX (unused)\r\n", nVector, *pVector++);
		else if ((*pVector & 0xFFFFFFFE) == ((uint32_t) &Default_Handler & 0xFFFFFFFE))
			printf("Vector %3d = %08lX (Default handler)\r\n", nVector, *pVector++);
		else if (nVector == 0)
			printf("Vector %3d = %08lX\r\n", nVector, *pVector++);
		else if ((*pVector & 0xFFFFFFFE) == ((uint32_t) &Reset_Handler & 0xFFFFFFFE))
			printf("Vector %3d = %08lX (Reset handler)\r\n", nVector, *pVector++);
		else
			printf("Vector %3d = %08lX (Used handler %d) \r\n", nVector, *pVector++, ++count);

		if (nVector & 1)
		{
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
			HAL_Delay(toggleOnTime);
		}
		else
		{
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
			HAL_Delay(toggleOffTime);
		}
	}
#endif

	printf("Done\r\n");
}
