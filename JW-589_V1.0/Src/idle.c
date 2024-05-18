#include "idle.h"
#include "user_init.h"
#include "can.h"
#include "adc.h"
#include "bsp_key.h"
#include "gptim.h"
#include "lcd.h"
#include "LCD00_Y094889VTTFGS.h"
#include "SystemRunning.h"
#include "eeprom.h"


void Idle(void)
{
	static uint8_t  statemachine = 0;
	static uint16_t i = 0;
	uint8_t keynum;	
  if(sendflag == 1){
	  sendflag = 0;
		heartbeat ++;
    CAN_SetMsg(&TxMessage);  
    /* ·¢ËÍÊý¾Ý */
    FL_CAN_Frame_Send(FL_CAN_FORMAT_EXTEND_DATA, &TxMessage);
		if(clearodo == 1){
			clearodo = 0;
			Read();
			if(thissystem.odo_number_km != 0){
			  thissystem.odo_number_km = 0;
			  Write();
			}
		}
	}
		if(factorysetting == 1){
			factorysetting = 0;
			thissystem.storage.odo_number_km = 0;
			thissystem.IsUseMPH = 0;
			Write();
			thissystem.odo_number_km = 0;
		}
		if(changeuint == 1){
			changeuint = 0;
			Write();			
		}

    switch(statemachine){
			case START_LCD:
				  LcdConfig();
					SC5037_ClearAllPoints();
					LedSet(light);
  		    statemachine ++;
			    i = 0;
			    SC5037_DisplayAll(1);
				break;
			case FULL_LCD:
					
			    FL_DelayMs(1);
					if(++i == 1000){
						i = 0;					
						SC5037_DisplayAll(0);
					  statemachine ++;
					  lcd_display_show_all_symbols();
						Read();
					}
				break;

			case TASK_LCD:

			    keynum = get_key();		
			    MainLoop(keynum);
				break;
		}
}
