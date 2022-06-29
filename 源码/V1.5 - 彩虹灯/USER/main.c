#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"
#include "timer.h"
#include "MY_KEY.h"


u8 SYS_Lamp_mode = 0;//0开机 1彩虹 2跟随传感器 3APP控制 4等待
static u8 Lamp_conversion = 0;//彩虹颜色转换
void Lamp_mode(void);//灯带模式控制
void Key_event_resolution(void);//按键事件解析

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	WS2812_Init();
	KEY_Init();
	TIM3_Int_Init(1000-1,720-1);//按键扫描 10ms
//	Set_ALL_Color(0);
//	LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
//	delay_us(400);//等待稳定
//	ColorToColor(0x000000,0xFF0000,1);
//	ColorToColor(0xFF0000,0x000000,1);
//	ColorToColor(0x000000,0xFFFFFF,1);
	while(1)
	{
		Lamp_mode();//灯光模式控制
		
		Key_event_resolution();//按键事件解析
	}
 }
 
 void Lamp_mode(void)//灯带模式控制
 {
	switch(SYS_Lamp_mode){
		case 0:
		LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
		delay_ms(100);
		Bead_color_shifts_R();
		break;
		case 1:
			switch(Lamp_conversion)
			{
				case 0:
					ColorToColor(RGB_white,RGB_red,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 1:
					ColorToColor(RGB_red,RGB_orange,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 2:
					ColorToColor(RGB_orange,RGB_yellow,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 3:
					ColorToColor(RGB_yellow,RGB_green,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 4:
					ColorToColor(RGB_green,RGB_blue,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 5:
					ColorToColor(RGB_blue,RGB_indigo,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 6:
					ColorToColor(RGB_indigo,RGB_purple,1);
				delay_ms(500);Lamp_conversion++;
				break;
				case 7:
					ColorToColor(RGB_purple,RGB_white,1);
					delay_ms(500);Lamp_conversion++;
				break;
				default:
					Lamp_conversion = 0;
			break;
			}
		
		break;
		case 2:
		;
		break;
		case 3:
		;
		break;
		case 4:
		;
		break;
		default:
			break;
	}
 }
 
 void Key_event_resolution(void)//按键事件解析
{
	if(MY_KEY.KEY_Event==1)
	{
		MY_KEY.KEY_Event=0;//清除按键事件
			switch(MY_KEY.KEY_Event_content)
			{
				case 0x01:
//					printf("KEY1");
				if(SYS_Lamp_mode==1) SYS_Lamp_mode = 4;
				else 
				{
					SYS_Lamp_mode = 1;
					Set_ALL_Color(0);
					LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
				}
					break;
				case 0x02:
//					printf("KEY2");
				
					break;
				case 0x04:
//					printf("KEY3");
				
					break;

			}
	}
}

