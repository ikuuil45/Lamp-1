#include "MY_KEY.h"
#define __MY_KEY_C
/*************************************************************
//按键编号映射表
	0x00,				0x04,				0x02,				0x01,				0x06,				0x05,				0x03,				0x07
//未按下			KEY3按下		KEY2按下		KEY1按下		KEY3、2按下	KEY3、1按下	KEY1、2按下	KEY3、2、1按下
*************************************************************///

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
 
} 

//定义按键变量并初始化 
My_key_InitTypeDef MY_KEY = {0};

//按键处理函数--需要在定时器中调用 推荐定时器时间10ms
//支持 短按 长按 组合按
void KEY_Scan(void)
{	 
	
	/*****更新按键状态值****/
	if(KEY1==0) MY_KEY.KEY_STA |= 0x01;//第一位置一
		else MY_KEY.KEY_STA &= 0xfe;//第一位清零
	if(KEY2==0) MY_KEY.KEY_STA |= 0x02;//第二位置一
		else MY_KEY.KEY_STA &= 0xfd;//第二位清零
	if(KEY3==0) MY_KEY.KEY_STA |= 0x04;//第三位置一
		else MY_KEY.KEY_STA &= 0xfb;//第三位清零
	
	/******判断******/
	if(MY_KEY.KEY_STA!=MY_KEY.Previous_STA)//与上一状态不相等
	{
		MY_KEY.Counter = 0;//计数器清零
		MY_KEY.Previous_STA = MY_KEY.KEY_STA;//更新状态值
	}
	else //遇上一次状态相等
	{
		if(MY_KEY.Counter<200)	//小于2S
		{
			MY_KEY.Counter++;//计数器自增
			if(MY_KEY.Counter==Shaking_time) //计数时间等于按键抖动时间，认为按键已稳定
			{
				if(MY_KEY.KEY_STA==0x00)//是否是按键弹起
				{
					MY_KEY.Counter = 0;//计数器清零
					
						if(MY_KEY.KEY_flag==1)//事件刷新
						{
							MY_KEY.KEY_Event = 1;//标记事件
							MY_KEY.KEY_Event_type = 1;//标记事件类型
							MY_KEY.KEY_Event_content = MY_KEY.key_value;//标记事件内容
						}

					MY_KEY.KEY_flag = 0;//记录此时按键状态  弹起
					MY_KEY.key_value = 0;//缓存清零
				}
				else 
				{
					MY_KEY.KEY_flag = 1;//记录此时按键状态  短按 按下
					if(MY_KEY.key_value<MY_KEY.KEY_STA) MY_KEY.key_value = MY_KEY.KEY_STA;//刷新只记录组合键最大值
//					MY_KEY.key_value = MY_KEY.KEY_STA;//刷新记录释放按键时的值
				}
				
			}
		}
		else //按键时间超过2S，判断为长按
		{
				if(MY_KEY.KEY_flag!=2)//屏蔽此句连续标记
				{
					MY_KEY.KEY_Event = 1;//标记事件
					MY_KEY.KEY_Event_type = 2;//标记事件类型
					MY_KEY.KEY_Event_content = MY_KEY.key_value;//标记事件内容
					MY_KEY.KEY_flag = 2;//记录此时按键状态  长按
				}
		}
	}

	
}











