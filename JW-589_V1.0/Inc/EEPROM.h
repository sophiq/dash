#include <stdint.h>
#include "main.h"
#define  EEPROM_ADDR         0x0001C000

#define  PARALENGTH                  40

typedef union           //������������  FLOAT_CV[]��result�����ڴ棬ʵ������һ��������
{
  unsigned char  FLOAT_CV[4];
  float          result;
}MYGAIN;




FL_ErrorStatus FLASH_Program_Para(FLASH_Type* FLASHx, uint32_t Address, uint32_t *data,uint32_t Length);
void FLASH_Read_Para(uint32_t Address, uint32_t *data,uint32_t Length);
void Write(void);
void Read(void);
