#include "main.h"
#include "user_init.h"
#include "lptim32.h"
#include "adc.h"
#include "bsp_key.h"
#include "idle.h"
#include "idwt.h"
#include "atim.h"
#include "bstim16.h"
#include "gptim.h"
#include "can.h"
#include "lcd.h"
#include "LCD00_Y094889VTTFGS.h"
#include "eeprom.h"


speed_t thisspeed;



uint32_t Get122VSample;
uint64_t GetVSample;
uint32_t GetV;
uint8_t  i;

int main(void)
{
	FL_Init();
	FL_DelayMs(20);
	MF_SystemClock_Config();

	
	UserInit();

	
	LPTIM32_Setup();  //5ms
	ATIM_Init();
	bsp_InitKey();
	KEYINPUT_Init();
	IWDT_Init();
	/* BSTIM≥ı ºªØ */
	BSTIM16_Init();	
	Can_Init();
	/* PWM */
	GPTIM_Setup();  
	
	FL_DelayMs(50);
	LedSet(1000);

	while(1)
	{
		FL_IWDT_ReloadCounter(IWDT);    //Œππ∑

//		printf("OK! \r\n");
		Idle();

	}
}


