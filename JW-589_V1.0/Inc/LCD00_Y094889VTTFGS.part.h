/*!-----------------------------------------------------------------
 * @file 	[LCD00_Y094889VTTFGS.part.h]
 * @remark	[液晶面板的全部显示部件定义]
 *------------------------------------------------------------------
 */
#ifndef __LCD00_Y094889VTTFGS_PART_H__
#define __LCD00_Y094889VTTFGS_PART_H__

/*!-----------------------------------------------------------------
 */

/*！#1~#17 */
typedef struct{
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
	uint8_t TirePressure :1; /* 胎压 */
	uint8_t TCS :1;
	uint8_t BlueTooth :1;
}flag_t;

/*！#18 */
	typedef struct{
	float number;						/* 温度值			*/
	uint8_t c_onoff_flag			:1;	/* 是否显示			*/
}tempture_t;

/*！#19 */
typedef struct{
	uint8_t number;						/* 档位值			*/
	uint8_t c_onoff_flag			:1;	/* 是否显示			*/
}shift_t;

/*！#20 */
typedef struct{
	uint8_t hour;						/* 时间-时			*/
	uint8_t min;						/* 时间-分			*/
	uint8_t sec;						/* 时间-秒			*/
	uint8_t c_onoff_flag			:1;	/* 是否显示			*/
	uint8_t s_colon_flag			:1;	/* 是否显示[:]		*/
	uint8_t s_am_flag				:1;	/* 是否显示[am]		*/
	uint8_t s_pm_flag				:1;	/* 是否显示[pm]		*/
}time_t;

/*！#21 */
typedef struct{
	uint8_t number;						/* 速度值			*/
	uint8_t c_onoff_flag			:1;	/* 是否显示			*/
	uint8_t c_onoff_number_flag		:1;	/* 是否显示数字		*/
	uint8_t s_mph_flag				:1;	/* 是否显示[MPH]	*/
	uint8_t s_kmh_flag				:1;	/* 是否显示[km/h]	*/
}speed_t;

/*！#22 */
typedef struct{
	uint8_t id						:2;	/* 电池编号			*/
	uint8_t level					:4;	/* 电池等级			*/
	uint8_t percent;					/* 电量百分比		*/
	uint8_t c_onoff_flag			:1;	/* 是否显示			*/
	uint8_t s_e_flag				:1;	/* 是否显示[E]		*/
	uint8_t s_f_flag				:1;	/* 是否显示[F]		*/
	uint8_t s_icon_flag				:1;	/* 是否显示图标		*/
	uint8_t s_biankuang_flag		:1;	/* 是否显示边框		*/
}battery_t;

/*！#23 */
typedef struct{
	uint8_t id						:2;	/* 里程类型			*/
	float number;						/* 里程值			*/
	uint8_t c_onoff_flag			:1;	/* 是否显示			*/
	uint8_t s_odo_flag				:1;	/* 是否显示[ODO]	*/
	uint8_t s_trip_flag				:1;	/* 是否显示[TRIP]	*/
	uint8_t s_miles_flag			:1;	/* 是否显示[miles]	*/
	uint8_t s_km_flag				:1;	/* 是否显示[km]		*/
	uint8_t s_a_flag				:1;	/* 是否显示[A]		*/
	uint8_t s_b_flag				:1;	/* 是否显示[B]		*/
}odograph_t;

/*！*/
typedef struct{
	/*------------------------------------------------------*/
	uint8_t    billtype;				/* bill显示类型 ODO-TRIP-ERRORCODE */
	odograph_t odo_odo;					/* 里程信息ODO		*/
	odograph_t odo_trip;				/* 里程信息TRIP		*/
	odograph_t odo_errorcode;		/* 里程信息TRIP		*/
	battery_t bat_a;					/* 电池信息A		*/
	battery_t bat_b;					/* 电池信息B		*/
	speed_t speed;						/* 速度信息			*/
	time_t time;						/* 时间信息			*/
	shift_t shift;						/* 档位信息			*/
	tempture_t tempture;				/* 温度信息			*/
	/*------------------------------------------------------*/
	flag_t flags;						/* 状态信息			*/
	/*------------------------------------------------------*/
}part_t;

#endif
