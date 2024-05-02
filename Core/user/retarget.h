
#include <stdio.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void RetargetInit(UART_HandleTypeDef *huart);

#if USE_READ_WRITE

// If USE_READ_WRITE is defined, use _write and _read functions
int _write(int fd, char* ptr, int len);
int _read(int fd, char* ptr, int len);

#else

// If USE_READ_WRITE is not defined, use __io_putchar and __io_getchar functions
int __io_putchar(int ch);
int __io_getchar(void);

void RetargetInit(UART_HandleTypeDef *huart);

#if USE_READ_ERITE

int _write(int fd, char* ptr, int len);
int _read(int fd, char* ptr, int len);

#else

int __io_putchar(int ch);
int __io_getchar(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
