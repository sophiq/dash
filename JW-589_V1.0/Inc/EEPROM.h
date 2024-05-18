#include <stdint.h>
#include "main.h"
#define  EEPROM_ADDR         0x0001C000

#define  PARALENGTH                  40

typedef union           //浮点数联合体  FLOAT_CV[]与result共享内存，实际上是一样的内容
{
  unsigned char  FLOAT_CV[4];
  float          result;
}MYGAIN;




FL_ErrorStatus FLASH_Program_Para(FLASH_Type* FLASHx, uint32_t Address, uint32_t *data,uint32_t Length);
void FLASH_Read_Para(uint32_t Address, uint32_t *data,uint32_t Length);
void Write(void);
void Read(void);
