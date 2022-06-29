#ifndef __MY_KEY_H
#define __MY_KEY_H
#include "sys.h"

#define Shaking_time 8 //消除抖动的时间8*定时中断时间

#define KEY1 PBin(5)   	
#define KEY2 PBin(10)	 
#define KEY3 PBin(11)	

typedef struct
{
	u8 KEY_STA;//当前按键状态
	
	u8 Previous_STA;//按键上一次状态
	
	u8 Counter;//计数器
	
	u8 KEY_flag;//按键标志 0未按下（弹起） 1已按下 2长按 
	
	u8 key_value;//键值缓存
	
	u8 KEY_Event;//记录按键事件 0已处理按键事件 1存在未处理的按键事件
	u8 KEY_Event_type;//按键类型 1短按 2长按 
	u8 KEY_Event_content;//按键值
	/*************************************************************
	//按键编号映射表
		0x00,				0x04,				0x02,				0x01,				0x06,				0x05,				0x03,				0x07
	//为按下			KEY3按下		KEY2按下		KEY1按下		KEY3、2按下	KEY3、1按下	KEY1、2按下	KEY3、2、1按下
	*************************************************************///
	
	
}My_key_InitTypeDef;

#ifndef __MY_KEY_C

extern My_key_InitTypeDef MY_KEY;

#endif
//按键IO初始化
void KEY_Init(void);
//按键扫描函数--建议中断调动
void KEY_Scan(void);

#endif











