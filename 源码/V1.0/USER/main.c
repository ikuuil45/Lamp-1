#include "led.h"
#include "delay.h"
#include "sys.h"

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	while(1)
	{
		
		LED_W = 0;
		delay_ms(300);
		LED_W = 1;
		delay_ms(300);
		LED_G = 0;
		delay_ms(300);
		LED_G = 1;
		delay_ms(300);
		LED_B = 0;
		delay_ms(300);
		LED_B = 1;
		delay_ms(300);
		LED_R = 0;
		delay_ms(300);
		LED_R = 1;
		delay_ms(300);
		
	}
 }

