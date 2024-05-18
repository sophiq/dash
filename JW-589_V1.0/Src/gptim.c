#include "user_init.h"
#include "gptim.h"
#include "SystemRunning.h"

uint32_t light = BRIGHT_MAX;
uint8_t  lightlevel = 3;    //没开大灯3档  开大灯0档
void GPTIM_Setup(void)
{
	FL_GPTIM_InitTypeDef        timInit;
	FL_GPTIM_OC_InitTypeDef     timOCInit;
	FL_GPIO_InitTypeDef         gpioInit = {0};

	gpioInit.pin = FL_GPIO_PIN_5;
	gpioInit.mode = FL_GPIO_MODE_DIGITAL;
	gpioInit.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	gpioInit.pull = FL_DISABLE;
	gpioInit.remapPin = FL_ENABLE;
	FL_GPIO_Init(GPIOA, &gpioInit);

	timInit.prescaler             = 80 - 1;                     // 分频系数8000
	timInit.counterMode           = FL_GPTIM_COUNTER_DIR_UP;   // 向上计数
	timInit.autoReload            = 1000 - 1;                          // 自动重装载值1000
	timInit.clockDivision         = FL_GPTIM_CLK_DIVISION_DIV1;  // 死区和滤波分频
	timInit.autoReloadState       = FL_ENABLE;                       // 预装载preload使能
	FL_GPTIM_Init(GPTIM1, &timInit);

	timOCInit.OCMode       = FL_GPTIM_OC_MODE_PWM1;                  // 输出比较模式PWM1
	timOCInit.OCETRFStatus = FL_DISABLE;                                // OC1REF不受ETR影响
	timOCInit.OCFastMode   = FL_DISABLE;                                // 关闭快速使能
	timOCInit.compareValue = 0;                                    // 比较值500
	timOCInit.OCPolarity   = FL_GPTIM_OC_POLARITY_NORMAL;            // OC正常输出
	timOCInit.OCPreload    = FL_DISABLE;                                // OC preload 无效
	FL_GPTIM_OC_Init(GPTIM1, FL_GPTIM_CHANNEL_4, &timOCInit);

	FL_GPTIM_Enable(GPTIM1);    // 使能定时器
}

void LedSet(uint32_t pwm)
{  
	FL_GPTIM_WriteCompareCH4(GPTIM1,pwm);
}
/*
void GPTIM_Setup(void)
{
	FL_GPTIM_InitTypeDef        timInit;
	FL_GPTIM_OC_InitTypeDef     timOCInit;
	FL_GPIO_InitTypeDef         gpioInit = {0};

	gpioInit.pin = FL_GPIO_PIN_1;
	gpioInit.mode = FL_GPIO_MODE_DIGITAL;
	gpioInit.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	gpioInit.pull = FL_DISABLE;
	gpioInit.remapPin = FL_ENABLE;
	FL_GPIO_Init(GPIOD, &gpioInit);

	timInit.prescaler             = 80 - 1;                     // 分频系数8000
	timInit.counterMode           = FL_GPTIM_COUNTER_DIR_UP;   // 向上计数
	timInit.autoReload            = 1000 - 1;                          // 自动重装载值1000
	timInit.clockDivision         = FL_GPTIM_CLK_DIVISION_DIV1;  // 死区和滤波分频
	timInit.autoReloadState       = FL_ENABLE;                       // 预装载preload使能
	FL_GPTIM_Init(GPTIM2, &timInit);

	timOCInit.OCMode       = FL_GPTIM_OC_MODE_PWM1;                  // 输出比较模式PWM1
	timOCInit.OCETRFStatus = FL_DISABLE;                                // OC1REF不受ETR影响
	timOCInit.OCFastMode   = FL_DISABLE;                                // 关闭快速使能
	timOCInit.compareValue = 0;                                    // 比较值500
	timOCInit.OCPolarity   = FL_GPTIM_OC_POLARITY_NORMAL;            // OC正常输出
	timOCInit.OCPreload    = FL_DISABLE;                                // OC preload 无效
	FL_GPTIM_OC_Init(GPTIM2, FL_GPTIM_CHANNEL_2, &timOCInit);

	FL_GPTIM_Enable(GPTIM2);    // 使能定时器
}

void LedSet(uint32_t pwm)
{  
	FL_GPTIM_WriteCompareCH2(GPTIM2,pwm);
}
*/