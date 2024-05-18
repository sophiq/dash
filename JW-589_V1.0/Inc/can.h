#ifndef __CAN_H__
#define __CAN_H__

#include "main.h"


#define FAST_RECOVER_TIMEOUT    50   /* 快恢复时间 50ms */
#define SLOW_RECOVER_TIMEOUT    200  /* 慢恢复时间 200ms */

#define  CALENDAR       0x1FFFC400
#define  MCU_BST_ID     0x1FFFC401
#define  TRIP_ODO       0x1FFFC402
#define  SPEED_SOC      0x1FFFC403
#define  MCU_ECODE_ID   0x1FFFC405
#define  VCU_ICM_ID     0x10040414


#define  MY_ID          0x1FFFD400



extern uint8_t  heartbeat;
extern uint8_t  sendflag;
extern bool  bms_bms2_cale;
extern uint8_t speed;
extern uint8_t  thisshift;
extern uint8_t  thischarge;
extern uint16_t trip;
extern uint16_t odo;
extern uint8_t  undervolflag;
extern uint8_t  clearodo;
extern uint8_t  factorysetting;
extern uint8_t  changeuint;
extern uint8_t  errorcode;

extern uint8_t mcu_die_cnt;
extern uint8_t mcu_die_flag;
extern uint8_t bms_die_cnt;
extern uint8_t bms_die_flag;
extern uint8_t vcu_die_cnt;
extern uint8_t vcu_die_flag;

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}Calendar;

typedef struct
{
    uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

    uint8_t DLC;     /*!< Specifies the length of the frame that will be
                        transmitted. This parameter can be a value between
                        0 to 8 */

    uint8_t Data[8]; /*!< Contains the data to be transmitted. It ranges from 0
                        to 0xFF. */
} CanTxMsg;
extern CanTxMsg TxMessage;               //·¢ËÍ»º³åÇø
extern Calendar MyCale;

typedef struct
{
    uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */
	  uint32_t RxId;   /* 读到数据包发送的ID */

    uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

    uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to
                        0xFF. */
} CanRxMsg;

extern void FL_CAN_FIFO_Write(uint32_t id, uint32_t len, uint32_t data1, uint32_t data2);
extern void FL_CAN_HPBUF_Write(uint32_t id, uint32_t len, uint32_t data1, uint32_t data2);
void FL_CAN_FIFO_Read(void);
void CAN_SetMsg(CanTxMsg *TxMessage);
void Can_Init(void);
void FL_CAN_Frame_Send(uint32_t frame_format, CanTxMsg *TxMessage);
void can_reset_with_send(void);
#endif

