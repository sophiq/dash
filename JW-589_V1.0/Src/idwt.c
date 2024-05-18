#include "idwt.h"
#include "user_init.h"

void IWDT_Init(void)
{
	FL_IWDT_InitTypeDef    IWDT_InitStruct;

	IWDT_InitStruct.iwdtWindows = 0;
	IWDT_InitStruct.overflowPeriod = FL_IWDT_PERIOD_4000MS;

	FL_IWDT_Init(IWDT, &IWDT_InitStruct);
}
