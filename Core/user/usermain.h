#ifndef USERMAIN_H_
#define USERMAIN_H_

#include "target.h"
#include "retarget.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define UART_TIMEOUT_MS 1000

typedef struct {
    char student_id[20];
    char sbc_model[20];
    char sbc_serial[20];
    char uuid[37];
    char vector_table[20];
} STLinkData;

#ifdef __cplusplus
extern "C" {
#endif

void usermain();

#ifdef __cplusplus
}
#endif

#endif
