#include "can.h"
#include "user_init.h"
#include "LCD00_Y094889VTTFGS.h"
#include "lcd.h"
#include "gptim.h"
#include "SystemRunning.h"

Calendar MyCale = {0};
CanTxMsg TxMessage = {0};                //发送缓冲区
CanRxMsg RxMessage = {0};                //接收缓冲区
uint8_t heartbeat = 0;
uint8_t sendflag = 0;
bool    bms_bms2_cale = 0;               //0 - 接收BMS     1 - 接收BMS2
uint8_t undervolflag = 0;                //低电压报警

static void MF_CAN_Init(void)
{
	FL_GPIO_InitTypeDef      GPIO_InitStruct = {0};
	FL_CAN_InitTypeDef       CAN_InitStructure = {0};
//	FL_CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};

	GPIO_InitStruct.pin = FL_GPIO_PIN_6|FL_GPIO_PIN_7;
	GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	GPIO_InitStruct.analogSwitch = FL_DISABLE;
	FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*----------------------------CAN结构体初始化--------------------------------------*/
	//波特率设置= CAN_CLK/(BRP+1)/(TS1_Tq+TS2_Tq+1);  8M/(7+1)/(5+4+1)=100K
	CAN_InitStructure.TS1 = FL_CAN_TS1_4Tq;
	CAN_InitStructure.TS2 = FL_CAN_TS2_3Tq;       //位时序设置
	CAN_InitStructure.SJW = FL_CAN_SJW_2Tq;
	CAN_InitStructure.BRP = 1;                    //波特率预分频
	CAN_InitStructure.mode = FL_CAN_MODE_NORMAL;  //工作模式设置
	CAN_InitStructure.clockSource = FL_CMU_CAN_CLK_SOURCE_XTHF; //时钟源设置
	FL_CAN_Init(CAN, &CAN_InitStructure);
	/*----------------------------接收滤波结构体初始化--------------------------------------*/
//	CAN_FilterInitStructure.filterIdExtend = VCU_ICM_ID; //扩展ID
//	CAN_FilterInitStructure.filterIdSRR = 0X01;
//	CAN_FilterInitStructure.filterIdIDE = 0X01;
//	CAN_FilterInitStructure.filterIdRTR = 0X00;

//	CAN_FilterInitStructure.filterMaskIdHigh = 0X7FF;
//	CAN_FilterInitStructure.filterMaskIdLow = 0X3FFFF;
//	CAN_FilterInitStructure.filterMaskIdSRR = 0X01;
//	CAN_FilterInitStructure.filterMaskIdIDE = 0X01;  //滤波器掩码,1：该位参与滤波器比较，0：不参与
//	CAN_FilterInitStructure.filterMaskIdRTR = 0x01;
//	CAN_FilterInitStructure.filterEn = FL_ENABLE;
//	FL_CAN_FilterInit(CAN, &CAN_FilterInitStructure, FL_CAN_FILTER1);

//	CAN_FilterInitStructure.filterIdExtend = MCU_BST_ID; //扩展ID
//	CAN_FilterInitStructure.filterIdSRR = 0X01;
//	CAN_FilterInitStructure.filterIdIDE = 0X01;
//	CAN_FilterInitStructure.filterIdRTR = 0X00;

//	CAN_FilterInitStructure.filterMaskIdHigh = 0X7FF;
//	CAN_FilterInitStructure.filterMaskIdLow = 0X3FFFF;
//	CAN_FilterInitStructure.filterMaskIdSRR = 0X01;
//	CAN_FilterInitStructure.filterMaskIdIDE = 0X01;  //滤波器掩码,1：该位参与滤波器比较，0：不参与
//	CAN_FilterInitStructure.filterMaskIdRTR = 0x01;
//	CAN_FilterInitStructure.filterEn = FL_ENABLE;
//	FL_CAN_FilterInit(CAN, &CAN_FilterInitStructure, FL_CAN_FILTER2);
////	
//	CAN_FilterInitStructure.filterIdExtend = MCU_ECODE_ID; //扩展ID
//	CAN_FilterInitStructure.filterIdSRR = 0X01;
//	CAN_FilterInitStructure.filterIdIDE = 0X01;
//	CAN_FilterInitStructure.filterIdRTR = 0X00;

//	CAN_FilterInitStructure.filterMaskIdHigh = 0X7FF;
//	CAN_FilterInitStructure.filterMaskIdLow = 0X3FFFF;
//	CAN_FilterInitStructure.filterMaskIdSRR = 0X01;
//	CAN_FilterInitStructure.filterMaskIdIDE = 0X01;  //滤波器掩码,1：该位参与滤波器比较，0：不参与
//	CAN_FilterInitStructure.filterMaskIdRTR = 0x01;
//	CAN_FilterInitStructure.filterEn = FL_ENABLE;
//	FL_CAN_FilterInit(CAN, &CAN_FilterInitStructure, FL_CAN_FILTER3);

//	CAN_FilterInitStructure.filterIdExtend = CALENDAR; //扩展ID
//	CAN_FilterInitStructure.filterIdSRR = 0X01;
//	CAN_FilterInitStructure.filterIdIDE = 0X01;
//	CAN_FilterInitStructure.filterIdRTR = 0X00;

//	CAN_FilterInitStructure.filterMaskIdHigh = 0X7FF;
//	CAN_FilterInitStructure.filterMaskIdLow = 0X3FFFF;
//	CAN_FilterInitStructure.filterMaskIdSRR = 0X01;
//	CAN_FilterInitStructure.filterMaskIdIDE = 0X01;  //滤波器掩码,1：该位参与滤波器比较，0：不参与
//	CAN_FilterInitStructure.filterMaskIdRTR = 0x01;
//	CAN_FilterInitStructure.filterEn = FL_ENABLE;
//	FL_CAN_FilterInit(CAN, &CAN_FilterInitStructure, FL_CAN_FILTER4);

  FL_CAN_ClearFlag_CRXOK(CAN);
	FL_CAN_EnableIT_RXOK(CAN);      //接收中断使能

	NVIC_DisableIRQ(CAN_IRQn);
	NVIC_SetPriority(CAN_IRQn, 1); //中断优先级配置
	NVIC_EnableIRQ(CAN_IRQn);
}




void Can_Init(void)
{
	/* Initial CAN */
	MF_CAN_Init();
}


/**
  * @brief  通过TX FIFO发送数据
  * @param  CANx
  * @retval  None
  */

void FL_CAN_FIFO_Write(uint32_t id, uint32_t len, uint32_t data1, uint32_t data2)
{
    uint32_t i = 5;

    while((FL_CAN_IsActiveFlag_TXBuffFull(CAN) != FL_RESET)&i)
    {
        i--;
        FL_DelayMs(1);
    }

    FL_CAN_WriteTXMessageID(CAN, id);
    FL_CAN_WriteTXMessageLength(CAN, len);
    FL_CAN_WriteTXMessageWord1(CAN, data1);
    FL_CAN_WriteTXMessageWord2(CAN, data2);
}

/**
  * @brief  通过HPB发送数据
  * @param  CANx
  * @retval  None
  */

void FL_CAN_HPBUF_Write(uint32_t id, uint32_t len, uint32_t data1, uint32_t data2)
{
    uint32_t i = 5;

    while((FL_CAN_IsActiveFlag_TXHighPriorBuffFull(CAN) != FL_RESET)&i)
    {
        i--;
        FL_DelayMs(1);
    }

    FL_CAN_WriteHighPriorTXMessageID(CAN, id);
    FL_CAN_WriteHighPriorMessageLength(CAN, len);
    FL_CAN_WriteHighPriorMessageWord1(CAN, data1);
    FL_CAN_WriteHighPriorMessageWord2(CAN, data2);
}


uint8_t Data[8] = {0x00}; //要发送的数据
/**
  * @brief  发送数据设置
  * @param  CANx
  * @retval  None
  */
void CAN_SetMsg(CanTxMsg *TxMessage)
{
    uint8_t ubCounter = 0;
    memset(Data,0,8);
    TxMessage->ExtId = MY_ID;                   //使用扩展ID
    TxMessage->DLC = 8;                          //数据长度为8字节
    Data[0] = (lightlevel<<4)|(thissystem.IsUseMPH);
    Data[7] = heartbeat;
    /*设置要发送的数据0-7*/
    for(ubCounter = 0; ubCounter < 8; ubCounter++)
    {
        TxMessage->Data[ubCounter] = Data[ubCounter];
    }

}

/**
  * @brief  读RX FIFO数据
  * @param  CANx
  * @retval  None
  */
uint32_t id = 0;
uint32_t len = 0;
uint32_t data1 = 0;
uint32_t data2 = 0;

uint8_t  speed = 0;
uint8_t  thisshift = 0;
uint8_t  thischarge = 0;
uint16_t trip  = 0;
uint16_t odo   = 0;
uint8_t  gearposition = 0;
uint8_t  clearodo = 0;
uint8_t  factorysetting = 0;
uint8_t  changeuint = 0;
uint8_t  errorcode = 0;

uint8_t mcu_die_cnt = 0;
uint8_t mcu_die_flag = 0;
uint8_t bms_die_cnt = 0;
uint8_t bms_die_flag = 0;
uint8_t vcu_die_cnt = 0;
uint8_t vcu_die_flag = 0;
uint8_t havenewtrip = 0;
void FL_CAN_FIFO_Read(void)
{

  static uint8_t pre_mcubeat = 0;
  static uint8_t pre_bmsbeat = 0;
  static uint8_t pre_vcubeat = 0;	 //时间
	uint32_t data1 = 0;
	uint32_t data2 = 0;

	RxMessage.ExtId = FL_CAN_ReadRXMessageID(CAN);
	RxMessage.DLC = FL_CAN_ReadRXMessageLength(CAN);

	data1 = FL_CAN_ReadRXMessageWord1(CAN);
	data2 = FL_CAN_ReadRXMessageWord2(CAN);
	RxMessage.RxId = ((RxMessage.ExtId & 0x7FF) << 18) + ((RxMessage.ExtId >> 13) & 0x3FFFF);
	RxMessage.Data[0] = (uint8_t)data1 & 0xff;
	RxMessage.Data[1] = (uint8_t)(data1 >> 8) & 0xff;
	RxMessage.Data[2] = (uint8_t)(data1 >> 16) & 0xff;
	RxMessage.Data[3] = (uint8_t)(data1 >> 24) & 0xff;
	RxMessage.Data[4] = (uint8_t)data2 & 0xff;
	RxMessage.Data[5] = (uint8_t)(data2 >> 8) & 0xff;
	RxMessage.Data[6] = (uint8_t)(data2 >> 16) & 0xff;
	RxMessage.Data[7] = (uint8_t)(data2 >> 24) & 0xff;


	switch(RxMessage.RxId){
		/* 0x1FFFC400 OK*/	
		case CALENDAR:
			if(pre_vcubeat!=RxMessage.Data[7]){vcu_die_cnt = 0;vcu_die_flag = 0;mcu_die_cnt = 0;mcu_die_flag = 0;bms_die_cnt = 0;bms_die_flag = 0;}  
		  pre_vcubeat = RxMessage.Data[7];
			thissystem.min = ((RxMessage.Data[5]&0xF0)>>4)+((RxMessage.Data[6]&0x03)<<4);
			if(thissystem.min>59)thissystem.min = 59;
		  thissystem.hour = ((RxMessage.Data[5]&0x0F)<<1)+((RxMessage.Data[4]&0x80)>>7);
			if(thissystem.hour>23)thissystem.hour = 23;
			break;
			
		/* 0x1FFFC401 OK*/
		case MCU_BST_ID:
		  thisshift = RxMessage.Data[4]&0x07;												//挡位
		  if(thisshift == 7)	 {thissystem.ReverseShiftFlag = 1;}
			else                 {thissystem.ReverseShiftFlag = 0;}
			
			thischarge = RxMessage.Data[5]&0x03;
//		  if(GetBit(RxMessage.Data[5],1)==1){thissystem.charge = 1;}
//			else                 {thissystem.charge = 0;}
		  if(GetBit(RxMessage.Data[3],2)==1){thissystem.SideshoringOffFlag = 1;}
			else                 {thissystem.SideshoringOffFlag = 0;}
		  if(GetBit(RxMessage.Data[4],3)==1){thissystem.CruiseRunFalg = 1;}
			else                 {thissystem.CruiseRunFalg = 0;}
		  if(GetBit(RxMessage.Data[3],5)==1){thissystem.TCS = 1;}
			else                 {thissystem.TCS = 0;}
		  if(GetBit(RxMessage.Data[5],2)==1){thissystem.BlueTooth = 1;}
			else                 {thissystem.BlueTooth = 0;}			
			if(GetBit(RxMessage.Data[5],4)==1){thissystem.SysRunFlag = 1;}
			else                 {thissystem.SysRunFlag = 0;}
			break;			
			
		/* 0x1FFFC402 OK*/
		case TRIP_ODO:
				trip  = ((uint16_t)RxMessage.Data[1]<<8) + RxMessage.Data[0];
				thissystem.odo_number_km = (float)(RxMessage.Data[4] + ((uint32_t)RxMessage.Data[5]<<8) + 
					                                ((uint32_t)RxMessage.Data[6]<<16) + ((uint32_t)RxMessage.Data[7]<<24));
			break;

		/* 0x1FFFC403 OK*/
		case SPEED_SOC:
			  speed = RxMessage.Data[1];
        if(speed >99)speed =99;
			  thissystem.battery_a = RxMessage.Data[0];
		    if(thissystem.battery_a>100)thissystem.battery_a = 100;		
			break;

		/* 0x1FFFC405 OK*/
		case MCU_ECODE_ID:
				errorcode = RxMessage.Data[7];
		    undervolflag = GetBit(RxMessage.Data[1],3);
				thissystem.MotorFaultFlag  = GetBit(RxMessage.Data[0],0);		
				thissystem.ControllerFaultFlag = GetBit(RxMessage.Data[0],1);
				thissystem.s_icona_flag = GetBit(RxMessage.Data[0],2);
				thissystem.HandleBarFaultFlag = GetBit(RxMessage.Data[0],3);
				thissystem.BrakeFaultFlag  = GetBit(RxMessage.Data[0],6);
				thissystem.TirePressure = GetBit(RxMessage.Data[0],5);
			break;
		
		/* 0x10040414 OK*/
		case VCU_ICM_ID:
				if((RxMessage.Data[0]&0x02)==0x02){
					if(thissystem.IsUseMPH != (RxMessage.Data[0]&0x01)){
						changeuint = 1;
						thissystem.IsUseMPH = RxMessage.Data[0]&0x01;
					}
				}
				if((RxMessage.Data[2]&0x03)==0x03){
					if(thissystem.odo_number_km != 0) clearodo = 1;
				}
				if((RxMessage.Data[2]&0x0C)==0x0C){
					if((thissystem.odo_number_km != 0)&&(thissystem.IsUseMPH != 1)) factorysetting = 1;
				}
		  break;
	}
}

/**
  * @brief  帧模式选择
  * @param  CANx
  * @retval  None
  */
void FL_CAN_Frame_Send(uint32_t frame_format, CanTxMsg *TxMessage)
{

    uint32_t id = 0;
    uint32_t len = 0;
    uint32_t data1 = 0;
    uint32_t data2 = 0;

    if(frame_format == FL_CAN_FORMAT_STANDARD_DATA) //标准数据帧
    {
        id = TxMessage->StdId & 0x7ff;
    }

    else
        if(frame_format == FL_CAN_FORMAT_STANDARD_REMOTE) //标准远程帧
        {
            id = (TxMessage->StdId & 0x7ff) | (1 << 11);
        }

        else
            if(frame_format == FL_CAN_FORMAT_EXTEND_DATA) //扩展数据帧
            {
                id = ((TxMessage->ExtId & 0x3ffff) << 13) | ((uint32_t)1 << 12) | ((uint32_t)1 << 11) | ((TxMessage->ExtId & 0x1ffc0000) >> 18);
            }

            else
                if(frame_format == FL_CAN_FORMAT_EXTEND_REMOTE) //扩展远程帧
                {
                    id = ((TxMessage->ExtId & 0x3ffff) << 13) | ((uint32_t)1 << 12) | ((uint32_t)1 << 31) | ((TxMessage->ExtId & 0x1ffc0000) >> 18);
                }


    len = TxMessage->DLC;
    data1 = (((uint32_t)TxMessage->Data[3] << 24) |
             ((uint32_t)TxMessage->Data[2] << 16) |
             ((uint32_t)TxMessage->Data[1] << 8) |
             ((uint32_t)TxMessage->Data[0]));
    data2 = (((uint32_t)TxMessage->Data[7] << 24) |
             ((uint32_t)TxMessage->Data[6] << 16) |
             ((uint32_t)TxMessage->Data[5] << 8) |
             ((uint32_t)TxMessage->Data[4]));

    FL_CAN_FIFO_Write(id, len, data1, data2);
    //    FL_CAN_HPBUF_Write(id,len,data1,data2);    //高优先级FIFO
}

void can_reset_with_send(void)
{
    FL_CAN_InitTypeDef       CAN_InitStructure = {0};
    
    CAN->CR = 0;
    
    /*----------------------------CAN结构体初始化--------------------------------------*/
    //波特率设置= CAN_CLK/(BRP+1)/(TS1_Tq+TS2_Tq+1);  8M/(7+1)/(5+4+1)=100K
    CAN_InitStructure.TS1 = FL_CAN_TS1_11Tq;
    CAN_InitStructure.TS2 = FL_CAN_TS2_4Tq;       //位时序设置
    CAN_InitStructure.SJW = FL_CAN_SJW_3Tq;
    CAN_InitStructure.BRP = 0;                    //波特率预分频
    CAN_InitStructure.mode = FL_CAN_MODE_NORMAL;  //工作模式设置
    CAN_InitStructure.clockSource = FL_CMU_CAN_CLK_SOURCE_XTHF; //时钟源设置
    FL_CAN_Init(CAN, &CAN_InitStructure);

    FL_CAN_ClearFlag_CRXOK(CAN);
    FL_CAN_EnableIT_RXOK(CAN);      //接收中断使能
    
    FL_CAN_ClearFlag_Error(CAN);
    FL_CAN_EnableIT_Error(CAN);     //错误中断使能  
    
    FL_CAN_ClearFlag_BusOff(CAN);   //BusOff中断使能
    FL_CAN_EnableIT_BusOff(CAN);
    
    FL_CAN_ClearFlag_TXOK(CAN);
    FL_CAN_EnableIT_TXOK(CAN);      //发送完成中断使能    
    
    /* 设置要发送的数据 */
    CAN_SetMsg(&TxMessage);  
   /* 发送数据 */
    FL_CAN_Frame_Send(FL_CAN_FORMAT_EXTEND_DATA, &TxMessage);   
}





void CAN_IRQHandler(void)            //中断接收
{
    if((FL_ENABLE == FL_CAN_IsEnabledIT_RXOK(CAN))
            && (FL_SET == FL_CAN_IsActiveFlag_RXOK(CAN)))
    {
        FL_CAN_FIFO_Read();
        FL_CAN_ClearFlag_CRXOK(CAN);
    }

}
