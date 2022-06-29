#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	WS2812_Init();
	Set_ALL_Color(0);
	LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
	delay_us(400);//等待稳定
	ColorToColor(0x000000,0xFF0000,1);
	ColorToColor(0xFF0000,0x000000,1);
	ColorToColor(0x000000,0xFFFFFF,1);
	while(1)
	{
		
	}
 }

