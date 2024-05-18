#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "fm33lg0xx.h"
#include "main.h"
/*！*/
#define ENABLE_OPENANIMATION				1

/*! */
#define BAT_METHOD_UART		0				//[0]: UART
#define BAT_METHOD_1WIRE	1				//[1]: 1-WIRE
#define BAT_METHOD_ADC		2				//[2]: ADC


#define BATTERY_METHOD		BAT_METHOD_UART

enum{
	/*!<====================================*/
	evEmpty,
	/*!<====================================*/
	evTime25ms,
	evTime50ms,
	evTime100ms,
	evTime250ms,
	evTime500ms,
	evTime1s,
	/*!<====================================*/
	evReceived1Wire,
	evReceiveBWS,
	/*!<====================================*/
	evKeyPower,
	/*!<====================================*/
};

typedef struct{
	uint8_t IsUseMPH				:1;	/* 存储当前单位类型	*/
	float   odo_number_km;				/* 存储总计里程(km)	*/
}storage_t;
	
typedef struct{
	struct{
		/*!----------------------------------*/
		// COMMON
		/*!----------------------------------*/
//		uint8_t IsPowerOffVoiceFinished		:1;
//		uint8_t IsPowerOnVoiceFinished		:1;
		uint8_t IsOpenAnimationFinished		:1;
		uint8_t IsColonFlashingFlag			:1;
		uint8_t IsUseMPH					:1;
		uint8_t IsTripFlag					:1;
		uint8_t IsRequiredStore				:1;
//		uint8_t SysRunFlag					:1;
//		uint8_t UartReceivedFlag			:1;
		uint8_t UartLostLinkFlag			:1;
//		uint8_t SingWireReceivedFlag		:1;
		uint8_t SingleWireLostLinkFlag		:1;
		/*!----------------------------------*/
		// instrument
		/*!----------------------------------*/
//		uint8_t Unkown1Flag					:1;
//		uint8_t FaultFlag					:1;
		uint8_t ReadyFlag					:1;
//		uint8_t EmergencyFlag				:1;
//		uint8_t Unkown2Flag					:1;
//		uint8_t AutoFlag					:1;
//		uint8_t OtaFlag						:1;
//		uint8_t WarningFlag					:1;
//		uint8_t BoostFlag					:1;
//		uint8_t RangeFlag					:1;
		/*!----------------------------------*/
		// Lamp Flashers
		/*!----------------------------------*/
//		uint8_t TurnLeftFlasherEnable		:1;
//		uint8_t TurnRightFlasherEnable		:1;
//		uint8_t EmergencyFlasherEnable		:1;
		/*!----------------------------------*/
		// Lamp Protect
		/*!----------------------------------*/
//		uint8_t ProtectTurnLeftFlag			:1;
//		uint8_t ProtectTurnRightFlag		:1;
//		uint8_t ProtectBrakeFlag			:1;
//		uint8_t ProtectHighBeamFlag			:1;
//		uint8_t ProtectLowBeamFlag			:1;
//		uint8_t ProtectWidthLampFlag		:1;
		/*!----------------------------------*/
		// other
		/*!----------------------------------*/
//		uint8_t LightSensorFlag1			:1;
//		uint8_t LightSensorFlag2			:1;
		uint8_t BatteryOnOffFlag			:1;
		uint8_t BatteryOnOffFlag2			:1;
		/*!----------------------------------*/
		/// fault
		/*!----------------------------------*/
		uint8_t ControllerFaultFlag			:1;
		uint8_t MotorFaultFlag				:1;
		uint8_t BrakeFaultFlag				:1;
		uint8_t HandleBarFaultFlag			:1;
		/*!----------------------------------*/
		/// signal
		/*!----------------------------------*/
		uint8_t LowBeamFlag					:1;
		uint8_t WidthLampFlag				:1;
		uint8_t HighBeamFlag				:1;
		uint8_t TurnLeftFlag				:1;
		uint8_t TurnRightFlag				:1;
		uint8_t EmergencyFlag				:1;
		uint8_t ParkingShiftFlag			:1;
		uint8_t ReverseShiftFlag			:1;
		uint8_t ChargeRunFlag				:1;
		uint8_t SideShoringOffFlag			:1;
		uint8_t BrakeRunFlag				:1;
		uint8_t CruiseRunFlag				:1;
		/*!----------------------------------*/
		/// warning
		/*!----------------------------------*/
		uint8_t UnderVolWarningFlag			:1;
		uint8_t BatteryWarningFlag			:1;
		uint8_t BatteryEmptyFlag			:1;
		uint8_t BatteryEmptyFlag2			:1;
		/*!----------------------------------*/
	}flags;
	/*------------------------------------------------------*/
	uint8_t bIsKeyPressedFlag		:1;	/* 按键是否被按下	*/
	uint8_t IsOpenAnimationFinished	:1;	/* 开机动画是否完成 */
	uint8_t IsUseMPH				:1;	/* 是否使用MPH单位  */
	uint8_t IsNeedReverseMPH		:1;	/* 是否切换公英里制 */
	uint8_t IsNeedStoreInfo			:1;	/* 是否需要保存信息 */
	uint8_t BackLightPWMDuty;			/* 背光占空比		*/
	storage_t storage;					/* EEPROM存放数据	*/
	/*------------------------------------------------------*/
	float 	tempture;					/* 系统温度			*/
	uint8_t shift					:4;	/* 档位信息			*/
	uint8_t s_colon_flag			:1;	/* 时间分隔符闪烁	*/
	uint8_t hour;						/* 时				*/
	uint8_t min;						/* 分				*/
	uint8_t sec;						/* 秒				*/
	uint8_t c_onoff_number_flag		:1;	/* 速度数字是否显示	*/
	uint8_t speed;						/* 速度信息			*/
	uint8_t speedkmh;					/* 速度信息(kmh)	*/
	uint8_t s_biankuang_a_flag		:1;	/* 电量 A边框闪烁	*/
	uint8_t s_biankuang_b_flag		:1;	/* 电量 B边框闪烁	*/
	uint8_t s_e_a_flag				:1;	/* 电量 A[e]闪烁	*/
	uint8_t s_e_b_flag				:1;	/* 电量 B[e]闪烁	*/
	uint8_t c_a_onoff_flag			:1;	/* 电量 A是否显示	*/
	uint8_t c_b_onoff_flag			:1;	/* 电量 B是否显示	*/
	uint8_t battery_a;					/* 电量 A			*/
	uint8_t level_a;					/* 电量 A 等级		*/ 
	uint8_t s_icona_flag			:1;
	uint8_t battery_b;					/* 电量 B			*/
	uint8_t level_b;					/* 电量 B 等级		*/ 
	uint8_t s_iconb_flag			:1;
	float 	odo_number;					/* 总计里程			*/
	float 	odo_number_km;				/* 总计里程(khm)	*/
	float 	trip_number;				/* 单次里程			*/
	float	trip_number_km;				/* 单次里程(kmh)	*/
	uint8_t	s_a_flag				:1;	/* 单次里程A是否显示*/
	uint8_t	s_b_flag				:1;	/* 单次里程A是否显示*/
	/*------------------------------------------------------*/
	uint8_t SysRunFlag				:1;	/* 电源上电状态		*/
	uint8_t CruiseRunFalg			:1;	/* 巡航开启状态		*/
	uint8_t ReverseShiftFlag		:1;	/* R-档开启状态		*/
	uint8_t SideshoringOffFlag		:1;	/* 边撑断电功能		*/
	uint8_t TurnLeftFlag			:1;	/* 左转向灯			*/
	uint8_t TurnRightFlag			:1;	/* 右转向灯			*/
	uint8_t LowBeamFlag				:1;	/* 近光灯			*/
	uint8_t HighBeamFlag			:1;	/* 远光灯			*/
	uint8_t WidthLampFlag			:1;	/* 示宽灯			*/
	uint8_t UnderVolWarningFlag		:1;	/* 欠压故障状态		*/
	uint8_t BrakeFaultFlag			:1;	/* 刹把故障状态		*/
	uint8_t MotorFaultFlag			:1;	/* 电机故障状态		*/
	uint8_t ControllerFaultFlag		:1;	/* 中控故障状态		*/
	uint8_t HandleBarFaultFlag		:1;	/* 转把故障状态		*/
	uint8_t Unknown1Flag			:1;	/* 未知图标1		*/
	uint8_t Unknown2Flag			:1;	/* 未知图标2		*/
	uint8_t charge			:1;	/* 未知图标3		*/
	uint8_t TirePressure :1;
	uint8_t TCS :1;
	uint8_t BlueTooth :1;
	/*------------------------------------------------------*/
	uint8_t ParkingShiftFlag		:1;	/* P-档开启状态		*/
	uint8_t BrakeRunFlag			:1;	/* 刹车状态			*/
	/*------------------------------------------------------*/
	uint8_t uart_nolink				:1;	/* 电量通讯失联		*/
	uint8_t uart_nolink_sec;			/* 电量通讯失联时间 */
	/*------------------------------------------------------*/
	uint8_t sw_nolink				:1;	/*					*/
	uint8_t sw_nolink_sec;				/*					*/
	uint8_t ChargeRunFlag			:1;	/*					*/
	/*------------------------------------------------------*/
	/*!----------------COMMON------------*/
	uint8_t u8OpenAnimationStep;
	uint8_t u8UartLostLinkTime;
	uint8_t u8SingleWireLostLinkTime;
	uint8_t u8NoSpeedTimeoutTime;
	uint8_t u8ClearOdographTimeout;
	uint8_t u8RunTimeHour;
	uint8_t u8RunTimeMin;
	uint8_t u8RunTimeSec;
	uint8_t u8SpeedNumber;
	uint8_t u8SpeedNumberKmh;
	uint8_t u8ShiftNumber;
	uint8_t u8BatteryNumber;
	uint8_t u8BatteryLevel;
	uint8_t u8BatteryNumber2;
	uint8_t u8BatteryLevel2;
	
	uint8_t poweron_5s;
	double  TemptureNumber;
	double  OdoNumber;
	double  OdoNumberKm;
	double  TripNumber;
	double  TripNumberKm;
}system_t;
	
#endif
