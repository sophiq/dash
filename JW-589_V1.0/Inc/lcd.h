#ifndef __LCD_H__
#define __LCD_H__
#include "fm33lg0xx.h"
#include "main.h"


#define  START_LCD    0
#define  FULL_LCD     1
#define  TASK_LCD     2

/*!<*/
typedef struct{
	uint8_t shiwan	:4;
	uint8_t wan		:4;
	uint8_t qian	:4;
	uint8_t bai		:4;
	uint8_t shi		:4;
	uint8_t ge		:4;
	uint8_t shifen	:4;
	uint8_t baifen	:4;
}floatinfo_t;

/*!-----------------------------------------------------------------
 */
enum{
	E_DUTY_1DIV1,
	E_DUTY_1DIV2,
	E_DUTY_1DIV3,
	E_DUTY_1DIV4,
};

enum{
	E_FREQ_50HZ = (0x10 | 0x00),
	E_FREQ_60HZ,
	E_FREQ_70HZ,
	E_FREQ_75HZ,
	E_FREQ_80HZ,
	E_FREQ_85HZ,
	E_FREQ_90HZ,
	E_FREQ_100HZ,
	E_FREQ_105HZ,
	E_FREQ_110HZ,
	E_FREQ_115HZ,
	E_FREQ_120HZ,
	E_FREQ_130HZ,
	E_FREQ_140HZ,
	E_FREQ_150HZ,
	E_FREQ_RESERVED,
};

enum{
	E_SCAN_COM0_3_SEG0_319 = 0x00,
	E_SCAN_COM0_3_SEG319_0 = 0x40,
	E_SCAN_COM3_0_SEG0_319 = 0x80,
	E_SCAN_COM3_0_SEG319_0 = 0xC0,
};

enum{
	E_BIAS_1DIV2,
	E_BIAS_1DIV3,
};

enum{
	E_COM_0,
	E_COM_1,
	E_COM_2,
	E_COM_3,
};

/*!-----------------------------------------------------------------
 */
#define COM_NUMBER			2
#define SEG_NUMBER			320
#define X_POSITION_NUMBER	4
#define Y_POSITION_NUMBER	160
#define V_DISP_ON_H			0x70
#define V_DISP_ON_L			0x07
#define V_DISP_OFF_H		0x00
#define V_DISP_OFF_L		0x00

/*!-----------------------------------------------------------------
 */
#define COM(x)				(x)				// COMx(x = 0...3)
#define SEG(x)				(x)				// SEGx(x = 0...319)
#define SEG2YPOS(x)			((x) >> 1)		// [seg1, seg0] = [2x+1, 2x]
#define SEG2BPOS(x)			((x) %  2)		// [H   , L   ] = [1   , 0 ]

bool GetBit(uint8_t n, uint8_t k);
void LcdConfig(void);
void SC5037_ClearAllPoints(void);
void SC5037_DisplayAll(uint8_t on);
void SC5037_ShowRectangeEx(uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2, uint8_t* pflags, uint8_t len);
bool GetBit(uint8_t n, uint8_t k);
void dsp(void);
void init(void);
#endif
