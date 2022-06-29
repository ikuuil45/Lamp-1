#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	WS2812_Init();
	delay_ms(1);
	WS2812B_Task();
	while(1)
	{
		
	}
 }

