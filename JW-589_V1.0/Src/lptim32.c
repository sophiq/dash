#include "lptim32.h"
#include "user_init.h"
#include "bsp_key.h"
#include "can.h"
#include "LCD00_Y094889VTTFGS.h"
#include "event.h"
#include "config.h"
#include "idle.h"
//LPTIM32中断服务程序   



uint8_t pwron5s_cnt = 0;


static void timer_25ms(void) {	PushEvent(evTime25ms);	}
static void timer_50ms(void) {	PushEvent(evTime50ms);	}
static void timer_100ms(void){	PushEvent(evTime100ms);	}
static void timer_250ms(void){	PushEvent(evTime250ms);	}
static void timer_500ms(void){	PushEvent(evTime500ms);	}
static void timer_1s(void)   {	PushEvent(evTime1s);	  }



static timer_t TIMERS[] = {
	{5,	    5,	timer_25ms},
	{10,	 10,	timer_50ms},
	{20,	 20,	timer_100ms},
	{50,	 50,	timer_250ms},
	{100,	100,	timer_500ms},
	{200,	200,	timer_1s},
};

static void timer_run(void){
	uint8_t timer_index;
	for(timer_index = 0; timer_index < sizeof(TIMERS) / sizeof(timer_t); timer_index++){
		if(TIMERS[timer_index].init > 0){
			TIMERS[timer_index].init--;
		}
		else{
			TIMERS[timer_index].init = TIMERS[timer_index].preset;
			TIMERS[timer_index].task();
		}
	}	
}

void LPTIM_IRQHandler(void)
{
	static uint8_t cnt500ms = 100;
	//定时器溢出时翻转LED
	if(FL_LPTIM32_IsEnabledIT_Update(LPTIM32) && FL_LPTIM32_IsActiveFlag_Update(LPTIM32))
	{
    mcu_die_cnt++;
		if(mcu_die_cnt>100){mcu_die_flag = 1; mcu_die_cnt = 100;}
    vcu_die_cnt++;
		if(vcu_die_cnt>100){vcu_die_flag = 1; vcu_die_cnt = 100;}		
    bms_die_cnt++;
		if(bms_die_cnt>100){bms_die_flag = 1; bms_die_cnt = 100;}		
		bsp_KeyScan10ms();	/* 每隔10ms调用一次此函数*/
    timer_run();
		if(!(--cnt500ms)){
			sendflag = 1;
			cnt500ms = 100;
			pwron5s_cnt++;
		}
		FL_LPTIM32_ClearFlag_Update(LPTIM32);
	}
}

void LPTIM32_Setup(void)
{
    FL_LPTIM32_InitTypeDef timInit;

    /*---------------- 定时器时间基准配置 5ms中断----------------*/
    FL_LPTIM32_StructInit(&timInit);

    timInit.clockSource          = FL_CMU_LPTIM32_CLK_SOURCE_APBCLK;
    timInit.mode                 = FL_LPTIM32_OPERATION_MODE_NORMAL;
    timInit.prescalerClockSource = FL_LPTIM32_CLK_SOURCE_INTERNAL;
    timInit.prescaler            = FL_LPTIM32_PSC_DIV64;
    timInit.autoReload           = 5000 - 1;
    timInit.onePulseMode         = FL_LPTIM32_ONE_PULSE_MODE_CONTINUOUS;
    timInit.triggerEdge          = FL_LPTIM32_ETR_TRIGGER_EDGE_RISING;
    timInit.countEdge            = FL_LPTIM32_ETR_COUNT_EDGE_RISING;
    FL_LPTIM32_Init(LPTIM32, &timInit);

    /*---------------------------------------------*/

    /*---------------- 中断配置 ----------------*/
    /* 清除标志 */
    FL_LPTIM32_ClearFlag_Update(LPTIM32);

    /* 中断使能 */
    FL_LPTIM32_EnableIT_Update(LPTIM32);

    /* 使能并配置NVIC */
    NVIC_DisableIRQ(LPTIMx_IRQn);
    NVIC_SetPriority(LPTIMx_IRQn, 3); //设置中断优先级
    NVIC_EnableIRQ(LPTIMx_IRQn);
    /*---------------------------------------------*/

    /* 使能LPTIM32 */
    FL_LPTIM32_Enable(LPTIM32);
}

