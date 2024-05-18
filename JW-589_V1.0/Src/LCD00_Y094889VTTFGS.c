/*!-----------------------------------------------------------------
 * @file 	[LCD00_Y094889VTTFGS.c]
 * @driver	[Sitronix/ST7037]
 *------------------------------------------------------------------
 */
#include <math.h>
#include "lptim32.h"
#include "lcd.h"
#include "can.h"
#include "LCD00_Y094889VTTFGS.h"
#include "LCD00_Y094889VTTFGS.tf.h"
#include "SystemRunning.h"
#include "idle.h"
#include "float.h"
#include <math.h>
/*!-----------------------------------------------------------------
 */
static  seg_t	_seg;

/* . g f e d c b a */
static uint8_t tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x73,0x40,0x77};
part_t	_part;

/*!<
 * @name	: getfloatinfo
 * @return	: floatinfo_t
 */
floatinfo_t getfloatinfo(float f)
{
	floatinfo_t result;
	double int_part, frc_part;
	uint8_t right;
	uint32_t left;

	frc_part	= modf(f, &int_part);
	left		= (uint32_t)int_part;
	right		= frc_part * 100;

	result.shiwan = left / 100000;
	result.wan = left % 100000 / 10000;
	result.qian = left % 10000 / 1000;
	result.bai = left % 1000 / 100;
	result.shi = left % 100 / 10;
	result.ge = left % 10;
	result.shifen = right / 10;
	result.baifen = right % 10;
	return result;
}



/* #18- 温度信息 */
static void lcd_display_tempture(tempture_t tempture)
{
	floatinfo_t info;
	uint8_t i_shi, i_ge, i_shifen;
	
	info = getfloatinfo(tempture.number);
	i_shi = tab[info.shi];
	i_ge = tab[info.ge];
	i_shifen = tab[info.shifen];
	
	_seg.bits.P_3A = tempture.c_onoff_flag & GetBit(i_shi, 0);		// i_shi
	_seg.bits.P_3B = tempture.c_onoff_flag & GetBit(i_shi, 1);
	_seg.bits.P_3C = tempture.c_onoff_flag & GetBit(i_shi, 2);
	_seg.bits.P_3D = tempture.c_onoff_flag & GetBit(i_shi, 3);
	_seg.bits.P_3E = tempture.c_onoff_flag & GetBit(i_shi, 4);
	_seg.bits.P_3F = tempture.c_onoff_flag & GetBit(i_shi, 5);
	_seg.bits.P_3G = tempture.c_onoff_flag & GetBit(i_shi, 6);
	_seg.bits.P_4A = tempture.c_onoff_flag & GetBit(i_ge, 0);		// i_ge
	_seg.bits.P_4B = tempture.c_onoff_flag & GetBit(i_ge, 1);
	_seg.bits.P_4C = tempture.c_onoff_flag & GetBit(i_ge, 2);
	_seg.bits.P_4D = tempture.c_onoff_flag & GetBit(i_ge, 3);
	_seg.bits.P_4E = tempture.c_onoff_flag & GetBit(i_ge, 4);
	_seg.bits.P_4F = tempture.c_onoff_flag & GetBit(i_ge, 5);
	_seg.bits.P_4G = tempture.c_onoff_flag & GetBit(i_ge, 6);
	_seg.bits.P_T10 = tempture.c_onoff_flag;						// s_dot
	_seg.bits.P_5A = tempture.c_onoff_flag & GetBit(i_shifen, 0);	// i_shifen
	_seg.bits.P_5B = tempture.c_onoff_flag & GetBit(i_shifen, 1);
	_seg.bits.P_5C = tempture.c_onoff_flag & GetBit(i_shifen, 2);
	_seg.bits.P_5D = tempture.c_onoff_flag & GetBit(i_shifen, 3);
	_seg.bits.P_5E = tempture.c_onoff_flag & GetBit(i_shifen, 4);
	_seg.bits.P_5F = tempture.c_onoff_flag & GetBit(i_shifen, 5);
	_seg.bits.P_5G = tempture.c_onoff_flag & GetBit(i_shifen, 6);
	_seg.bits.P_T11 = tempture.c_onoff_flag;						// s_unit
}

/* #19- 档位信息 */
static void lcd_display_shift(shift_t shift)
{
//	uint8_t i_ge = tab[shift.number];
	uint8_t i_ge = shift.number;
	if(thissystem.IsOpenAnimationFinished == 1){
				/*  MCU 心跳超时 */
		if(mcu_die_flag == 1){
				_seg.bits.P_T31   = 1;		// 一档圆弧指示条
				_seg.bits.P_T32   = 1;		// 二档圆弧指示条
				_seg.bits.P_T33   = 1;		// 三档圆弧指示条	
				_seg.bits.P_T28   = 0;		// 一档色块
				_seg.bits.P_T29   = 0;		// 二档色块
				_seg.bits.P_T30   = 0;		// 三档色块			
				return;
		}else if((thissystem.ParkingShiftFlag==1)||(thisshift == 7)||(thisshift == 0)){
			/*	7倒挡  0P档		*/
				_seg.bits.P_T31   = 0;		// 一档圆弧指示条
				_seg.bits.P_T32   = 0;		// 二档圆弧指示条
				_seg.bits.P_T33   = 0;		// 三档圆弧指示条	
				_seg.bits.P_T28   = 0;		// 一档色块
				_seg.bits.P_T29   = 0;		// 二档色块
				_seg.bits.P_T30   = 0;		// 三档色块	

				_seg.bits.P_T37   = 0;    //速度百位 清零
				_seg.bits.P_12A   = 0;		//速度十位 清零
				_seg.bits.P_12B 	= 0;
				_seg.bits.P_12C 	= 0;
				_seg.bits.P_12D 	= 0;
				_seg.bits.P_12E 	= 0;
				_seg.bits.P_12F 	= 0;
				_seg.bits.P_12G 	= 0;

				_seg.bits.P_13A   = 1;		//速度十位 清零
				_seg.bits.P_13B 	= 1;
				_seg.bits.P_13C 	= 1;
				_seg.bits.P_13D 	= 0;
				_seg.bits.P_13E 	= 1;
				_seg.bits.P_13F 	= 1;
				_seg.bits.P_13G 	= 1;
				return;
			}
  }
	switch(i_ge){
    case 1:
			_seg.bits.P_T31   = 1;		// 一档圆弧指示条
			_seg.bits.P_T32   = 1;		// 二档圆弧指示条
			_seg.bits.P_T33   = 1;		// 三档圆弧指示条	
			_seg.bits.P_T28   = 1;		// 一档色块
			_seg.bits.P_T29   = 0;		// 二档色块
			_seg.bits.P_T30   = 0;		// 三档色块					
			break;
		case 2:
			_seg.bits.P_T31   = 1;		// 一档圆弧指示条
			_seg.bits.P_T32   = 1;		// 二档圆弧指示条
			_seg.bits.P_T33   = 1;		// 三档圆弧指示条	
			_seg.bits.P_T28   = 0;		// 一档色块
			_seg.bits.P_T29   = 1;		// 二档色块
			_seg.bits.P_T30   = 0;		// 三档色块			
			break;		
		case 3:
			_seg.bits.P_T31   = 1;		// 一档圆弧指示条
			_seg.bits.P_T32   = 1;		// 二档圆弧指示条
			_seg.bits.P_T33   = 1;		// 三档圆弧指示条	
			_seg.bits.P_T28   = 0;		// 一档色块
			_seg.bits.P_T29   = 0;		// 二档色块
			_seg.bits.P_T30   = 1;		// 三档色块			
			break;
		case 4:
			/*	4档待定 先与3档同样显示	*/
			_seg.bits.P_T31   = 1;		// 一档圆弧指示条
			_seg.bits.P_T32   = 1;		// 二档圆弧指示条
			_seg.bits.P_T33   = 1;		// 三档圆弧指示条	
			_seg.bits.P_T28   = 0;		// 一档色块
			_seg.bits.P_T29   = 0;		// 二档色块
			_seg.bits.P_T30   = 1;		// 三档色块			
			break;
		default:
			_seg.bits.P_T31   = 1;		// 一档圆弧指示条
			_seg.bits.P_T32   = 1;		// 二档圆弧指示条
			_seg.bits.P_T33   = 1;		// 三档圆弧指示条	
			_seg.bits.P_T28   = 1;		// 一档色块
			_seg.bits.P_T29   = 0;		// 二档色块
			_seg.bits.P_T30   = 0;		// 三档色块					
			break;
		
	}

}

/* #20- 时间信息 */
static void lcd_display_time(time_t time)
{
	uint8_t v1 = time.hour;
	uint8_t v2 = time.min;
	uint8_t v1_shi, v1_ge, v2_shi, v2_ge;
	
	v1_shi = v1 / 10;
	v1_ge  = v1 % 10;
	v2_shi = v2 / 10;
	v2_ge  = v2 % 10;
	
	v1_shi = tab[v1_shi];
	v1_ge  = tab[v1_ge];
	v2_shi = tab[v2_shi];
	v2_ge  = tab[v2_ge];	
	/*  VCU 心跳超时 并且动画结束*/
	if((vcu_die_flag == 1)&&(thissystem.IsOpenAnimationFinished==1)){
	 v1_shi = tab[17];
	 v1_ge  = tab[17];
	 v2_shi = tab[17];
	 v2_ge  = tab[17];
	 time.s_am_flag = 0;		// s_am
	 time.s_pm_flag = 0;;		// s_pm		
	}	
	_seg.bits.P_14A = time.c_onoff_flag & GetBit(v1_shi, 0);		// v1_shi
	_seg.bits.P_14B = time.c_onoff_flag & GetBit(v1_shi, 1);
	_seg.bits.P_14C = time.c_onoff_flag & GetBit(v1_shi, 2);
	_seg.bits.P_14D = time.c_onoff_flag & GetBit(v1_shi, 3);
	_seg.bits.P_14E = time.c_onoff_flag & GetBit(v1_shi, 4);
	_seg.bits.P_14F = time.c_onoff_flag & GetBit(v1_shi, 5);
	_seg.bits.P_14G = time.c_onoff_flag & GetBit(v1_shi, 6);
	_seg.bits.P_15A = time.c_onoff_flag & GetBit(v1_ge, 0);		// v1_ge
	_seg.bits.P_15B = time.c_onoff_flag & GetBit(v1_ge, 1);
	_seg.bits.P_15C = time.c_onoff_flag & GetBit(v1_ge, 2);
	_seg.bits.P_15D = time.c_onoff_flag & GetBit(v1_ge, 3);
	_seg.bits.P_15E = time.c_onoff_flag & GetBit(v1_ge, 4);
	_seg.bits.P_15F = time.c_onoff_flag & GetBit(v1_ge, 5);
	_seg.bits.P_15G = time.c_onoff_flag & GetBit(v1_ge, 6);
	_seg.bits.P_T22 = time.c_onoff_flag & time.s_colon_flag;		// s_colon
	_seg.bits.P_16A = time.c_onoff_flag & GetBit(v2_shi, 0);		// v2_shi
	_seg.bits.P_16B = time.c_onoff_flag & GetBit(v2_shi, 1);
	_seg.bits.P_16C = time.c_onoff_flag & GetBit(v2_shi, 2);
	_seg.bits.P_16D = time.c_onoff_flag & GetBit(v2_shi, 3);
	_seg.bits.P_16E = time.c_onoff_flag & GetBit(v2_shi, 4);
	_seg.bits.P_16F = time.c_onoff_flag & GetBit(v2_shi, 5);
	_seg.bits.P_16G = time.c_onoff_flag & GetBit(v2_shi, 6);
	_seg.bits.P_17A = time.c_onoff_flag & GetBit(v2_ge, 0);		// v2_ge
	_seg.bits.P_17B = time.c_onoff_flag & GetBit(v2_ge, 1);
	_seg.bits.P_17C = time.c_onoff_flag & GetBit(v2_ge, 2);
	_seg.bits.P_17D = time.c_onoff_flag & GetBit(v2_ge, 3);
	_seg.bits.P_17E = time.c_onoff_flag & GetBit(v2_ge, 4);
	_seg.bits.P_17F = time.c_onoff_flag & GetBit(v2_ge, 5);
	_seg.bits.P_17G = time.c_onoff_flag & GetBit(v2_ge, 6);
//	_seg.bits.P_S9 = time.c_onoff_flag & time.s_am_flag;		// s_am
//	_seg.bits.P_S10= time.c_onoff_flag & time.s_pm_flag;		// s_pm
}

/* #21- 速度信息 */
static void lcd_display_speed(speed_t speed)
{
	uint8_t i_shi, i_ge;
	i_shi = speed.number / 10;
	i_ge  = speed.number % 10;
	if(thissystem.IsOpenAnimationFinished==0){
		thissystem.ParkingShiftFlag = 0;
	}
	
	/*  MCU 心跳超时 并且动画结束*/
	if((mcu_die_flag == 1)&&(thissystem.IsOpenAnimationFinished==1)){
		i_shi = tab[17];
		i_ge  = tab[17];
		speed.s_mph_flag	= 0;
		speed.s_kmh_flag	= 0;		
	}else	if(thissystem.ParkingShiftFlag==1){
		i_shi = 0;
		i_ge  = tab[16];
		speed.s_mph_flag	= 0;
		speed.s_kmh_flag	= 0;
	}else if(thissystem.ReverseShiftFlag == 1){
		i_shi = 0;
		i_ge  = tab[18];
		speed.s_mph_flag	= 0;
		speed.s_kmh_flag	= 0;	
	}else{
		i_shi = tab[i_shi];
		i_ge  = tab[i_ge];
	}
	
//	_seg.bits.P_S16_0 = speed.c_onoff_flag;													// s_biankuang  新款没有外框
//	_seg.bits.P_S16_1 = speed.c_onoff_flag;
	_seg.bits.P_12A = speed.c_onoff_flag & GetBit(i_shi, 0) & speed.c_onoff_number_flag;	// i_shi
	_seg.bits.P_12B = speed.c_onoff_flag & GetBit(i_shi, 1) & speed.c_onoff_number_flag;
	_seg.bits.P_12C = speed.c_onoff_flag & GetBit(i_shi, 2) & speed.c_onoff_number_flag;
	_seg.bits.P_12D = speed.c_onoff_flag & GetBit(i_shi, 3) & speed.c_onoff_number_flag;
	_seg.bits.P_12E = speed.c_onoff_flag & GetBit(i_shi, 4) & speed.c_onoff_number_flag;
	_seg.bits.P_12F = speed.c_onoff_flag & GetBit(i_shi, 5) & speed.c_onoff_number_flag;
	_seg.bits.P_12G = speed.c_onoff_flag & GetBit(i_shi, 6) & speed.c_onoff_number_flag;
//	_seg.bits.P_15A_1 = _seg.bits.P_15A_0;
//	_seg.bits.P_15B_1 = _seg.bits.P_15B_0;
//	_seg.bits.P_15C_1 = _seg.bits.P_15C_0;
//	_seg.bits.P_15D_1 = _seg.bits.P_15D_0;
//	_seg.bits.P_15E_1 = _seg.bits.P_15E_0;
//	_seg.bits.P_15G_1 = _seg.bits.P_15G_0;
	_seg.bits.P_13A = speed.c_onoff_flag & GetBit(i_ge, 0) & speed.c_onoff_number_flag;	// i_ge
	_seg.bits.P_13B = speed.c_onoff_flag & GetBit(i_ge, 1) & speed.c_onoff_number_flag;
	_seg.bits.P_13C = speed.c_onoff_flag & GetBit(i_ge, 2) & speed.c_onoff_number_flag;
	_seg.bits.P_13D = speed.c_onoff_flag & GetBit(i_ge, 3) & speed.c_onoff_number_flag;
	_seg.bits.P_13E = speed.c_onoff_flag & GetBit(i_ge, 4) & speed.c_onoff_number_flag;
	_seg.bits.P_13F = speed.c_onoff_flag & GetBit(i_ge, 5) & speed.c_onoff_number_flag;
	_seg.bits.P_13G = speed.c_onoff_flag & GetBit(i_ge, 6) & speed.c_onoff_number_flag;
//	_seg.bits.P_16A_1 = _seg.bits.P_16A_0;
//	_seg.bits.P_16B_1 = _seg.bits.P_16B_0;
//	_seg.bits.P_16C_1 = _seg.bits.P_16C_0;
//	_seg.bits.P_16D_1 = _seg.bits.P_16D_0;
//	_seg.bits.P_16E_1 = _seg.bits.P_16E_0;
//	_seg.bits.P_16F_1 = _seg.bits.P_16F_0;
//	_seg.bits.P_16G_1 = _seg.bits.P_16G_0;
	_seg.bits.P_T34	  = speed.c_onoff_flag & speed.s_mph_flag;								// s_mph
	_seg.bits.P_T36   = speed.c_onoff_flag & speed.s_kmh_flag;								// s_kmh
}

/* #22- 电池信息 */
static void lcd_display_battery(battery_t bat)
{
	uint8_t  i_shi, i_ge;
	
//	i_bai = bat.percent / 100;
	i_shi = (bat.percent % 100) / 10;
	i_ge  = (bat.percent % 100) % 10;
	/*  BMS 心跳超时 并且动画结束*/	
	if((bms_die_flag)&&(thissystem.IsOpenAnimationFinished==1)){
	  i_shi = tab[17];
	  i_ge  = tab[17];	
		bat.percent = 0;
		bat.level = 0;
		_seg.bits.P_T2 = 0;									// 电量%		
	}else{
	  i_shi = tab[i_shi];
	  i_ge  = tab[i_ge];
		_seg.bits.P_T2 = bat.c_onoff_flag;									// s_percent		
	}
	if(bat.id == E_BATTERY_A){
		_seg.bits.P_T3	= bat.c_onoff_flag & bat.s_icon_flag;				// s_icon
		_seg.bits.P_L2  = bat.c_onoff_flag & (bat.level > 9);				// s_level_10_f
		_seg.bits.P_L3  = bat.c_onoff_flag & (bat.level > 8);				// s_level_9
		_seg.bits.P_L4  = bat.c_onoff_flag & (bat.level > 7);				// s_level_8
		_seg.bits.P_L5  = bat.c_onoff_flag & (bat.level > 6);				// s_level_7
		_seg.bits.P_L6  = bat.c_onoff_flag & (bat.level > 5);				// s_level_6
		_seg.bits.P_L7  = bat.c_onoff_flag & (bat.level > 4);				// s_level_5
		_seg.bits.P_L8  = bat.c_onoff_flag & (bat.level > 3);				// s_level_4
		_seg.bits.P_L9  = bat.c_onoff_flag & (bat.level > 2);				// s_level_3
		_seg.bits.P_L10 = bat.c_onoff_flag & (bat.level > 1);				// s_level_2
		_seg.bits.P_L11 = bat.c_onoff_flag & (bat.level > 0);				// & bat.s_e_flag;// s_level_1_e
		_seg.bits.P_L1 = bat.c_onoff_flag & bat.s_biankuang_flag;			// 电池框
		_seg.bits.P_T1 = bat.c_onoff_flag & (bat.percent == 100);			// 电池100%之百位
		_seg.bits.P_1A 	= bat.c_onoff_flag & GetBit(i_shi, 0);				// i_shi
		_seg.bits.P_1B 	= bat.c_onoff_flag & GetBit(i_shi, 1);
		_seg.bits.P_1C 	= bat.c_onoff_flag & GetBit(i_shi, 2);
		_seg.bits.P_1D 	= bat.c_onoff_flag & GetBit(i_shi, 3);
		_seg.bits.P_1E 	= bat.c_onoff_flag & GetBit(i_shi, 4);
		_seg.bits.P_1F 	= bat.c_onoff_flag & GetBit(i_shi, 5);
		_seg.bits.P_1G 	= bat.c_onoff_flag & GetBit(i_shi, 6);
		_seg.bits.P_2A 	= bat.c_onoff_flag & GetBit(i_ge, 0);				// i_ge
		_seg.bits.P_2B 	= bat.c_onoff_flag & GetBit(i_ge, 1);
		_seg.bits.P_2C 	= bat.c_onoff_flag & GetBit(i_ge, 2);
		_seg.bits.P_2D 	= bat.c_onoff_flag & GetBit(i_ge, 3);
		_seg.bits.P_2E 	= bat.c_onoff_flag & GetBit(i_ge, 4);
		_seg.bits.P_2F 	= bat.c_onoff_flag & GetBit(i_ge, 5);
		_seg.bits.P_2G 	= bat.c_onoff_flag & GetBit(i_ge, 6);
//		_seg.bits.P_A14 = bat.c_onoff_flag;									// s_percent
	}
	else if(bat.id == E_BATTERY_B){
		_seg.bits.P_T3	= bat.c_onoff_flag & bat.s_icon_flag;				// s_icon
		_seg.bits.P_L13  = bat.c_onoff_flag & (bat.level > 9);				// s_level_10_f
		_seg.bits.P_L14  = bat.c_onoff_flag & (bat.level > 8);				// s_level_9
		_seg.bits.P_L15  = bat.c_onoff_flag & (bat.level > 7);				// s_level_8
		_seg.bits.P_L16  = bat.c_onoff_flag & (bat.level > 6);				// s_level_7
		_seg.bits.P_L17  = bat.c_onoff_flag & (bat.level > 5);				// s_level_6
		_seg.bits.P_L18  = bat.c_onoff_flag & (bat.level > 4);				// s_level_5
		_seg.bits.P_L19  = bat.c_onoff_flag & (bat.level > 3);				// s_level_4
		_seg.bits.P_L20  = bat.c_onoff_flag & (bat.level > 2);				// s_level_3
		_seg.bits.P_L21 = bat.c_onoff_flag & (bat.level > 1);				// s_level_2
		_seg.bits.P_L22 = bat.c_onoff_flag & (bat.level > 0) & bat.s_e_flag;// s_level_1_e
		_seg.bits.P_L12 = bat.c_onoff_flag & bat.s_biankuang_flag;			// s_biankuang
		_seg.bits.P_T1 = bat.c_onoff_flag & (bat.percent == 100);			// i_bai
		_seg.bits.P_7A 	= bat.c_onoff_flag & GetBit(i_shi, 0);				// i_shi
		_seg.bits.P_7B 	= bat.c_onoff_flag & GetBit(i_shi, 1);
		_seg.bits.P_7C 	= bat.c_onoff_flag & GetBit(i_shi, 2);
		_seg.bits.P_7D 	= bat.c_onoff_flag & GetBit(i_shi, 3);
		_seg.bits.P_7E 	= bat.c_onoff_flag & GetBit(i_shi, 4);
		_seg.bits.P_7F 	= bat.c_onoff_flag & GetBit(i_shi, 5);
		_seg.bits.P_7G 	= bat.c_onoff_flag & GetBit(i_shi, 6);
		_seg.bits.P_8A 	= bat.c_onoff_flag & GetBit(i_ge, 0);				// i_ge
		_seg.bits.P_8B 	= bat.c_onoff_flag & GetBit(i_ge, 1);
		_seg.bits.P_8C 	= bat.c_onoff_flag & GetBit(i_ge, 2);
		_seg.bits.P_8D 	= bat.c_onoff_flag & GetBit(i_ge, 3);
		_seg.bits.P_8E 	= bat.c_onoff_flag & GetBit(i_ge, 4);
		_seg.bits.P_8F 	= bat.c_onoff_flag & GetBit(i_ge, 5);
		_seg.bits.P_8G 	= bat.c_onoff_flag & GetBit(i_ge, 6);
		_seg.bits.P_T2 = bat.c_onoff_flag;									// s_percent
	}
	else{
		// INVALID CASE
	}
}

/* #23- 里程信息 */
static void lcd_display_odograph(part_t thispart)
{
	floatinfo_t info;
	uint8_t i_wan, i_qian, i_bai, i_shi, i_ge, i_shifen;
  double int_part, frc_part;
  /* 开机前5秒显示ODO 当然也包括动画时期 */
	if(thissystem.poweron_5s == 0){
			if(thispart.billtype == E_ODOGRAPH_ODO){
				frc_part	= modf(thispart.odo_odo.number, &int_part);
				if(frc_part>=0.5) int_part +=1;
				thispart.odo_odo.number = int_part+0.1;		
				info		 = getfloatinfo(thispart.odo_odo.number / 10);
				i_wan		 = tab[info.wan];
				i_qian	 = tab[info.qian];
				i_bai		 = tab[info.bai];
				i_shi		 = tab[info.shi];
				i_ge		 = tab[info.ge];
				i_shifen = tab[info.shifen];		
				/* 动画阶段TRIP仅显示 TRIP字符 这里与老屏差异 */
				if(thissystem.IsOpenAnimationFinished == 0){
					_seg.bits.P_T15	= 1;							              // 动画时也显示TRIP
				}else{
					_seg.bits.P_T15	= 0;							              // 显示ODO关闭TRIP
				}			
				
						
				_seg.bits.P_T16	= thispart.odo_odo.c_onoff_flag;							// s_odo
				_seg.bits.P_6A 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 0);		// i_wan
				_seg.bits.P_6B 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 1);
				_seg.bits.P_6C 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 2);
				_seg.bits.P_6D 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 3);
				_seg.bits.P_6E 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 4);
				_seg.bits.P_6F 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 5);
				_seg.bits.P_6G 	= thispart.odo_odo.c_onoff_flag & GetBit(i_wan, 6);
				_seg.bits.P_7A = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 0);		// i_qian
				_seg.bits.P_7B = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 1);
				_seg.bits.P_7C = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 2);
				_seg.bits.P_7D = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 3);
				_seg.bits.P_7E = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 4);
				_seg.bits.P_7F = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 5);
				_seg.bits.P_7G = thispart.odo_odo.c_onoff_flag & GetBit(i_qian, 6);
				_seg.bits.P_8A = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 0);		// i_bai
				_seg.bits.P_8B = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 1);
				_seg.bits.P_8C = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 2);
				_seg.bits.P_8D = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 3);
				_seg.bits.P_8E = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 4);
				_seg.bits.P_8F = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 5);
				_seg.bits.P_8G = thispart.odo_odo.c_onoff_flag & GetBit(i_bai, 6);
				_seg.bits.P_9A = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 0);		// i_shi
				_seg.bits.P_9B = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 1);
				_seg.bits.P_9C = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 2);
				_seg.bits.P_9D = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 3);
				_seg.bits.P_9E = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 4);
				_seg.bits.P_9F = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 5);
				_seg.bits.P_9G = thispart.odo_odo.c_onoff_flag & GetBit(i_shi, 6);
				_seg.bits.P_10A = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 0);		// i_ge
				_seg.bits.P_10B = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 1);
				_seg.bits.P_10C = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 2);
				_seg.bits.P_10D = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 3);
				_seg.bits.P_10E = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 4);
				_seg.bits.P_10F = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 5);
				_seg.bits.P_10G = thispart.odo_odo.c_onoff_flag & GetBit(i_ge, 6);
				_seg.bits.P_T12 = 0;//odo.c_onoff_flag;							// s_dot
				_seg.bits.P_11A = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 0);	// i_shifen
				_seg.bits.P_11B = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 1);
				_seg.bits.P_11C = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 2);
				_seg.bits.P_11D = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 3);
				_seg.bits.P_11E = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 4);
				_seg.bits.P_11F = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 5);
				_seg.bits.P_11G = thispart.odo_odo.c_onoff_flag & GetBit(i_shifen, 6);
				_seg.bits.P_T13 = thispart.odo_odo.c_onoff_flag & thispart.odo_odo.s_miles_flag;		// s_miles
				_seg.bits.P_T14 = thispart.odo_odo.c_onoff_flag & thispart.odo_odo.s_km_flag;			// s_km
			}
	}else {
		/*判断有没有故障码*/
		if(errorcode != 0){
			i_wan		= tab[14];
			i_qian		= tab[errorcode/100];
			i_bai		= tab[(errorcode%100)/10];
			i_shi		= tab[errorcode%10];
			i_ge		= 0;
			i_shifen	= 0;
			_seg.bits.P_T15	= 0;							// s_trip
			_seg.bits.P_T16	= 0;              // s_odo
//			_seg.bits.P_S23	= 0;			// s_a  新款没有AB区间里程
//			_seg.bits.P_S24	= 0;			// s_b
		_seg.bits.P_6A 	= GetBit(i_wan, 0);		// i_wan
		_seg.bits.P_6B 	= GetBit(i_wan, 1);
		_seg.bits.P_6C 	= GetBit(i_wan, 2);
		_seg.bits.P_6D 	= GetBit(i_wan, 3);
		_seg.bits.P_6E 	= GetBit(i_wan, 4);
		_seg.bits.P_6F 	= GetBit(i_wan, 5);
		_seg.bits.P_6G 	= GetBit(i_wan, 6);
		_seg.bits.P_7A = GetBit(i_qian, 0);		// i_qian
		_seg.bits.P_7B = GetBit(i_qian, 1);
		_seg.bits.P_7C = GetBit(i_qian, 2);
		_seg.bits.P_7D = GetBit(i_qian, 3);
		_seg.bits.P_7E = GetBit(i_qian, 4);
		_seg.bits.P_7F = GetBit(i_qian, 5);
		_seg.bits.P_7G = GetBit(i_qian, 6);
		_seg.bits.P_8A = GetBit(i_bai, 0);		// i_bai
		_seg.bits.P_8B = GetBit(i_bai, 1);
		_seg.bits.P_8C = GetBit(i_bai, 2);
		_seg.bits.P_8D = GetBit(i_bai, 3);
		_seg.bits.P_8E = GetBit(i_bai, 4);
		_seg.bits.P_8F = GetBit(i_bai, 5);
		_seg.bits.P_8G = GetBit(i_bai, 6);
		_seg.bits.P_9A = GetBit(i_shi, 0);		// i_shi
		_seg.bits.P_9B = GetBit(i_shi, 1);
		_seg.bits.P_9C = GetBit(i_shi, 2);
		_seg.bits.P_9D = GetBit(i_shi, 3);
		_seg.bits.P_9E = GetBit(i_shi, 4);
		_seg.bits.P_9F = GetBit(i_shi, 5);
		_seg.bits.P_9G = GetBit(i_shi, 6);
		_seg.bits.P_10A = GetBit(i_ge, 0);		// i_ge
		_seg.bits.P_10B = GetBit(i_ge, 1);
		_seg.bits.P_10C = GetBit(i_ge, 2);
		_seg.bits.P_10D = GetBit(i_ge, 3);
		_seg.bits.P_10E = GetBit(i_ge, 4);
		_seg.bits.P_10F = GetBit(i_ge, 5);
		_seg.bits.P_10G = GetBit(i_ge, 6);
		_seg.bits.P_T12 = 0;//odo.c_onoff_flag;							// s_dot
		_seg.bits.P_11A = GetBit(i_shifen, 0);	// i_shifen
		_seg.bits.P_11B = GetBit(i_shifen, 1);
		_seg.bits.P_11C = GetBit(i_shifen, 2);
		_seg.bits.P_11D = GetBit(i_shifen, 3);
		_seg.bits.P_11E = GetBit(i_shifen, 4);
		_seg.bits.P_11F = GetBit(i_shifen, 5);
		_seg.bits.P_11G = GetBit(i_shifen, 6);
		_seg.bits.P_T13 = 0;		//有故障码不显示单位
		_seg.bits.P_T14 = 0;		//有故障码不显示单位	
		}else{
			
		  if(mcu_die_flag){
			  info = getfloatinfo(0.0);        /* MCU失联 显示0  */
		  }else{
				info = getfloatinfo(thispart.odo_trip.number+0.01); /* 正常显示  */
			}				
		  i_wan		= tab[info.wan];
			i_qian	= tab[info.qian];
			i_bai		= tab[info.bai];
			i_shi		= tab[info.shi];
			i_ge		= tab[info.ge];
			i_shifen	= tab[info.shifen];		
//printf("%3.1f %d %d %d \r\n",odo.number,info.shi,info.ge,info.shifen);			
			_seg.bits.P_T16	= 0;							              // 显示TRIP关闭ODO
			_seg.bits.P_T15	= thispart.odo_trip.c_onoff_flag;							// s_trip
		_seg.bits.P_6A 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 0);		// i_wan
		_seg.bits.P_6B 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 1);
		_seg.bits.P_6C 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 2);
		_seg.bits.P_6D 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 3);
		_seg.bits.P_6E 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 4);
		_seg.bits.P_6F 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 5);
		_seg.bits.P_6G 	= thispart.odo_trip.c_onoff_flag & GetBit(i_wan, 6);
		_seg.bits.P_7A = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 0);		// i_qian
		_seg.bits.P_7B = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 1);
		_seg.bits.P_7C = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 2);
		_seg.bits.P_7D = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 3);
		_seg.bits.P_7E = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 4);
		_seg.bits.P_7F = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 5);
		_seg.bits.P_7G = thispart.odo_trip.c_onoff_flag & GetBit(i_qian, 6);
		_seg.bits.P_8A = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 0);		// i_bai
		_seg.bits.P_8B = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 1);
		_seg.bits.P_8C = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 2);
		_seg.bits.P_8D = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 3);
		_seg.bits.P_8E = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 4);
		_seg.bits.P_8F = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 5);
		_seg.bits.P_8G = thispart.odo_trip.c_onoff_flag & GetBit(i_bai, 6);
		_seg.bits.P_9A = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 0);		// i_shi
		_seg.bits.P_9B = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 1);
		_seg.bits.P_9C = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 2);
		_seg.bits.P_9D = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 3);
		_seg.bits.P_9E = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 4);
		_seg.bits.P_9F = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 5);
		_seg.bits.P_9G = thispart.odo_trip.c_onoff_flag & GetBit(i_shi, 6);
		_seg.bits.P_10A = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 0);		// i_ge
		_seg.bits.P_10B = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 1);
		_seg.bits.P_10C = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 2);
		_seg.bits.P_10D = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 3);
		_seg.bits.P_10E = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 4);
		_seg.bits.P_10F = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 5);
		_seg.bits.P_10G = thispart.odo_trip.c_onoff_flag & GetBit(i_ge, 6);
		_seg.bits.P_T12 = 1;//odo.c_onoff_flag;							// s_dot
		_seg.bits.P_11A = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 0);	// i_shifen
		_seg.bits.P_11B = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 1);
		_seg.bits.P_11C = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 2);
		_seg.bits.P_11D = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 3);
		_seg.bits.P_11E = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 4);
		_seg.bits.P_11F = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 5);
		_seg.bits.P_11G = thispart.odo_trip.c_onoff_flag & GetBit(i_shifen, 6);
		_seg.bits.P_T13 = thispart.odo_trip.c_onoff_flag & thispart.odo_trip.s_miles_flag;		// s_miles
		_seg.bits.P_T14 = thispart.odo_trip.c_onoff_flag & thispart.odo_trip.s_km_flag;			// s_km
	 }
  }

}

/* lcd_display_clear_all_flags */
void lcd_display_clear_all_flags(void)
{
	/* CLEAR ALL FLAGS */
	_part.flags.TurnLeftFlag		= 0;			/* #1 - 左转向灯 */
	_part.flags.HighBeamFlag		= 0;			/* #2 - 远光灯 */
	_part.flags.WidthLampFlag		= 0;			/* #3 - 示宽灯 */
	_part.flags.SysRunFlag			= 0;			/* #4 - 电源状态灯 */
	_part.flags.LowBeamFlag			= 0;			/* #5 - 近光灯 */
	_part.flags.UnderVolWarningFlag	= 0;			/* #6 - 欠压灯 */
	_part.flags.TurnRightFlag		= 0;			/* #7 - 右转向灯 */
	_part.flags.CruiseRunFalg		= 0;			/* #8 - 巡航指示灯 */
	_part.flags.BrakeFaultFlag		= 0;			/* #9 - 刹把故障 */
	_part.flags.ControllerFaultFlag	= 0;			/* #10- 控制器故障 */
	_part.flags.SideshoringOffFlag	= 0;			/* #11- 单撑断电功能状态 */
	_part.flags.MotorFaultFlag		= 0;			/* #12- 电机故障 */
	_part.flags.HandleBarFaultFlag	= 0;			/* #13- 转把故障 */
	_part.flags.ReverseShiftFlag	= 0;			/* #14- 倒挡状态 */
	_part.flags.Unknown1Flag		= 0;			/* #15- UNKNOWN1 */
	_part.flags.Unknown2Flag		= 0;			/* #16- UNKNOWN2 */
	_part.flags.charge		= 0;			/* #17- UNKNOWN3 */
}

void lcd_display_show_all_symbols(void)
{
	/* DISPLAY ALL SYMBOLS */
	_part.tempture.c_onoff_flag			= 0;			/* #18- 温度信息 */
	_part.shift.c_onoff_flag			= 1;			/* #19- 档位信息 */
	_part.time.c_onoff_flag				= 1;			/* #20- 时间信息 */
	_part.time.s_colon_flag				= 1;
	_part.time.s_am_flag				= 1;
	_part.time.s_pm_flag				= 1;
	_part.speed.c_onoff_flag			= 1;			/* #21- 速度信息 */
	_part.speed.c_onoff_number_flag		= 1;
	_part.speed.s_mph_flag				= 1;
	_part.speed.s_kmh_flag				= 1;
	_part.bat_a.id						= E_BATTERY_A;	/* #22- 电池信息 */
	_part.bat_a.s_biankuang_flag		= 1;
	_part.bat_a.s_e_flag				= 1;
	_part.bat_a.s_f_flag				= 1;
	_part.bat_a.s_icon_flag				= 1;
	_part.bat_a.c_onoff_flag			= 1;
	_part.bat_b.id						= E_BATTERY_B;
	_part.bat_b.s_biankuang_flag		= 1;
	_part.bat_b.s_e_flag				= 1;
	_part.bat_b.s_f_flag				= 1;
	_part.bat_b.s_icon_flag				= 1;
	_part.bat_b.c_onoff_flag			= 0;
	_part.odo_odo.id					= E_ODOGRAPH_ODO;/* #23- 里程信息 */
	_part.odo_odo.s_odo_flag			= 1;
	_part.odo_odo.s_trip_flag			= 1;
	_part.odo_odo.s_miles_flag			= 1;
	_part.odo_odo.s_km_flag				= 1;
	_part.odo_odo.s_a_flag				= 1;
	_part.odo_odo.s_b_flag				= 1;
	_part.odo_odo.c_onoff_flag			= 1;
	_part.odo_trip.id					= E_ODOGRAPH_TRIP;
	_part.odo_trip.s_odo_flag			= 1;
	_part.odo_trip.s_trip_flag			= 1;
	_part.odo_trip.s_miles_flag			= 1;
	_part.odo_trip.s_km_flag			= 1;
	_part.odo_trip.s_a_flag				= 1;
	_part.odo_trip.s_b_flag				= 1;
	_part.odo_trip.c_onoff_flag			= 1;
}

/* lcd_display_animation */
void lcd_display_animation(uint8_t step)
{
	_part.tempture.number				= step * 11.11;			/* #18- 温度信息 */
	switch(step){
		case 0: _part.shift.number = 1;
			break;
		case 1: _part.shift.number = 2;
			break;		
		case 2: _part.shift.number = 3;
			break;
		case 3: _part.shift.number = 2;
			break;		
		case 4: _part.shift.number = 1;
			break;
		case 5: _part.shift.number = 2;
			break;		
		case 6: _part.shift.number = 3;
			break;
		case 7: _part.shift.number = 2;
			break;	
		case 8: _part.shift.number = 1;
			break;
		case 9: _part.shift.number = 2;
			break;		
	}

	_part.time.hour						= step * 11;			/* #20- 时间信息 */
	_part.time.min						= step * 11;
	_part.time.sec						= step * 11;
	_part.speed.number					= step * 11;			/* #21- 速度信息 */
	_part.bat_a.percent					= step * 11;			/* #22- 电池信息 */
	_part.bat_a.level					= step * 1 + 1;
	_part.bat_b.percent					= step * 11;
	_part.bat_b.level					= step * 1 + 1;
	_part.odo_odo.number				= step * 111111.00;		/* #23- 里程信息 */
//	_part.odo_trip.number				= step * 11111.11;
}
	
/* lcd_display_ready_task */
void lcd_display_ready_task(void)
{
	
	if(thissystem.IsOpenAnimationFinished == 1){
		if(thissystem.poweron_5s == 0){
			if(pwron5s_cnt>10){thissystem.poweron_5s = 1;}
		}
	}else{
		pwron5s_cnt = 0;
	}
	/*=================================================================*/
	/* #1 - 左转向灯 */
	_seg.bits.P_T43		= _part.flags.TurnLeftFlag;
	/* #2 - 远光灯 */
	_seg.bits.P_T46		= _part.flags.HighBeamFlag;
	/* #3 - 示宽灯 */
	_seg.bits.P_T44	= _part.flags.WidthLampFlag;
//	_seg.bits.P_S3_1	= _seg.bits.P_S3_0;
	/* #4 - 电源状态灯 */
	_seg.bits.P_T42	= _part.flags.SysRunFlag;
	/* #5 - 近光灯 */
	_seg.bits.P_T40		= _part.flags.LowBeamFlag;
	/* #6 - 欠压灯 */
	_seg.bits.P_T38		= _part.flags.UnderVolWarningFlag;
	/* #7 - 右转向灯 */
	_seg.bits.P_T41		= _part.flags.TurnRightFlag;
	/* #8 - 巡航指示灯 */
	_seg.bits.P_T26		= _part.flags.CruiseRunFalg;
	/* #9 - 刹把故障 */
	_seg.bits.P_T8		= _part.flags.BrakeFaultFlag;
	/* #10- 控制器故障 */
	_seg.bits.P_T7		= _part.flags.ControllerFaultFlag;
	/* #11- 单撑断电功能状态 */
	_seg.bits.P_T6		= _part.flags.SideshoringOffFlag;
	/* #12- 电机故障 */
	_seg.bits.P_T5		= _part.flags.MotorFaultFlag;
	/* #13- 转把故障 */
	_seg.bits.P_T4		= _part.flags.HandleBarFaultFlag;
	/* #14- 倒挡状态 */
	/* 新款没有倒挡图标 */
//	_seg.bits.P_S21_0	= _part.flags.ReverseShiftFlag;
//	_seg.bits.P_S21_1	= _seg.bits.P_S21_0;
	/* #15- UNKNOWN1 */
//	_seg.bits.P_T26		= _part.flags.Unknown1Flag;
	/* #16- UNKNOWN2 */
//	_seg.bits.P_T23	= _part.flags.Unknown2Flag;
//	_seg.bits.P_T24	= _part.flags.Unknown2Flag;
	/* #17- UNKNOWN3 */
	/*	新款没有充电图标	*/
	_seg.bits.P_T24		= _part.flags.charge;
	_seg.bits.P_T27   = _part.flags.TirePressure;
	_seg.bits.P_T45   = _part.flags.BlueTooth;
	_seg.bits.P_T25   = _part.flags.TCS;

	/*=================================================================*/
	/* #18- 温度信息 */
	lcd_display_tempture(_part.tempture);
	
	/* #19- 档位信息 */
	lcd_display_shift(_part.shift);
	
	/* #20- 时间信息 */
	lcd_display_time(_part.time);
	
	/* #21- 速度信息 */
	lcd_display_speed(_part.speed);
	
	/* #22- 电池信息 */
	lcd_display_battery(_part.bat_a);
//	lcd_display_battery(_part.bat_b);
	
	/* #23- 里程信息 */
	lcd_display_odograph(_part);
//	lcd_display_odograph(_part.odo_trip);
	
	/*=================================================================*/
}

/* lcd_display_go_task */
void lcd_display_go_task(void)
{	
	SC5037_ShowRectangeEx(0, 1, 0, 319, &_seg.flags[0], 80);
}
