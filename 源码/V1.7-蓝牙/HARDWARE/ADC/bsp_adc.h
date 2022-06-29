#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK1                 RCC_APB2Periph_GPIOA
#define    ADC_PORT1                     GPIOA

// ע��
// ���� PC0 �� ADC ת��ͨ����ʱ�򣬽�����ܻ������

// ת��ͨ������
#define    NOFCHANEL										 1
// ADC���������С
#define    ADC_data_buff								 10

#define    ADC_PIN5                      GPIO_Pin_5
#define    ADC_CHANNEL5                 ADC_Channel_5


// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************��������********************************/
void               ADCx_Init                               (void);

extern __IO uint16_t ADC_DATA[ADC_data_buff];
extern u8 Collect_complete ;//������ɱ�ʶ
extern u16 Brightness_data ;//��������



#endif /* __ADC_H */

