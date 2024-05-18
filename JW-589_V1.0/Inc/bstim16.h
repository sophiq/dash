#ifndef __BSTIM16_H__
#define __BSTIM16_H__
#include "fm33lg0xx.h"

extern uint8_t  Bus_Off_Flag;
extern uint8_t  Fast_Recover_Count;
extern uint32_t Recover_Timeout;

void BSTIM16_Init(void);



#endif

