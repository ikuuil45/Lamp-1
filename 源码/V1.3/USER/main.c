#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	WS2812_Init();
	delay_ms(1);//等待稳定
	LED_SPI_Update(Lamp_control_DATA_BUFF,108);
	while(1)
	{
		delay_ms(100);//延时
		Bead_color_shifts_R();
		LED_SPI_Update(Lamp_control_DATA_BUFF,108);
	}
 }

