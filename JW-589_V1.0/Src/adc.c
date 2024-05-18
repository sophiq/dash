#include "adc.h"
#include "user_init.h"

uint8_t ADCComplete = 0;
uint16_t  ADC_ConvertedValue[4]={0};
//void ADC_IRQHandler(void)
//{

//    if((FL_ENABLE == FL_ADC_IsEnabledIT_EndOfConversion(ADC))
//            && (FL_SET == FL_ADC_IsActiveFlag_EndOfConversion(ADC)))
//    {
//        FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
//        ADCComplete = 1;

//    }

//}


void DMA_IRQHandler(void)
{
	if(FL_DMA_IsActiveFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4))
    {

	    FL_DMA_ClearFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4); 
			ADCComplete = 1;
		}
}







void AdcInit(void)
{
    FL_GPIO_InitTypeDef         GPIO_InitStruct = {0};
    FL_ADC_CommonInitTypeDef    ADC_CommonInitStruct;
    FL_ADC_InitTypeDef          ADC_InitStruct ;

    //配置引脚为模拟功能
    //ADC ADC_1 引脚 PD1
    GPIO_InitStruct.pin        = FL_GPIO_PIN_7 | FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10;
    GPIO_InitStruct.mode       = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull       = FL_DISABLE;
    GPIO_InitStruct.remapPin   = FL_DISABLE;
    GPIO_InitStruct.analogSwitch   = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    //ADC 时钟设置
    ADC_CommonInitStruct.operationSource            = FL_ADC_CLK_SOURCE_ADCCLK;
    ADC_CommonInitStruct.clockSource                = FL_CMU_ADC_CLK_SOURCE_PLL;
    ADC_CommonInitStruct.clockPrescaler             = FL_CMU_ADC_PSC_DIV8;
    ADC_CommonInitStruct.APBClockPrescaler          = FL_ADC_APBCLK_PSC_DIV1;
    ADC_CommonInitStruct.referenceSource            = FL_ADC_REF_SOURCE_VDDA;
    ADC_CommonInitStruct.bitWidth                   = FL_ADC_BIT_WIDTH_12B;
    FL_ADC_CommonInit(&ADC_CommonInitStruct);

    //ADC 寄存器设置
    ADC_InitStruct.conversionMode                   = FL_ADC_CONV_MODE_SINGLE;//单次模式
    ADC_InitStruct.autoMode                         = FL_ADC_SINGLE_CONV_MODE_AUTO;//自动
    ADC_InitStruct.waitMode                         = FL_ENABLE;//等待
    ADC_InitStruct.overrunMode                      = FL_ENABLE;//覆盖上次数据
    ADC_InitStruct.scanDirection                    = FL_ADC_SEQ_SCAN_DIR_BACKWARD;//通道正序扫描
    ADC_InitStruct.externalTrigConv                 = FL_ADC_TRIGGER_EDGE_RISING;//禁止触发信号
    ADC_InitStruct.triggerSource                    = FL_ADC_TRGI_ATIM;//FL_ADC_TRGI_LUT0;
    ADC_InitStruct.fastChannelTime                  = FL_ADC_FAST_CH_SAMPLING_TIME_2_ADCCLK;//快速通道采样时间
    ADC_InitStruct.lowChannelTime                   = FL_ADC_SLOW_CH_SAMPLING_TIME_192_ADCCLK;//慢速通道采样时间
    ADC_InitStruct.oversamplingMode                 = FL_DISABLE;//过采样关闭
    ADC_InitStruct.overSampingMultiplier            = FL_ADC_OVERSAMPLING_MUL_8X;//8倍过采样
    ADC_InitStruct.oversamplingShift                = FL_ADC_OVERSAMPLING_SHIFT_3B;//数据右移, /8

    FL_ADC_Init(ADC, &ADC_InitStruct);
		FL_ADC_EnableDMAReq(ADC);
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH6); //通道选择ADC_6
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH13);//通道选择ADC_13
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH14);//通道选择ADC_14
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH15);//通道选择ADC_15

}

void ADC_DMA(uint16_t *buffer, uint32_t length)
{
    FL_DMA_InitTypeDef DMA_InitStruct = {0};
    FL_DMA_ConfigTypeDef DMA_ConfigStruct = {0};
    DMA_InitStruct.periphAddress = FL_DMA_PERIPHERAL_FUNCTION1;
    DMA_InitStruct.direction = FL_DMA_DIR_PERIPHERAL_TO_RAM;
    DMA_InitStruct.memoryAddressIncMode = FL_DMA_MEMORY_INC_MODE_INCREASE;
    DMA_InitStruct.dataSize = FL_DMA_BANDWIDTH_16B;
    DMA_InitStruct.priority = FL_DMA_PRIORITY_HIGH;
    DMA_InitStruct.circMode = FL_DISABLE;
    FL_DMA_Init(DMA, &DMA_InitStruct, FL_DMA_CHANNEL_4);

    DMA_ConfigStruct.memoryAddress = (uint32_t)buffer;
    DMA_ConfigStruct.transmissionCount = length - 1;
    FL_DMA_StartTransmission(DMA, &DMA_ConfigStruct, FL_DMA_CHANNEL_4);
	  NVIC_DisableIRQ(DMA_IRQn);
	  NVIC_SetPriority(DMA_IRQn, 4);
	  NVIC_EnableIRQ(DMA_IRQn);
    FL_DMA_Enable(DMA);	
	  FL_DMA_ClearFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4);		
    FL_DMA_EnableIT_TransferComplete(DMA, FL_DMA_CHANNEL_4);
	  FL_DMA_EnableChannel(DMA, FL_DMA_CHANNEL_4);
}

uint32_t GetVREF1P2Sample(void)
{
    uint16_t ADCRdresult;
    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV8);
    FL_VREF_EnableVREFBuffer(VREF);//使能VREF BUFFER
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);//通道选择VREF

    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
    FL_ADC_Enable(ADC);      // 启动ADC
    FL_ADC_EnableSWConversion(ADC);  // 开始转换

    // 等待转换完成
    while(FL_ADC_IsActiveFlag_EndOfConversion(ADC) == FL_RESET);

    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
    ADCRdresult = FL_ADC_ReadConversionData(ADC); //获取采样值

    FL_ADC_Disable(ADC);    // 关闭ADC
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);//通道关闭VREF
    FL_VREF_DisableVREFBuffer(VREF);//关闭VREF BUFFER
    // 转换结果
    return ADCRdresult;
}

uint32_t GetDifferentialChannelSample(void)
{
    uint16_t ADCRdresult;
    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV1);
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH1);//通道选择ADC_1
    FL_ADC_EnableDifferentialChannel(ADC, FL_ADC_DIFFERENTIAL_GROUP1);
    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
    FL_ADC_Enable(ADC);      // 启动ADC
    FL_ADC_EnableSWConversion(ADC);  // 开始转换

    // 等待转换完成
    while(FL_ADC_IsActiveFlag_EndOfConversion(ADC) == FL_RESET);

    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
    ADCRdresult = FL_ADC_ReadConversionData(ADC); //获取采样值?

    FL_ADC_Disable(ADC);    // 关闭ADC
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH1);//通道关闭ADC_1
    FL_ADC_DisableDifferentialChannel(ADC, FL_ADC_DIFFERENTIAL_GROUP1);
    // 转换结果
    return ADCRdresult;
}

uint32_t GetSingleChannelSample(void)
{
    uint16_t ADCRdresult;
    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV1);
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH1);//通道选择ADC_1

    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
    FL_ADC_Enable(ADC);      // 启动ADC
    FL_ADC_EnableSWConversion(ADC);  // 开始转换

    // 等待转换完成
    while(FL_ADC_IsActiveFlag_EndOfConversion(ADC) == FL_RESET);

    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
    ADCRdresult = FL_ADC_ReadConversionData(ADC); //获取采样值?

    FL_ADC_Disable(ADC);    // 关闭ADC
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH1);//通道关闭ADC_1
    // 转换结果
    return ADCRdresult;
}

void GetDMAChannelSample(void)
{
//    FL_VREF_EnableVREFBuffer(VREF);//使能VREF BUFFER
//    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);//通道选择VREF
//    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH1);//通道选择ADC_1
//    ADC_DMA(DMAResult, 2);

//    FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志
//    FL_ADC_Enable(ADC);      // 启动ADC
//    FL_ADC_EnableSWConversion(ADC);  // 开始转换

//    // 等待转换完成
//    while(!FL_DMA_IsActiveFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4));

//    FL_DMA_ClearFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4);

//    FL_ADC_Disable(ADC);    // 关闭ADC
//    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH1);//通道关闭ADC_1
//    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);//通道关闭VREF
//    FL_VREF_DisableVREFBuffer(VREF);//关闭VREF BUFFER
//    // 转换结果
	
  ADC_DMA(ADC_ConvertedValue,4);	
	FL_ADC_ClearFlag_EndOfConversion(ADC);//清标志			
	FL_ADC_Enable(ADC);   	 // 启动ADC	

}

