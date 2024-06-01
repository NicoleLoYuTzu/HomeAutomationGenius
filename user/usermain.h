/**	\file	usermain.h
 * 	\brief	C++ 主程式 (標頭檔)
 *
 *  Created on: May 5, 2024
 *      Author: ysliu
 */

#ifndef USERMAIN_H_
#define USERMAIN_H_

void 	usermain(void *argument);
void 	taskConsole(void *argument);
void 	StartLcdTask(void *argument);
void	init_oled();


#ifdef __cplusplus
extern "C" {
#endif

void StartDefaultTask(void *argument);
void user_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* USERMAIN_H_ */
