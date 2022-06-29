#include "Bluetooth.h"
#include "string.h"
#include "sys.h"
#include "stdio.h"	 
#define __Bluetooth_C


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 

//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4,������ʱ����ͨ�ŷ�֡
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ����ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE ); //ʹ��ָ���ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIMx�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM4->CNT = 0;
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}

void Bluetooth_init(u32 bound)
{
	 //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);//ʹ�ܶ˿�ʱ��
 	USART_DeInit(USART1);  //��λ����1

		//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��

		//USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��
	
		//EN
		//��ʼ������Ĭ��ʹ�ܿ���
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);						 //Ĭ�������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		
		//ST
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��
		


   //USART1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		 //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
  USART_Init(USART1, &USART_InitStructure); //��ʼ������
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	USART_ClearFlag(USART1,USART_FLAG_TC);//���TC״̬λ�����,��ֹӲ����λ����֮��, ��һ���ֽڶ�ʧ,���Ĭ��ֵ�������ַ�����ʧ��	
	TIM4_Int_Init(999,719);	//10ms�ж�
}

//˫����
INIT_Bluetooth Bluetooth_DATA={0,0,0};
INIT_Bluetooth Bluetooth_DATA_BUFF={0,0,0};

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
		{
		Res =USART_ReceiveData(USART1);//	//��ȡ���յ�������
			
			if(Bluetooth_DATA.STA == 0)
			{
				if(Bluetooth_DATA.Rx_len<Bluetooth_REC_LEN)
				{
					Bluetooth_DATA.Rx_Buffer[Bluetooth_DATA.Rx_len++] = Res;
					//���¿�ʼ��ʱ
					TIM4->CNT = 0;
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
					TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
				}
				else 
				{
					if(Bluetooth_DATA_BUFF.STA == 0)
					{
						Bluetooth_DATA_BUFF = Bluetooth_DATA;//ת�뻺��
						Bluetooth_DATA_BUFF.STA = 1;
						Bluetooth_DATA.Rx_Buffer[0] = Res;
						Bluetooth_DATA.STA = 0; Bluetooth_DATA.Rx_len = 1; 
					}
					else Bluetooth_DATA.STA = 1;//�������������ޣ�ǿ�ƽ������
					TIM4->CNT = 0;
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
					TIM_ITConfig( TIM4,TIM_IT_Update ,DISABLE);
				}
			}
		 
		}		
} 
	

//��ʱ4�жϷ������
void TIM4_IRQHandler(void)   //TIMx�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIMx�����жϷ������
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);		//�رն�ʱ��  
			if(Bluetooth_DATA.Rx_len!=0) 
			{
				if(Bluetooth_DATA_BUFF.STA == 0)
					{
						Bluetooth_DATA_BUFF = Bluetooth_DATA;//ת�뻺��
						Bluetooth_DATA_BUFF.STA = 1;
						Bluetooth_DATA.STA = 0; Bluetooth_DATA.Rx_len = 0; 
					}
				else Bluetooth_DATA.STA = 1;	
			}	

		}
}

//Bluetooth����һ���ֽ�
void Bluetooth_send_byte(u8 val)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
	USART1->DR = val; //��������
}

//Bluetooth����һ���ַ���
void Bluetooth_send_str(u8 *str,u8 Bluetooth_len)
{
	u8 i = 0;
	for(i=0;i<Bluetooth_len;i++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
		USART1->DR=str[i];
	}			
}





