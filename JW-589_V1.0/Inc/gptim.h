#ifndef __GPTIM_H__
#define __GPTIM_H__

#include <stdint.h>


extern uint32_t light;
extern uint8_t  lightlevel;
void GPTIM_Setup(void);
void LedSet(uint32_t pwm);
#endif
