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
#include "irTime.h"
#include "../../ssd1306/ssd1306.h"
#include "../../ssd1306/fonts.h"
// 定義紅外線波形的最大長度
#define MAX_IR_WAVEFORM_LENGTH 100

extern "C" void Reset_Handler();
extern "C" void Default_Handler();
extern uint32_t g_pfnVectors;
extern uint32_t g_pfnVectorsEnd;

void hw2_uid();

uint32_t lastLedToggleTime = 0;
IR_STATE waitState;

uint32_t preSignalTick = 0;
IrTiming irTiming;

// 變數宣告
uint32_t irWaveform[MAX_IR_WAVEFORM_LENGTH]; // 紀錄紅外線波形的陣列
int irWaveformIndex = 0; // 紀錄波形的索引

/**	usermain() in c++.
 *
 */
extern "C" void usermain()
{
    // start timer with 10us interrupt interval.
    HAL_TIM_Base_Start_IT(&htim6);

    // initial next wait state
    waitState = (HAL_GPIO_ReadPin(IRDA_GPIO_Port, IRDA_Pin) == GPIO_PIN_SET) ? MARK : SPACE;


    //4/27add 從example 範例的main加進來的
    // Init lcd using one of the stm32HAL i2c typedefs
      if (ssd1306_Init(&hi2c1) != 0) {
        Error_Handler();
      }
      HAL_Delay(1000);

      ssd1306_Fill(Black);
      ssd1306_UpdateScreen(&hi2c1);

      HAL_Delay(1000);

//      // Write data to local screenbuffer
//      ssd1306_SetCursor(0, 0);
      ssd1306_WriteString("Nicole in", Font_11x18, White);
//
//      ssd1306_SetCursor(0, 36);
//      ssd1306_WriteString("Embedded system class", Font_11x18, White);
      // Write data to local screenbuffer
      ssd1306_SetCursor(0, 16);
      ssd1306_WriteString("Embedded", Font_7x10, White);
      ssd1306_SetCursor(0, 26);
      ssd1306_WriteString("system", Font_7x10, White);
      ssd1306_SetCursor(0, 36);
      ssd1306_WriteString("class", Font_7x10, White);


      // Draw rectangle on screen
      for (uint8_t i=0; i<28; i++) {
          for (uint8_t j=0; j<64; j++) {
              ssd1306_DrawPixel(100+i, 0+j, White);
          }
      }

      // Copy all data from local screenbuffer to the screen
      ssd1306_UpdateScreen(&hi2c1);

    while(1)
    {
        // Flashing LD1 in 1 Hz
#if 0
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        HAL_Delay(500L);
#else
        uint32_t sysTick = HAL_GetTick();
        if ((sysTick - lastLedToggleTime) >= 500L)
        {
            HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
            lastLedToggleTime = sysTick;
        }
#endif

        //	process signal from remote controller
        if (irTiming.hasEvent())
        {
            // toggle LD2
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

            IR_EVENT event = irTiming.get();

            // if pulse width > 10 ms, it may mean a new code
            uint32_t pulseWidth = event.time - preSignalTick;
            if (pulseWidth > MAX_STATE_TIME)
            {
                pulseWidth = 0;
                printf("\r\n");
            }

            // falling edge = end of SPACE state
            // rising edge = end of MARK state
            // a line = 17 bytes, take about 1.62 ms for 115200 bps


            if (event.state == SPACE)
                printf("%3lu (M)\r\n", pulseWidth);
            else
                printf("%3lu (S) ", pulseWidth);

            preSignalTick = event.time;
        }
    }
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
        irTiming.put(waitState);
        waitState = (waitState == SPACE) ? MARK : SPACE;

//        // 捕獲 GPIO 引腳的狀態變化，並記錄相應的時間戳
//        uint32_t timestamp = HAL_GetTick(); // 獲取當前時間
//        irWaveform[irWaveformIndex++] = timestamp; // 將時間戳添加到波形陣列中
//
//        // 將時間戳以 log 的形式印出
//        printf("Timestamp: %lu\r\n", timestamp);
    }
}

/** \brief	homework 2: retarget, show UID.
 *
 */
void hw2_uid()
{
    uint32_t uid[3] = {0};

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




