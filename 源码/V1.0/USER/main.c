#include "led.h"
#include "delay.h"
#include "sys.h"

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
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

