#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	WS2812_Init();
	delay_ms(1);//�ȴ��ȶ�
	LED_SPI_Update(Lamp_control_DATA_BUFF,108);
	while(1)
	{
		delay_ms(100);//��ʱ
		Bead_color_shifts_R();
		LED_SPI_Update(Lamp_control_DATA_BUFF,108);
	}
 }

