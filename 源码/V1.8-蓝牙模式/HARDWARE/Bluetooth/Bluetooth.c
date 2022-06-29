#include "Bluetooth.h"
#include "string.h"
#include "sys.h"
#include "stdio.h"	 
#define __Bluetooth_C


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 

//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4,用来定时串口通信分帧
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE ); //使能指定中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIMx中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM4->CNT = 0;
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}

void Bluetooth_init(u32 bound)
{
	 //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);//使能端口时钟
 	USART_DeInit(USART1);  //复位串口1

		//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化

		//USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化
	
		//EN
		//初始化蓝牙默认使能开启
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);						 //默认输出低
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
		
		//ST
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化
		


   //USART1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		 //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
  USART_Init(USART1, &USART_InitStructure); //初始化串口
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART1, ENABLE);                    //使能串口 
	USART_ClearFlag(USART1,USART_FLAG_TC);//完成TC状态位的清除,防止硬件复位重启之后, 第一个字节丢失,清掉默认值避免首字符发送失败	
	TIM4_Int_Init(999,719);	//10ms中断
}

//双缓存
INIT_Bluetooth Bluetooth_DATA={0,0,0};
INIT_Bluetooth Bluetooth_DATA_BUFF={0,0,0};

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
		{
		Res =USART_ReceiveData(USART1);//	//读取接收到的数据
			
			if(Bluetooth_DATA.STA == 0)
			{
				if(Bluetooth_DATA.Rx_len<Bluetooth_REC_LEN)
				{
					Bluetooth_DATA.Rx_Buffer[Bluetooth_DATA.Rx_len++] = Res;
					//重新开始计时
					TIM4->CNT = 0;
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
					TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
				}
				else 
				{
					if(Bluetooth_DATA_BUFF.STA == 0)
					{
						Bluetooth_DATA_BUFF = Bluetooth_DATA;//转入缓存
						Bluetooth_DATA_BUFF.STA = 1;
						Bluetooth_DATA.Rx_Buffer[0] = Res;
						Bluetooth_DATA.STA = 0; Bluetooth_DATA.Rx_len = 1; 
					}
					else Bluetooth_DATA.STA = 1;//超出数据量上限，强制接收完成
					TIM4->CNT = 0;
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
					TIM_ITConfig( TIM4,TIM_IT_Update ,DISABLE);
				}
			}
		 
		}		
} 
	

//定时4中断服务程序
void TIM4_IRQHandler(void)   //TIMx中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIMx更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);		//关闭定时器  
			if(Bluetooth_DATA.Rx_len!=0) 
			{
				if(Bluetooth_DATA_BUFF.STA == 0)
					{
						Bluetooth_DATA_BUFF = Bluetooth_DATA;//转入缓存
						Bluetooth_DATA_BUFF.STA = 1;
						Bluetooth_DATA.STA = 0; Bluetooth_DATA.Rx_len = 0; 
					}
				else Bluetooth_DATA.STA = 1;	
			}	

		}
}

//Bluetooth发送一个字节
void Bluetooth_send_byte(u8 val)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
	USART1->DR = val; //发送数据
}

//Bluetooth发送一个字符串
void Bluetooth_send_str(u8 *str,u8 Bluetooth_len)
{
	u8 i = 0;
	for(i=0;i<Bluetooth_len;i++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
		USART1->DR=str[i];
	}			
}





