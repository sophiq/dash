#ifndef __SYSTEM_RUNNING_H__
#define __SYSTEM_RUNNING_H__
#include "fm33lg0xx_fl.h"
#include "main.h"
#include "config.h"

#define MPH2KMH(x)		((x) * 1.6093)
#define KMH2MPH(x)		((x) * 0.6214)
#define BRIGHT_MAX    1000
#define BRIGHT_MIN    300
#define METRIC          0
#define ENGLISH         1

extern system_t thissystem;
void task_io_refresh(uint8_t keynum);
void MainLoop(uint8_t key);

#endif
