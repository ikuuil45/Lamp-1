#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	WS2812_Init();
	Set_ALL_Color(0);
	LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
	delay_us(400);//�ȴ��ȶ�
	ColorToColor(0x000000,0xFF0000,1);
	ColorToColor(0xFF0000,0x000000,1);
	ColorToColor(0x000000,0xFFFFFF,1);
	while(1)
	{
		
	}
 }

