#include "bsp_key.h"
#include "lcd.h"
#include "SystemRunning.h"
#include "config.h"
#include "Display.h"
#include "event.h"
#include "gptim.h"
#include "can.h"
#include "idle.h"
#include "eeprom.h"

#define MAX_VOL 1817	// 48.0(V)
#define MIN_VOL 1588	// 42.0(V)
uint8_t metric_english = METRIC;

typedef struct{
	/*---------------------------*/// byte1
	uint8_t	dev_id				:8;
	/*---------------------------*/// byte2
	uint8_t	seq_id_l8			:8;
	/*---------------------------*/// byte3
	uint8_t	voice_15kmh			:1;
	uint8_t	ParkingShiftFlag	:1;
	uint8_t						:1;
	uint8_t	SideshoringOffFlag	:1;
	uint8_t	seq_id_h4			:4;
	/*---------------------------*/// byte4
	uint8_t						:1;
	uint8_t						:1;
	uint8_t	CruiseRunFalg		:1;
	uint8_t	undervol_error		:1;
	uint8_t	ControllerFaultFlag	:1;
	uint8_t	HandleBarFaultFlag	:1;
	uint8_t	MotorFaultFlag		:1;
	uint8_t	varfreq_flag		:1;
	/*---------------------------*/// byte5
	uint8_t	speedmode_b10		:2;
	uint8_t						:1;
	uint8_t						:1;
	uint8_t						:1;
	uint8_t	BrakeFaultFlag		:1;
	uint8_t						:1;
	uint8_t	speedmode_b2		:1;
	/*---------------------------*/// byte6
	uint8_t						:1;
	uint8_t	BrakeRunFlag		:1;
	uint8_t	ReverseShiftFlag	:1;
	uint8_t						:1;
	uint8_t						:1;
	uint8_t						:1;
	uint8_t						:1;
	uint8_t						:1;
	/*---------------------------*/// byte7
	uint8_t	time_hour			:8;
	/*---------------------------*/// byte8
	uint8_t	speed_h8			:8;
	/*---------------------------*/// byte9
	uint8_t	speed_l8			:8;
	/*---------------------------*/// byte10
	uint8_t	voltage_per			:8;
	/*---------------------------*/// byte11
	uint8_t	time_min			:8;
	/*---------------------------*/// byte12
	uint8_t	crc_xor				:8;
	/*---------------------------*/
}common1wire_t;


typedef struct{
	/*---------------------------*/// byte1
	uint8_t	dev_id				:8;
	
	/*---------------------------*/// byte2
	uint8_t	seq_id_l8			:8;
	
	/*---------------------------*/// byte3
	uint8_t turnhandleron		:1;/* 转把是否开启	*/
	uint8_t nolink				:1;/* 控制器无通讯	*/
	uint8_t locked				:1;/* 防盗锁上锁中	*/
	uint8_t ParkingShiftFlag	:1;/* 驻车状态		*/
	uint8_t head_reserved		:4;/* 流水号		*/
	
	/*---------------------------*/// byte4
	uint8_t MotorFaultFlag		:1;/* 电机故障		*/
	uint8_t SideshoringOffFlag	:1;/* 助力			*/
	uint8_t CruiseRunFalg		:1;/* 巡航			*/
	uint8_t UnderVolWarningFlag	:1;/* 欠压			*/
	uint8_t ControllerFaultFlag	:1;/* 控制器故障	*/
	uint8_t HandleBarFaultFlag	:1;/* 转把故障		*/
	uint8_t hallerror			:1;/* 霍尔故障		*/
	uint8_t Cruise6kmRunFalg	:1;/* 巡航-6km		*/

	/*---------------------------*/// byte5
	uint8_t speedmode_b10		:2;/* 速度档位01	*/
	uint8_t runaway_protect		:1;// 防飞车保护	(Run Away Protection)
	uint8_t charging			:1;/* 滑行充电		*/
	uint8_t ctrlprotect			:1;/* 控制器保护	*/
	uint8_t BrakeFaultFlag		:1;/* 刹车			*/
	uint8_t motorrun			:1;/* 电机运行中	*/
	uint8_t speedmode_b2		:1;/* 速度档位2		*/

	/*---------------------------*/// byte6
	uint8_t restrict_speed		:1;/* 限速			*/
	uint8_t BrakeRunFlag		:1;/* 电子刹车		*/
	uint8_t ReverseShiftFlag	:1;/* 倒车			*/
	uint8_t msl_protect			:1;// 堵转保护		(Motor Shaft Lock Protection)
	uint8_t oc_protect			:1;// 过流保护		(Over Current Protection)
	uint8_t eps_on				:1;// 启用备用电源	(Emergency Power Supply)
	uint8_t onekey				:1;/* 启用一键通	*/
	uint8_t current70per		:1;/* 70%电流标志	*/
	
	/*---------------------------*/// byte7
	uint8_t current				:8;/* 运行电流		*/
	
	/*---------------------------*/// byte8
	uint8_t speed_h8			:8;/* 运行速度-高位	*/
	
	/*---------------------------*/// byte9
	uint8_t speed_l8			:8;/* 运行速度-低位	*/
	
	/*---------------------------*/// byte10
	uint8_t voltage_pre			:8;/* 电压比例	*/
	
	/*---------------------------*/// byte11
	uint8_t current_pre			:8;/* 电流比例	*/
	
	/*---------------------------*/// byte12
	uint8_t hall_h				:8;/* 速度霍尔-高位	*/
	
	/*---------------------------*/// byte13
	uint8_t hall_l				:8;/* 速度霍尔-低位	*/
	
	/*---------------------------*/// byte14
	uint8_t reserved1			:1;/* 备用			*/
	uint8_t reserved2			:1;/* 备用			*/
	uint8_t mode_shushi			:1;/* 舒适模式		*/
	uint8_t mode_sport			:1;/* 运动模式		*/
	uint8_t mode_chaoche		:1;/* 超车模式		*/
	uint8_t HighBeamFlag		:1;/* 大灯			*/
	uint8_t TurnRightFlag		:1;/* 右转向		*/
	uint8_t TurnLeftFlag		:1;/* 左转向		*/
	
	/*---------------------------*/// byte15
	uint8_t	crc_xor				:8;
	/*---------------------------*/
}jw_1wire_t;



system_t thissystem;




/*!<
 *---------------------------------------------------------------
 * @name	task_io_refresh
 * @period	25ms
 *----------------------------------------------------------------
 */
void task_io_refresh(uint8_t keynum)
{
//	static uint8_t pre_key = 0;
	static uint8_t keycnt = 0;
	static uint8_t havelong = 0;
	if(GetBit(keynum,KEY_PRESS)){
		keycnt ++;
		if(keycnt>50){
			keycnt = 50;
			if(havelong == 0){
				trip = 0;
				thissystem.trip_number_km = 0;
				havelong = 1;
			}
		}
	}else{
		if((keycnt!=0)&&(keycnt<50)){
//printf("switch \n");			
			thissystem.IsUseMPH = ~thissystem.IsUseMPH;
			Write();
		}
		havelong = 0;
		keycnt = 0;
		
	}
	thissystem.shift = thisshift;
//	uint8_t cur_level = !_IOREAD_HIGHBEAM();
//	
//	if(cur_level < pre_level){
//		switch_highbean_count++;
//	}
//	
//	pre_level = cur_level;
	
	
	/*=================================================================*/
	/*=================================================================*/
	/* #1 - 左转向灯 */
	thissystem.TurnLeftFlag = GetBit(keynum,KEY_LEFT);
	/* #2 - 远光灯 */
	thissystem.HighBeamFlag = GetBit(keynum,KEY_FARLIGHT);
//	printf("LIGHT:%d  %d\r\n",thissystem.HighBeamFlag,light);	
	if(thissystem.HighBeamFlag == 1){if(light>BRIGHT_MIN){light -=25;LedSet(light);lightlevel = 0;}}
	else                            {if(light<BRIGHT_MAX){light +=25;LedSet(light);lightlevel = 3;}}
	
	/* #3 - 示宽灯 */
	thissystem.WidthLampFlag = GetBit(keynum,KEY_POSITION);	
	
	/* #4 - 电源状态灯 */
	if(thisshift == 0)thissystem.ParkingShiftFlag = 1;
	else              thissystem.ParkingShiftFlag = 0;
	
//	if(thissystem.ParkingShiftFlag==1){
//	thissystem.SysRunFlag = 0;
//	}else{
//	thissystem.SysRunFlag = (thissystem.speedkmh == 0 && (!thissystem.BrakeFaultFlag && !thissystem.MotorFaultFlag && !thissystem.ControllerFaultFlag && !thissystem.HandleBarFaultFlag));
//	}
	/* #5 - 近光灯 */
	/* #6 - 欠压灯 */
	/* #7 - 右转向灯 */
	thissystem.TurnRightFlag = GetBit(keynum,KEY_RIGHT);
	/* #8 - 巡航指示灯 */
	/* #9 - 刹把故障 */
	/* #10- 控制器故障 */
	/* #11- 单撑断电功能状态 */
	/* #12- 电机故障 */
	/* #13- 转把故障 */
	/* #14- 倒挡状态 */
	/* #15- UNKNOWN1 */
	/* #16- UNKNOWN2 */
	/* #17- UNKNOWN3 */
	/*=================================================================*/
	/* #18- 温度信息 */
	/* #19- 档位信息 */
	/* #20- 时间信息 */
	/* #21- 速度信息 */
	/* #22- 电池信息 */
	/* #23- 里程信息 */
	/*=================================================================*/
	/* 业务需求 */
	thissystem.BackLightPWMDuty = thissystem.WidthLampFlag ? 30 : 100;
}

/*!<
 *---------------------------------------------------------------
 * @name	task_time_flashing
 * @period	500ms
 *----------------------------------------------------------------
 */
void task_time_flashing(void)
{
	thissystem.s_colon_flag = ~thissystem.s_colon_flag;
}

/*!<
 *---------------------------------------------------------------
 * @name	task_time_counting
 * @period	1s
 *----------------------------------------------------------------
 */
void task_time_counting(void)
{
	if(thissystem.sec < 59){
		thissystem.sec++;
	}
	else{
		thissystem.sec = 0;
		if(thissystem.min < 59){
			thissystem.min++;
		}
		else{
			thissystem.min = 0;
			if(thissystem.hour < 23){
				thissystem.hour++;
			}
			else{
				thissystem.hour = 0;
			}
		}
	}
}

/*!<
 *---------------------------------------------------------------
 * @name	task_odograph_counting
 * @desc	store the odo number when 1km arrived
 * @period	1s   
 *----------------------------------------------------------------
 */
void task_odograph_counting(void)
{
	static float pre_trip = 0.0;
	static float pre_odo_number_km = 0;
	float thistrip;
	
	if(thissystem.odo_number_km == 0)pre_odo_number_km = 0.0;	
	thistrip = (float)trip / 10;
  if(thistrip > pre_trip){
    thissystem.trip_number_km += thistrip - pre_trip;
		pre_trip = thistrip;
	}
	
	/*!< odo/trip displaying... */
	if(thissystem.IsUseMPH){
		thissystem.odo_number = KMH2MPH(thissystem.odo_number_km);
		thissystem.trip_number = KMH2MPH(thissystem.trip_number_km);
	}
	else{
		thissystem.odo_number = thissystem.odo_number_km;
		thissystem.trip_number = thissystem.trip_number_km;
	}
//printf("%3.1f  %3.1f  %3.1f \r\n",thistrip,thissystem.trip_number_km,thissystem.trip_number);	
}

/*!<
 *---------------------------------------------------------------
 * @name	task_system_store
 * @desc	store the odo number when 1km arrived
 * @period	realtime
 *----------------------------------------------------------------
 */

static void Do25ms(uint8_t key)
{
	DisplaySystemInformation();
	task_io_refresh(key);
}

static void Do50ms(void)
{
//	KeyProcessTask();
}
void KeyPower(void)
{
	thissystem.trip_number_km = 0;
}
static void Do100ms(void)
{
//	task_odograph_counting();
}

static void Do250ms(void)
{
	static uint8_t thisflash = 0;		
	/*!< 开机动画 */
	DisplayAnimation();
	
	/* 充电闪烁 */
	/* 开机动画中直接退出 */
	if(!thissystem.IsOpenAnimationFinished){
		return;
	}
	switch(thischarge){
		case 0: thissystem.charge = 0;
						thisflash = 0;
			break;
		case 1: thissystem.charge = ~thissystem.charge;
						thisflash = 0;
			break;
		case 2: thisflash ++;
						if(thisflash == 4){
							thisflash = 0;
							thissystem.charge = ~thissystem.charge;
						}
			break;		
		case 3: thissystem.charge = 1;
						thisflash = 0;
			break;
		default:thissystem.charge = 0;
						thisflash = 0;
			break;
	}	
//	bsp_pwm_setduty(thissystem.BackLightPWMDuty);
}


static uint8_t BatteryGetLevelByPercent(uint8_t percent)
{
	uint8_t level = 0;
	
	if(percent ==0){
		level = 0;
	}
	else if(percent == 100){
		level = 10;
	}
	else{
		level = percent / 10 + 1;
	}
	
	return level;
}

//static void SystemTimeCount1Second(void)
//{
//	if(thissystem.sw_nolink_sec < 255){
//		thissystem.sw_nolink_sec++;
//	}
//	
//	if(thissystem.sw_nolink_sec > 5){
//		thissystem.sw_nolink = 1;
//	}
//}


static void Do500ms(void)
{

//	static uint8_t batteryb = 0; 
//	static uint8_t batteryab = 0;
	
	/* thissystem time colon flashing */
	task_time_flashing();
	if(undervolflag)thissystem.UnderVolWarningFlag = ~thissystem.UnderVolWarningFlag;
	else            thissystem.UnderVolWarningFlag = 0;
	thissystem.level_a = BatteryGetLevelByPercent(thissystem.battery_a);
	thissystem.level_b = BatteryGetLevelByPercent(thissystem.battery_b);
	thissystem.c_onoff_number_flag = 1;
	

	
	
}




static void Do1s(void)
{
	/* thissystem time */
//	task_time_counting();
	if(thissystem.hour<12){_part.time.s_am_flag	= 1;_part.time.s_pm_flag	= 0;}
	else									{_part.time.s_am_flag	= 0;_part.time.s_pm_flag	= 1;}
	
	
	/* thissystem odo/trip */
	task_odograph_counting();
	
	
//	SystemTimeCount1Second();
	
  
	
//	if(timer1s < 255){
//		timer1s++;
//	}
	
//	if(timer1s <= 15){
//		if(switch_highbean_count >= 8){
//			if(isreversed == 0){
//				thissystem.storage.IsUseMPH = ~thissystem.storage.IsUseMPH;
//				isreversed = 1;
//			}
//			thissystem.IsUseMPH = thissystem.storage.IsUseMPH;
//			thissystem.IsNeedStoreInfo = 1;
//		}
//	}
}

static void Empty(void)
{
}
	
/*!<*/
void MainLoop(uint8_t key)
{

	/*!<*/
	lcd_display_ready_task();
	
	/*!<*/
	lcd_display_go_task();
	
	/*!<*/
	switch(PopEvent()){
		/*!<========================================*/
		case evEmpty:			  Empty();		break;
		/*!<========================================*/
		case evTime25ms:		Do25ms(key);break;
		case evTime50ms:		Do50ms();		break;
		case evTime100ms:		Do100ms();	break;
		case evTime250ms:		Do250ms();  break;
		case evTime500ms:		Do500ms();	break;
		case evTime1s:			Do1s();			break;
//		case evKeyPower:		KeyPower();	break;
		/*!<========================================*/
	}
}
