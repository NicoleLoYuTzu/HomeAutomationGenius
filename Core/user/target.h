
#ifndef __target_H
#define __target_H

#if defined(STM32F303xC) || defined(STM32F303xE)

#include "stm32f3xx_hal.h"

#elif defined(STM32F439xx)

#include "stm32f4xx_hal.h"

#elif defined(STM32F756xx) || defined(STM32F767xx)

#include "stm32f7xx_hal.h"

#elif defined(STM32H723xx) || defined(STM32H753xx)

#include "stm32h7xx_hal.h"

#endif

#ifdef __cplusplus
extern "C"{
#endif

void usermain();
void ErrorHalt(void);

#ifdef __cplusplus
}
#endif
#endif

