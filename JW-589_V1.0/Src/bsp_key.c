#include "bsp_key.h"
#include "fm33lg0xx.h"
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
#include "can.h"
#include "LCD00_Y094889VTTFGS.h"
#include "stdio.h"


#define HARD_KEY_NUM	   7	   				      /* 实体按键个数 */
#define KEY_COUNT   	 	(HARD_KEY_NUM + 0)	/* 7个独立建 + 0个组合按键 */

/* 依次定义GPIO */
typedef struct
{
	GPIO_Type* gpio;
	uint16_t pin;
	uint8_t ActiveLevel;	/* 激活电平 这里低电平有效按下*/
}X_GPIO_T;

/* GPIO和PIN定义 */
static const X_GPIO_T s_gpio_list[HARD_KEY_NUM] = {
	{GPIOC, FL_GPIO_PIN_0, 1},	 /*位置 PC0    */
	{GPIOC, FL_GPIO_PIN_1, 1},	 /*备用 PC1    */
	{GPIOC, FL_GPIO_PIN_6, 0},	 /*按键 PC6    */
	{GPIOB, FL_GPIO_PIN_10, 1},	 /*近灯 PB10   */
	{GPIOB, FL_GPIO_PIN_11, 1},	 /*右灯 PB11 	 */	
	{GPIOB, FL_GPIO_PIN_12, 1},	 /*左灯 PB12 	 */
	{GPIOB, FL_GPIO_PIN_9,  1},	 /*远灯 PB9 	 */	
};	

/* 定义一个宏函数简化后续代码 
	判断GPIO引脚是否有效按下
*/
static KEY_T s_tBtn[KEY_COUNT] = {0};
static KEY_FIFO_T s_tKey;		/* 按键FIFO变量,结构体 */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(void);
//static void bsp_DetectKey(uint8_t i);

#define KEY_PIN_ACTIVE(id)	

/*
*********************************************************************************************************
*	函 数 名: KeyPinActive
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
static uint8_t KeyPinActive(uint8_t _id)
{
	uint8_t level;
	
	if ((s_gpio_list[_id].gpio->DIN & s_gpio_list[_id].pin) == 0)
	{
		level = 0;
	}
	else
	{
		level = 1;
	}

	if (level == s_gpio_list[_id].ActiveLevel)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: IsKeyDownFunc
*	功能说明: 判断按键是否按下。单键和组合键区分。单键事件不允许有其他键按下。
*	形    参: 无
*	返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
uint8_t IsKeyDownFunc(void)
{
	uint8_t save  = 0;
  uint8_t i;
	for(i = 0;i<HARD_KEY_NUM;i++)
	{
		if (KeyPinActive(i)){
			save |= (1<<i);
		 }
	 }
	return save;
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* 初始化按键变量 */
	bsp_InitKeyHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyHard
*	功能说明: 配置按键对应的GPIO
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{	
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t i;

	
	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStruct.mode       = FL_GPIO_MODE_INPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull       = FL_ENABLE;
	GPIO_InitStruct.remapPin   = FL_DISABLE;	
	for (i = 0; i < HARD_KEY_NUM; i++)
	{
		GPIO_InitStruct.pin = s_gpio_list[i].pin;
		FL_GPIO_Init(s_gpio_list[i].gpio, &GPIO_InitStruct);	
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			  /* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							        /* 按键缺省状态，0为未按下 */
		s_tBtn[i].RepeatSpeed = 0;						    /* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						    /* 连发计数器 */
	}

}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参: 无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	static uint8_t ret = KEY_NONE;
	if (s_tKey.Read == s_tKey.Write)
	{
		return ret;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键的状态
*	形    参:  _ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
//{
//	return s_tBtn[_ucKeyID].State;
//}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参：_ucKeyID : 按键ID，从0开始
*			_LongTime : 长按事件时间
*			 _RepeatSpeed : 连发速度
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* 按键连发的速度，0表示不支持连发 */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* 连发计数器 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参: IO的id， 从0开始编码
*	返 回 值: 无
*********************************************************************************************************
*/
//static void bsp_DetectKey(uint8_t i)
//{
//	KEY_T *pBtn;

//	pBtn = &s_tBtn[i];
//	if (IsKeyDownFunc(i))
//	{
//		if (pBtn->Count < KEY_FILTER_TIME)
//		{
//			pBtn->Count = KEY_FILTER_TIME;
//		}
//		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
//		{
//			pBtn->Count++;
//		}
//		else
//		{
//			if (pBtn->State == 0)
//			{
//				pBtn->State = 1;

//				/* 发送按钮按下的消息 */
//				bsp_PutKey((uint8_t)(3 * i + 1));
//			}

//			if (pBtn->LongTime > 0)
//			{
//				if (pBtn->LongCount < pBtn->LongTime)
//				{
//					/* 发送按钮持续按下的消息 */
//					if (++pBtn->LongCount == pBtn->LongTime)
//					{
//						/* 键值放入按键FIFO */
//						bsp_PutKey((uint8_t)(3 * i + 3));
//					}
//				}
//				else
//				{
//					if (pBtn->RepeatSpeed > 0)
//					{
//						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
//						{
//							pBtn->RepeatCount = 0;
//							/* 常按键后，每隔10ms发送1个按键 */
//							bsp_PutKey((uint8_t)(3 * i + 1));
//						}
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//		if(pBtn->Count > KEY_FILTER_TIME)
//		{
//			pBtn->Count = KEY_FILTER_TIME;
//		}
//		else if(pBtn->Count != 0)
//		{
//			pBtn->Count--;
//		}
//		else
//		{
//			if (pBtn->State == 1)
//			{
//				pBtn->State = 0;

//				/* 发送按钮弹起的消息 */
//				bsp_PutKey((uint8_t)(3 * i + 2));
//			}
//		}

//		pBtn->LongCount = 0;
//		pBtn->RepeatCount = 0;
//	}
//}
/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参: IO的id， 从0开始编码
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectKey(void)
{
	static KEY_T pBtn;

  static uint8_t prekeynum = 0;    //之前GPIO按键输入值
	uint8_t thiskeynum = 0;
	pBtn.IsKeyDownFunc = IsKeyDownFunc;
	thiskeynum = pBtn.IsKeyDownFunc();
	if(thiskeynum != prekeynum){
		if (pBtn.Count < KEY_FILTER_TIME)
		 {
			 pBtn.Count = KEY_FILTER_TIME;
		 }
		else if(pBtn.Count < 2 * KEY_FILTER_TIME)
		 {
			 pBtn.Count++;
		 }
		else
		 {
			 /* 发送按钮按下的消息 */
			 bsp_PutKey(thiskeynum);
			 pBtn.Count = 0;
			 prekeynum = thiskeynum;
		 }
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan10ms
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用，10ms一次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyScan10ms(void)
{
//	uint8_t i;
  bsp_DetectKey();
//	for (i = 0; i < KEY_COUNT; i++)
//	{
//		bsp_DetectFastIO(i);
//	}
}


uint8_t nokey       = 1;

uint8_t get_key(void)
{
	uint8_t  ret = KEY_NONE;
	static uint32_t keycnt = 0;
  uint8_t ucKeyCode = KEY_NONE;		/* 按键代码 */
	static uint8_t preKeyCode = KEY_NONE;
	ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */

	if(preKeyCode != ucKeyCode){
		keycnt = 0;
		preKeyCode = ucKeyCode;
	}else{
		keycnt ++;
		if(keycnt>50){keycnt = 50;preKeyCode = ucKeyCode; ret= ucKeyCode; }
		else         {                                    ret= preKeyCode;}
	}
	return ret;
}	

