#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "fm33lg0xx.h"

/* 根据应用程序的功能重命名按键宏 */

#define KEY_POSITION     0
#define KEY_STANDBY      1
#define KEY_PRESS        2
#define KEY_FARLIGHT     3
#define KEY_RIGHT        4
#define KEY_LEFT         5



#define KEY_FILTER_TIME   16
#define KEY_LONG_TIME     100			/* 单位10ms， 持续1秒，认为长按事件 */


typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			  /* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			  /* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */
	KEY_1_DOWN,				/* 1键按下 */
	KEY_1_UP,				/* 1键弹起 */
	KEY_1_LONG,				/* 1键长按 */

	KEY_2_DOWN,				/* 2键按下 */
	KEY_2_UP,				/* 2键弹起 */
	KEY_2_LONG,				/* 2键长按 */

	KEY_3_DOWN,				/* 3键按下 */
	KEY_3_UP,				/* 3键弹起 */
	KEY_3_LONG,				/* 3键长按 */

	KEY_4_DOWN,				/* 4键按下 */
	KEY_4_UP,				/* 4键弹起 */
	KEY_4_LONG,				/* 4键长按 */

	KEY_5_DOWN,				/* 3键按下 */
	KEY_5_UP,				/* 3键弹起 */
	KEY_5_LONG,				/* 3键长按 */

	KEY_6_DOWN,				/* 4键按下 */
	KEY_6_UP,				/* 4键弹起 */
	KEY_6_LONG,				/* 4键长按 */
	
	KEY_7_DOWN,				/* 4键按下 */
	KEY_7_UP,				/* 4键弹起 */
	KEY_7_LONG,				/* 4键长按 */	
	
	
}KEY_ENUM;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					        /* 缓冲区读指针1 */
	uint8_t Write;					      /* 缓冲区写指针 */
	uint8_t Read2;					      /* 缓冲区读指针2 */
}KEY_FIFO_T;

/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void bsp_KeyScan10ms(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);
uint8_t get_key(void);
#endif

