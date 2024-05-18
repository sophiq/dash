#include "Display.h"
#include "SystemRunning.h"
#include "can.h"
#include <math.h>

void DisplayAnimation(void)
{
#if !ENABLE_OPENANIMATION
	thissystem.IsOpenAnimationFinished = 1;
#else
	static uint8_t u8OpenAnimationStep = 0;

	if(thissystem.IsOpenAnimationFinished){
		return;
	}
	
	/* CLEAR ALL FLAGS */
	lcd_display_clear_all_flags();
	
	/* DISPLAY ALL VALUES */
	lcd_display_animation(u8OpenAnimationStep);
	switch(u8OpenAnimationStep++){
		case 0:
			/* DISPLAY ALL SYMBOLS */
			lcd_display_show_all_symbols();
			break;
		case 1:
			_part.flags.HighBeamFlag			= 1;			  /* #1 - Զ��� */
		  _part.flags.HandleBarFaultFlag		= 1;		/* #1 - ת�ѹ��� */
		
			break;
		case 2:
			_part.flags.BlueTooth = 1;                /* #2 ���� */
			
		  _part.flags.MotorFaultFlag			= 1;			/* #2- ������� */

			break;
		case 3:
			_part.flags.WidthLampFlag			= 1;			  /* #3 - ʾ��� */
			_part.flags.BlueTooth = 0;
		  _part.flags.SideshoringOffFlag		= 1;	  /* #3- ���Ŷϵ繦��״̬ */

			break;
		case 4:
			_part.flags.TurnLeftFlag			= 1;			  /* #4 - ��ת��� */
			
			_part.flags.ControllerFaultFlag		= 1;		/* #4 - ���������� */
			break;
		case 5:
			_part.flags.SysRunFlag				= 1;			  /* #5 - ��Դ״̬�� */
			
			_part.flags.TirePressure = 1;							/* #5 - ̥ѹ���� */
			_part.flags.BrakeFaultFlag			= 1;			/* #5 - ɲ�ѹ��� */
			break;
		case 6:
			_part.flags.TurnRightFlag			= 1;			  /* #6 - ��ת��� */
			_part.flags.CruiseRunFalg			= 1;			  /* #6 - Ѳ��ָʾ�� */
		  _part.flags.TirePressure = 0;
			break;
		case 7:
			_part.flags.LowBeamFlag				= 1;			  /* #7 - ����� */
			_part.flags.TCS			= 1;									/* #7 - TCS */
			break;
		case 8:	
			_part.flags.UnderVolWarningFlag		= 1;		/* #8 - Ƿѹ�� */
		  _part.flags.charge			= 1;			        /* #8 - ��� */
		  _part.flags.TCS			= 0;
		
			break;
		case 9:
			_part.flags.ReverseShiftFlag	= 1;			  /* #9 - ����ͼ��1 */
			_part.flags.charge			= 0;
			break;
		case 10:
			u8OpenAnimationStep		= 0;
			thissystem.IsOpenAnimationFinished	= 1;
			thissystem.uart_nolink_sec = 0;
		  thissystem.c_a_onoff_flag = 1;
		  thissystem.s_biankuang_a_flag = 1;
		  mcu_die_flag = 0;
		  bms_die_flag = 0;
			break;
		default:
			break;
	}
#endif
}

void DisplaySystemInformation(void)
{
	static uint8_t step = 0;
		
	double y;
	
	if(!thissystem.IsOpenAnimationFinished){
		return;
	}
	switch(step++){
		case 0:
			/* CLEAR ALL FLAGS */
			lcd_display_clear_all_flags();
			/* DISPLAY ALL SYMBOLS */
			_part.tempture.c_onoff_flag			= 0;			/* #18- �¶���Ϣ */
			_part.tempture.number				= 0.00;			/* #18- �¶���Ϣ */
			_part.shift.c_onoff_flag			= 1;			/* #19- ��λ��Ϣ */
			_part.shift.number					= 0;			/* #19- ��λ��Ϣ */
			_part.time.c_onoff_flag				= 1;			/* #20- ʱ����Ϣ */
			_part.time.hour						= 0;			/* #20- ʱ����Ϣ */
			_part.time.min						= 0;
			_part.time.sec						= 0;
			_part.time.s_colon_flag				= 1;
			_part.time.s_am_flag				= 0;
			_part.time.s_pm_flag				= 0;
			_part.speed.c_onoff_flag			= 1;			/* #21- �ٶ���Ϣ */
			_part.speed.c_onoff_number_flag		= 1;			/* #21- �ٶ���Ϣ */
			_part.speed.number					= 0;			/* #21- �ٶ���Ϣ */
			_part.speed.s_mph_flag				= 0;
			_part.speed.s_kmh_flag				= 0;
			_part.bat_a.id						= E_BATTERY_A;	/* #22- �����Ϣ */
			_part.bat_a.percent					= 0;			/* #22- �����Ϣ */
			_part.bat_a.s_e_flag				= 0;
			_part.bat_a.s_f_flag				= 0;
			_part.bat_a.s_icon_flag				= 0;
			_part.bat_a.c_onoff_flag			= 1;
			_part.bat_b.id						= E_BATTERY_B;
			_part.bat_b.percent					= 0;
			_part.bat_b.s_e_flag				= 0;
			_part.bat_b.s_f_flag				= 0;
			_part.bat_b.s_icon_flag				= 0;
			_part.bat_b.c_onoff_flag			= 0;
			_part.odo_odo.id					= E_ODOGRAPH_ODO;/* #23- �����Ϣ */
			_part.odo_odo.number				= 0.00;			 /* #23- �����Ϣ */
			_part.odo_odo.s_odo_flag			= 1;
			_part.odo_odo.s_trip_flag			= 0;
			_part.odo_odo.s_miles_flag			= 0;
			_part.odo_odo.s_km_flag				= 0;
			_part.odo_odo.s_a_flag				= 0;
			_part.odo_odo.s_b_flag				= 0;
			_part.odo_odo.c_onoff_flag			= 1;
			_part.odo_trip.id					= E_ODOGRAPH_TRIP;
			_part.odo_trip.number				= 0.00;
			_part.odo_trip.s_odo_flag			= 0;
			_part.odo_trip.s_trip_flag			= 1;
			_part.odo_trip.s_miles_flag			= 0;
			_part.odo_trip.s_km_flag			= 0;
			_part.odo_trip.s_a_flag				= 0;
			_part.odo_trip.s_b_flag				= 0;
			_part.odo_trip.c_onoff_flag			= 1;
			break;
		case 1:
			
			if(mcu_die_flag){
				thissystem.UnderVolWarningFlag = 0;
				thissystem.ControllerFaultFlag = 0;
				thissystem.HandleBarFaultFlag = 0;
				thissystem.SideshoringOffFlag = 0;
				thissystem.MotorFaultFlag = 0;				
				thissystem.BrakeFaultFlag = 0;
				thissystem.CruiseRunFalg = 0;
				thissystem.s_icona_flag = 0;
				thissystem.SysRunFlag = 0;
				thissystem.charge = 0;
				thissystem.TirePressure = 0;
				thissystem.TCS = 0;
				thissystem.BlueTooth = 0;
			}		
		
			/* #1 - ��ת��� */
			_part.flags.TurnLeftFlag			= thissystem.TurnLeftFlag;
			/* #2 - Զ��� */
			_part.flags.HighBeamFlag			= thissystem.HighBeamFlag;
			/* #3 - ʾ��� */
			_part.flags.WidthLampFlag			= thissystem.WidthLampFlag;		
//			_part.flags.WidthLampFlag			= 1;//system.WidthLampFlag;
			/* #4 - ��Դ״̬�� */
			_part.flags.SysRunFlag				= thissystem.SysRunFlag;
			/* #5 - ����� */
			_part.flags.LowBeamFlag				= thissystem.LowBeamFlag;
			/* #6 - Ƿѹ�� */
			_part.flags.UnderVolWarningFlag		= thissystem.UnderVolWarningFlag;
			/* #7 - ��ת��� */
			_part.flags.TurnRightFlag			= thissystem.TurnRightFlag;
			/* #8 - Ѳ��ָʾ�� */
			_part.flags.CruiseRunFalg			= thissystem.CruiseRunFalg;
			/* #9 - ɲ�ѹ��� */
			_part.flags.BrakeFaultFlag			= thissystem.BrakeFaultFlag;
			/* #10- ���������� */
			_part.flags.ControllerFaultFlag		= thissystem.ControllerFaultFlag;
			/* #11- ���Ŷϵ繦��״̬ */
			_part.flags.SideshoringOffFlag		= thissystem.SideshoringOffFlag;
			/* #12- ������� */
			_part.flags.MotorFaultFlag			= thissystem.MotorFaultFlag;
			/* #13- ת�ѹ��� */
			_part.flags.HandleBarFaultFlag		= thissystem.HandleBarFaultFlag;
			/* #14- ����״̬ */
			_part.flags.ReverseShiftFlag		= thissystem.ReverseShiftFlag;
			/* #15- UNKNOWN1 */
			_part.flags.Unknown1Flag			= thissystem.Unknown1Flag;
			/* #16- UNKNOWN2 */
			_part.flags.Unknown2Flag			= thissystem.Unknown2Flag;
			/* #17- UNKNOWN3 */
			_part.flags.charge			= thissystem.charge;
			_part.flags.TirePressure = thissystem.TirePressure;
			_part.flags.BlueTooth = thissystem.BlueTooth;
			_part.flags.TCS = thissystem.TCS;
			
			
			/* #18- �¶���Ϣ */
			_part.tempture.number				= thissystem.tempture;
			/* #19- ��λ��Ϣ */
			_part.shift.number					=   thissystem.shift;
			/* #20- ʱ����Ϣ */
			_part.time.hour						= thissystem.hour;
			_part.time.min						= thissystem.min;
			_part.time.sec						= thissystem.sec;
			_part.time.s_colon_flag				= thissystem.s_colon_flag;
			/* #21- �ٶ���Ϣ */
			_part.speed.c_onoff_number_flag		= thissystem.c_onoff_number_flag;

			if(thissystem.IsUseMPH){
				thissystem.speed = (uint8_t)(KMH2MPH((float)speed));
				if((modf((double)KMH2MPH((float)speed)/10,&y))>0.5)thissystem.speed += 1;//��������
			}
			else{
			  thissystem.speed = speed;
			}
			_part.speed.number					= thissystem.speed;
			_part.speed.s_mph_flag				= thissystem.IsUseMPH;
			_part.speed.s_kmh_flag				= !thissystem.IsUseMPH;
			/* #22- �����Ϣ */
			_part.bat_a.c_onoff_flag			= thissystem.c_a_onoff_flag;
			_part.bat_a.percent					= thissystem.battery_a;
			_part.bat_a.s_biankuang_flag		= thissystem.s_biankuang_a_flag;
			_part.bat_a.s_e_flag				= thissystem.s_e_a_flag;
			_part.bat_a.level					= thissystem.level_a;  
			_part.bat_a.s_icon_flag				= thissystem.s_icona_flag;
			_part.bat_b.c_onoff_flag			= thissystem.c_b_onoff_flag;
			_part.bat_b.percent					= thissystem.battery_b;
			_part.bat_b.s_biankuang_flag		= thissystem.s_biankuang_b_flag;
			_part.bat_b.s_e_flag				= thissystem.s_e_b_flag;
			_part.bat_b.level					= thissystem.level_b; 
			_part.bat_b.s_icon_flag				= thissystem.s_iconb_flag;
			
			/* #23- �����Ϣ */
			_part.odo_odo.number				= thissystem.odo_number;
			_part.odo_odo.s_miles_flag			= thissystem.IsUseMPH;
			_part.odo_odo.s_km_flag				= !thissystem.IsUseMPH;
			_part.odo_trip.number				= thissystem.trip_number;			
			
			
		
			_part.odo_trip.s_a_flag				= thissystem.s_a_flag;
			_part.odo_trip.s_b_flag				= thissystem.s_b_flag;
			_part.odo_trip.s_miles_flag			= thissystem.IsUseMPH;
			_part.odo_trip.s_km_flag			= !thissystem.IsUseMPH;
			

			
			
			
			
			break;
		default:
			step = 1;
			break;
	}
	
	
	
	
	/*=================================================================*/

}
