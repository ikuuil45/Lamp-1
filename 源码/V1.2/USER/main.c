#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	WS2812_Init();
	delay_ms(1);
	WS2812B_Task();
	while(1)
	{
		
	}
 }

