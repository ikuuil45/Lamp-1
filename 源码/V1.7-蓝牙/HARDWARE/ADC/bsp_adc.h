#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK1                 RCC_APB2Periph_GPIOA
#define    ADC_PORT1                     GPIOA

// 注意
// 所以 PC0 做 ADC 转换通道的时候，结果可能会有误差

// 转换通道个数
#define    NOFCHANEL										 1
// ADC缓存数组大小
#define    ADC_data_buff								 10

#define    ADC_PIN5                      GPIO_Pin_5
#define    ADC_CHANNEL5                 ADC_Channel_5


// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************函数声明********************************/
void               ADCx_Init                               (void);

extern __IO uint16_t ADC_DATA[ADC_data_buff];
extern u8 Collect_complete ;//采样完成标识
extern u16 Brightness_data ;//亮度数据



#endif /* __ADC_H */

