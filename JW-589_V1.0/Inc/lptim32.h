#ifndef __LPTIM32_H__
#define __LPTIM32_H__
#include "user_init.h"

typedef void (*callback)(void);

typedef struct{
	uint32_t init;
	uint32_t preset;
	callback task;
}timer_t;

extern uint8_t pwron5s_cnt;

void LPTIM32_Setup(void);

#endif
