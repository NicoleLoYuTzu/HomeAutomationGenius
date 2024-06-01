/** \file	proccmd.cpp
	\brief	Task to process UART command.
*/

#include <stdio.h>
#include <string.h>

#include <stm32f7xx_hal.h>

#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <task.h>

#include "command.h"
#include "ansi.h"

//	Firmware version and module name

const char g_version[] = "V1.00";		//!< Firmware version
const char g_moduleName[] = "STM32H755";

/** \brief	Show software version.

	The user interface look like:
\verbatim
Kernel Information: FreeRTOS V10.3.1
Kernel Version    : 100005169
Kernel API Version: 100005169
*** STM32H755 V1.00 (May 31 2024 19:11:04)
$ $
\endverbatim
*/
int	ShowVersion(void)
{
	osVersion_t osv;
	char infobuf[32];

	osStatus_t status = osKernelGetInfo(&osv, infobuf, sizeof(infobuf));

	if (status == osOK)
	{
		printf("*** %s %s (%s %s)\n", g_moduleName, g_version, __DATE__, __TIME__);
	    printf("Kernel Information: %s\n", infobuf);
	    printf("Kernel Version    : %lu\n", osv.kernel);
	    printf("Kernel API Version: %lu\n", osv.api);
	}

	return 0;
}

/** \brief  Task management.

    - task					Show tasks
    - task start prio	    resume a task
    - task stop  prio	    suspend a task
*/
int		proc_task()
{
	if (g_parm[0] == '\0')
	{
		ShowTask();
	}
	else if (strcmp(g_parm, "?") == 0)
	{
		printf("task                : Show tasks\n");
		printf("task start <prio>   : resume a task\n");
		printf("task stop <prio>    : suspend a task\n");
	}
	else if (strcmp(g_parm, "start") == 0)
	{
		g_pArg = get_parm(g_pArg, g_parm, MAX_PARM_LEN, true);
		TaskHandle_t handle = xTaskGetHandle(g_parm);
		if (handle == NULL)
			printf("Task %s not found\n", g_parm);
		else
			vTaskResume(handle);
	}
	else if (strcmp(g_parm, "stop") == 0)
	{
		g_pArg = get_parm(g_pArg, g_parm, MAX_PARM_LEN, true);
		TaskHandle_t handle = xTaskGetHandle(g_parm);
		if (handle == NULL)
			printf("Task %s not found\n", g_parm);
		else
			vTaskSuspend(handle);
	}
	else
		return parm_error(g_parm);

	return E_OK;
}

void	ShowTask()
{
	static char	pcWriteBuffer[40*8];

	int n = uxTaskGetNumberOfTasks();
	printf("%d tasks\n", n);

	vTaskList(pcWriteBuffer);
	printf("%s", pcWriteBuffer);
}

/** \brief	Reset system user command parser.

	This routine shows last reset source if no parameter is given.
	Reset source is one of the following:
	- Power on or VDD monitor.
	- Hardware (/RST pin).
	- Missing clock. (Touch C35 on 8051 backsize can result this reset)
	- Watch dog timer.
	- Software.
	- Comparator 0.
	- CNVSTR0.

	It can also reset the system by specify one of the following parameters:
	- wdt: Watch dog timer.
	- sw: Software.
	- hw: Hardware.
*/
int	proc_reset(void)
{
	if (g_parm[0] == '\0')
	{
	    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
	    {
			printf("Low Power reset\n");
	    }
	    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
	    {
			printf("Window Watchdog reset\n");
	    }
	    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	    {
			printf("Independent Watchdog reset\n");
	    }
	    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
	    {
	        // This reset is induced by calling the ARM CMSIS
	        // `NVIC_SystemReset()` function!
			printf("Software reset\n");
	    }
	    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
	    {
			printf("POR/PDR reset\n");
	    }
	    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
	    {
			printf("Pin reset\n");
	    }
	    // Needs to come *after* checking the `RCC_FLAG_PORRST` flag in order to
	    // ensure first that the reset cause is NOT a POR/PDR reset. See note
	    // below.
	    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
	    {
			printf("POR/PDR or BOR reset\n");
	    }
	    else
	    {
			printf("Reset source cleared\n");
	    }
	    __HAL_RCC_CLEAR_RESET_FLAGS();
	}
	else if (strcmp(g_parm, "?") == 0)
	{
		printf("rst 	: Show last reset source\n");
		printf("rst wdt : Reset system by watchdog timer\n");
		printf("rst sw	: Reset system by software\n");
//		printf("rst hw	: Reset system by hardware reset\n");
	}
	else if (strcmp(g_parm, "wdt") == 0)
	{
		__disable_irq();
		while (1)
		{}
	}
	else if (strcmp(g_parm, "sw") == 0)
	{
		osDelay(100);
		NVIC_SystemReset();
	}
#if 0
	else if (strcmp(g_parm, "hw") == 0)
	{
		osDelay(100);
	}
#endif
	else
		return parm_error(g_parm);

	return E_OK;
}

/** \brief	Clear screen.

	Clear screen using ANSI control code.
*/
int	clear_screen(void)
{
	ansi_clearScreen();

	return E_OK;
}

/** \brief	Show ANSI color.

	Show ANSI colors.
*/
int	show_color(void)
{
	for (int color=0; color<16; color++)
	{
		int bgColor = _GRAY;
		if (color == _GRAY || color == _WHITE)
			bgColor = _BLACK;
		ansi_setColor(color, bgColor);
		printf("This is color %d\n", color);
	}

	return E_OK;
}

/** \brief	Halt system.

	Halt system and wait for reset button.
*/
int	halt_system(void)
{
	printf("Set console task to an infinite loop.\n");
	while(1)
	{}

	return E_OK;
}

/** \brief  Show byte as binary bits.

    \param[in]	byte	Data to shown.
*/
void	show_binary(uint8_t byte)
{
	static uint8_t	i;

	for (i=0; i<8; i++)
	{
		putchar((byte & 0x80) ? '1' : '0');
		byte <<= 1;
		if (i == 3)
			putchar(' ');
	}
}
