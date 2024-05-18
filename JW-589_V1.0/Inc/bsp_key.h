#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "fm33lg0xx.h"

/* ����Ӧ�ó���Ĺ��������������� */

#define KEY_POSITION     0
#define KEY_STANDBY      1
#define KEY_PRESS        2
#define KEY_FARLIGHT     3
#define KEY_RIGHT        4
#define KEY_LEFT         5



#define KEY_FILTER_TIME   16
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */


typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			  /* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			  /* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */
	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				/* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				/* 3������ */
	KEY_3_LONG,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,				/* 4������ */
	KEY_4_LONG,				/* 4������ */

	KEY_5_DOWN,				/* 3������ */
	KEY_5_UP,				/* 3������ */
	KEY_5_LONG,				/* 3������ */

	KEY_6_DOWN,				/* 4������ */
	KEY_6_UP,				/* 4������ */
	KEY_6_LONG,				/* 4������ */
	
	KEY_7_DOWN,				/* 4������ */
	KEY_7_UP,				/* 4������ */
	KEY_7_LONG,				/* 4������ */	
	
	
}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					        /* ��������ָ��1 */
	uint8_t Write;					      /* ������дָ�� */
	uint8_t Read2;					      /* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan10ms(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);
uint8_t get_key(void);
#endif

