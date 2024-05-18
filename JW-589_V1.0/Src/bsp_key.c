#include "bsp_key.h"
#include "fm33lg0xx.h"
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
#include "can.h"
#include "LCD00_Y094889VTTFGS.h"
#include "stdio.h"


#define HARD_KEY_NUM	   7	   				      /* ʵ�尴������ */
#define KEY_COUNT   	 	(HARD_KEY_NUM + 0)	/* 7�������� + 0����ϰ��� */

/* ���ζ���GPIO */
typedef struct
{
	GPIO_Type* gpio;
	uint16_t pin;
	uint8_t ActiveLevel;	/* �����ƽ ����͵�ƽ��Ч����*/
}X_GPIO_T;

/* GPIO��PIN���� */
static const X_GPIO_T s_gpio_list[HARD_KEY_NUM] = {
	{GPIOC, FL_GPIO_PIN_0, 1},	 /*λ�� PC0    */
	{GPIOC, FL_GPIO_PIN_1, 1},	 /*���� PC1    */
	{GPIOC, FL_GPIO_PIN_6, 0},	 /*���� PC6    */
	{GPIOB, FL_GPIO_PIN_10, 1},	 /*���� PB10   */
	{GPIOB, FL_GPIO_PIN_11, 1},	 /*�ҵ� PB11 	 */	
	{GPIOB, FL_GPIO_PIN_12, 1},	 /*��� PB12 	 */
	{GPIOB, FL_GPIO_PIN_9,  1},	 /*Զ�� PB9 	 */	
};	

/* ����һ���꺯���򻯺������� 
	�ж�GPIO�����Ƿ���Ч����
*/
static KEY_T s_tBtn[KEY_COUNT] = {0};
static KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(void);
//static void bsp_DetectKey(uint8_t i);

#define KEY_PIN_ACTIVE(id)	

/*
*********************************************************************************************************
*	�� �� ��: KeyPinActive
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
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
*	�� �� ��: IsKeyDownFunc
*	����˵��: �жϰ����Ƿ��¡���������ϼ����֡������¼������������������¡�
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
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
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* ��ʼ���������� */
	bsp_InitKeyHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyHard
*	����˵��: ���ð�����Ӧ��GPIO
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{	
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t i;

	
	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
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
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			  /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State = 0;							        /* ����ȱʡ״̬��0Ϊδ���� */
		s_tBtn[i].RepeatSpeed = 0;						    /* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						    /* ���������� */
	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��: ��
*	�� �� ֵ: ��������
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
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
//{
//	return s_tBtn[_ucKeyID].State;
//}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��: IO��id�� ��0��ʼ����
*	�� �� ֵ: ��
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

//				/* ���Ͱ�ť���µ���Ϣ */
//				bsp_PutKey((uint8_t)(3 * i + 1));
//			}

//			if (pBtn->LongTime > 0)
//			{
//				if (pBtn->LongCount < pBtn->LongTime)
//				{
//					/* ���Ͱ�ť�������µ���Ϣ */
//					if (++pBtn->LongCount == pBtn->LongTime)
//					{
//						/* ��ֵ���밴��FIFO */
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
//							/* ��������ÿ��10ms����1������ */
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

//				/* ���Ͱ�ť�������Ϣ */
//				bsp_PutKey((uint8_t)(3 * i + 2));
//			}
//		}

//		pBtn->LongCount = 0;
//		pBtn->RepeatCount = 0;
//	}
//}
/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��: IO��id�� ��0��ʼ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(void)
{
	static KEY_T pBtn;

  static uint8_t prekeynum = 0;    //֮ǰGPIO��������ֵ
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
			 /* ���Ͱ�ť���µ���Ϣ */
			 bsp_PutKey(thiskeynum);
			 pBtn.Count = 0;
			 prekeynum = thiskeynum;
		 }
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan10ms
*	����˵��: ɨ�����а���������������systick�ж������Եĵ��ã�10msһ��
*	��    ��: ��
*	�� �� ֵ: ��
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
  uint8_t ucKeyCode = KEY_NONE;		/* �������� */
	static uint8_t preKeyCode = KEY_NONE;
	ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */

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

