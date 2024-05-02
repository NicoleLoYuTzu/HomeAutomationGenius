#include "retarget.h"
#include <cerrno>
#include <unistd.h>

// Standard library file descriptor numbers
#define STDOUT_FILENO    1
#define STDERR_FILENO    2

// Pointer to UART handle
UART_HandleTypeDef *g_huart;

// Initialize UART handle
void RetargetInit(UART_HandleTypeDef *huart)
{
    g_huart = huart;
}

// Redirect STDOUT to UART
int _write(int fd, char *ptr, int len)
{
    if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    {
        HAL_StatusTypeDef status = HAL_UART_Transmit(g_huart, (uint8_t*)ptr, len, HAL_MAX_DELAY);
        if (status == HAL_OK)
            return len;
        else
            return EOF;
    }
    errno = EBADF;
    return -1;
}

// Redirect STDIN to UART
int _read(int fd, char *ptr, int len)
{
    if (fd == STDIN_FILENO)
    {
        HAL_StatusTypeDef status = HAL_UART_Receive(g_huart, (uint8_t*)ptr, len, HAL_MAX_DELAY);
        if (status == HAL_OK)
            return len;
        else
            return EOF;
    }
    errno = EBADF;
    return -1;
}

// Placeholder for __io_putchar function if USE_READ_WRITE is not defined
#ifdef USE_READ_WRITE
#else
//int __io_putchar(int ch)
//{
//    HAL_UART_Transmit(g_huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
//    return ch;
//}

// Placeholder for __io_getchar function if USE_READ_WRITE is not defined
int __io_getchar(void)
{
    uint8_t ch;
    HAL_UART_Receive(g_huart, &ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif
