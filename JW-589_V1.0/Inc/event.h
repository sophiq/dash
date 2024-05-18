#ifndef __EVENT_H__
#define __EVENT_H__

#include "fm33lg0xx.h"
#include "main.h"

void PushEvent(uint8_t eid);
uint8_t PopEvent(void);

#endif
