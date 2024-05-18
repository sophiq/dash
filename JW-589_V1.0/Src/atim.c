#include "atim.h"
#include "main.h"

void ATIM_Init(void)
{
	FL_ATIM_InitTypeDef        InitStructer;

	InitStructer.clockSource           = FL_CMU_ATIM_CLK_SOURCE_APBCLK;  //时钟源选择APB2
	InitStructer.prescaler             = 1-1;                                         //分频系数8000
	InitStructer.counterMode           = FL_ATIM_COUNTER_DIR_UP;                    //向上计数
	InitStructer.autoReload            = 2560-1;                                          //自动重装载值1000
	InitStructer.clockDivision         = FL_ATIM_CLK_DIVISION_DIV1;                   //死区和滤波设置
	InitStructer.repetitionCounter     = 1;                                            //重复计数
	InitStructer.autoReloadState       = FL_ENABLE;                                      //自动重装载禁止preload
	FL_ATIM_Init(ATIM, &InitStructer);

	FL_ATIM_SetCounterAlignedMode(ATIM,FL_ATIM_COUNTER_ALIGNED_CENTER_UP);  
	FL_ATIM_SetTriggerOutput(ATIM,FL_ATIM_TRGO_UPDATE);                       //触发源，到DMA-ADC

	FL_ATIM_Enable(ATIM); //定时器使能
	
	FL_ATIM_EnableALLOutput(ATIM);//主控输出使能


}

