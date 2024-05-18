/**************************************************************************//**
 * @file     system_fm33lg0xx.h
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Header File for
 *           Device FM33LG0XX
 * @version  V2.00
 * @date     15. March 2021
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef SYSTEM_FM33LG0XX_H
#define SYSTEM_FM33LG0XX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "fm33lg0xx.h"
            
#define LDT_CHECK(_N_VALUE_, _T_VALUE_)                         \
                            ((((_N_VALUE_ >> 16) & 0xffff) ==   \
                            ((~_N_VALUE_) & 0xffff)) ? _N_VALUE_ : _T_VALUE_)

#define RCHF8M_LDT_TRIM     (*(uint32_t *)0x1FFFFB40)	// RC8M 常温校准值
#define RCHF16M_LDT_TRIM 	(*(uint32_t *)0x1FFFFB3C)	// RC16M 常温校准值
#define RCHF24M_LDT_TRIM 	(*(uint32_t *)0x1FFFFB38)	// RC24M 常温校准值
#define RCHF32M_LDT_TRIM 	(*(uint32_t *)0x1FFFFB34)	// RC32M 常温校准值
#define RCLF_LDT_TRIM 	    (*(uint32_t *)0x1FFFFB44)	// RCLF 常温校准值
#define RCLP_LDT_TRIM 	    (*(uint32_t *)0x1FFFFB20)	// RCLP 常温校准值  
    
#define RCHF8M_TRIM         (LDT_CHECK(RCHF8M_LDT_TRIM, 0x80) & 0xff)
#define RCHF16M_TRIM        (LDT_CHECK(RCHF16M_LDT_TRIM, 0x80) & 0xff)
#define RCHF24M_TRIM        (LDT_CHECK(RCHF24M_LDT_TRIM, 0x80) & 0xff)
#define RCHF32M_TRIM        (LDT_CHECK(RCHF32M_LDT_TRIM, 0x80) & 0xff)
#define RCLF_TRIM           (LDT_CHECK(RCLF_LDT_TRIM, 0x80) & 0xff)
#define RCLP_TRIM           (LDT_CHECK(RCLP_LDT_TRIM, 0x80) & 0xff)


#define ULPBG_LDT_TRIM      (*(uint32_t *)0x1FFFFA98)

#define ULPBG_TRIM          (LDT_CHECK(ULPBG_LDT_TRIM, 0x10) & 0x1f)


#if !defined  (XTHF_VALUE)
    #define XTHF_VALUE    ((uint32_t)8000000U) /*!< Value of the External oscillator in Hz */
#endif /* XTHF_VALUE */    

#if !defined  (XTLF_VALUE)
    #define XTLF_VALUE    ((uint32_t)32768U) /*!< Value of the Internal oscillator in Hz*/
#endif /* XTLF_VALUE */        
    
    
#define __SYSTEM_CLOCK          (8000000)    

                            
/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern uint32_t SystemCoreClock;
void SystemCoreClockUpdate (void);


/**
  * @brief FL NVIC Init Structure definition
  */
typedef  struct
{
    /* 中断抢占优先级 */
    uint32_t preemptPriority;
    
}NVIC_ConfigTypeDef;

/**
  * @brief	NVIC_Init config NVIC
  *
  * @param 	nvicConfigStruct config params
  * @param 	IRQn             Interrupt number
  *
  * @retval	None
  */
void NVIC_Init(NVIC_ConfigTypeDef *nvicConfigStruct,IRQn_Type IRQn);


#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_FM33LG0XX_H */