#include "bstim16.h"
#include "main.h"
#include "can.h"




uint8_t  Bus_Off_Flag = 0;                       /* CAN BUSOFF 标志 */
uint8_t  Fast_Recover_Count = 5;                 /* 快恢复次数定义 */
uint32_t Recover_Timeout = FAST_RECOVER_TIMEOUT; /* 初始为快恢复 */


/**
  * @brief  BSTIM16中断
  * @param  void
  * @retval void
  */
void BSTIM_IRQHandler(void)
{
    if (FL_BSTIM16_IsActiveFlag_Update(BSTIM16) && FL_BSTIM16_IsEnabledIT_Update(BSTIM16))
    {
        FL_BSTIM16_ClearFlag_Update(BSTIM16);
        
        if (Bus_Off_Flag)
        {   
            if (--Recover_Timeout == 0)
            {
                Bus_Off_Flag = 0;
                
                if (Fast_Recover_Count)
                {
                    Fast_Recover_Count--;
                }
                
                if (Fast_Recover_Count == 0)
                {
                    Recover_Timeout = SLOW_RECOVER_TIMEOUT;
                }
                else
                {
                    Recover_Timeout = FAST_RECOVER_TIMEOUT;
                }

                can_reset_with_send();
            }
        }
    }
}

/**
  * @brief  BSTIM16初始化
  * @note   1ms定时器
  * @param  void
  * @retval void
  */
void BSTIM16_Init(void)
{
    FL_BSTIM16_InitTypeDef    TimerBase_InitStruct;

    TimerBase_InitStruct.prescaler = 0;
    TimerBase_InitStruct.autoReload = 8000-1;
    TimerBase_InitStruct.autoReloadState = FL_DISABLE;
    TimerBase_InitStruct.clockSource = FL_CMU_BSTIM16_CLK_SOURCE_APBCLK;

    FL_BSTIM16_Init(BSTIM16, &TimerBase_InitStruct);

    FL_BSTIM16_ClearFlag_Update(BSTIM16);    /* 清除计数器中断标志位 */
    FL_BSTIM16_EnableIT_Update(BSTIM16);     /* 开启计数器中断 */
    NVIC_ClearPendingIRQ(BSTIM_IRQn);
  
    NVIC_DisableIRQ(BSTIM_IRQn);
    NVIC_SetPriority(BSTIM_IRQn, 2);        /* 中断优先级配置 */
    NVIC_EnableIRQ(BSTIM_IRQn);

    FL_BSTIM16_Enable(BSTIM16);         /* 使能定时器 */

}

