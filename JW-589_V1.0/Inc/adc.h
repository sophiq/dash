/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : adc.h
  * @brief          : Header for adc.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 FMSH.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by FM under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */

extern uint16_t  ADC_ConvertedValue[4];
extern uint8_t ADCComplete;

/* Exported functions prototypes ---------------------------------------------*/
extern void Error_Handler(void);
extern void AdcInit(void);
extern uint32_t GetVREF1P2Sample(void);
extern uint32_t GetDifferentialChannelSample(void);
extern uint32_t GetSingleChannelSample(void);
extern void GetDMAChannelSample(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
