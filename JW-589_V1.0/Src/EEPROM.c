#include "fm33lg0xx_fl_flash.h"
#include "fm33lg0xx_fl_cmu.h"
#include "fm33lg0xx_fl_dma.h"
#include "fm33_assert.h"
#include "eeprom.h"
#include "user_init.h"
#include "lcd.h"
#include "SystemRunning.h"
#include "config.h"
#include "main.h"
#include "float.h"



uint32_t WritePara[PARALENGTH];
uint32_t ReadPara[PARALENGTH];
/*******************************************************************************
**�������ƣ�void EEPROM_Byte_Write(unsigned int address , unsigned char date)
**������������EEPROM�й̶���ַд��һ���ֽ�����
**��ڲ�����unsigned int address , unsigned char date
            address  ��Ҫд�����ݵĴ洢��ַ
              date   ��һ���ֽ�����
**�������
*******************************************************************************/
FL_ErrorStatus FLASH_Program_Para(FLASH_Type* FLASHx, uint32_t Address, uint32_t *data,uint32_t Length)
{
  uint32_t i;
	FL_ErrorStatus ret;
	FL_FLASH_PageErase(FLASH,Address);
	for(i=0; i<Length; i++)
	{
		ret = FL_FLASH_Program_Word(FLASH, Address + i * 4, data[i]);
		if(ret == FL_FAIL)
			return FL_FAIL;
	}
  return FL_PASS;
}
//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
unsigned short ReadHalfWord(uint32_t faddr)
{
	return *(unsigned short*)faddr; 
}
void FLASH_Read_Para(uint32_t Address, uint32_t *data,uint32_t Length)
{
	unsigned char i;
	for(i=0;i<Length;i++){
				data[i]=ReadHalfWord(Address);//��ȡ2���ֽ�.
		Address+=4;//ƫ��2���ֽ�.
	}
}










//thissystem.odo_number_km = 0;
MYGAIN   GAIN;
void Write(void)
{
	WritePara[0] = thissystem.IsUseMPH;
	GAIN.result = thissystem.odo_number_km;
	WritePara[1] = GAIN.FLOAT_CV[0];
	WritePara[2] = GAIN.FLOAT_CV[1];
	WritePara[3] = GAIN.FLOAT_CV[2];
	WritePara[4] = GAIN.FLOAT_CV[3];	
  FLASH_Program_Para(FLASH,EEPROM_ADDR,WritePara,5);
}

void Read(void)
{
  FLASH_Read_Para(EEPROM_ADDR,ReadPara,5);
	
	thissystem.IsUseMPH = ReadPara[0];
	GAIN.FLOAT_CV[0]=ReadPara[1];	
  GAIN.FLOAT_CV[1]=ReadPara[2];	
  GAIN.FLOAT_CV[2]=ReadPara[3];	
  GAIN.FLOAT_CV[3]=ReadPara[4];
  thissystem.odo_number_km = GAIN.result;

	thissystem.odo_number = thissystem.odo_number_km;	
//	printf("%d   %f  \r\n",thissystem.IsUseMPH,thissystem.odo_number);	
}
