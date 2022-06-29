#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"
#include "timer.h"
#include "MY_KEY.h"


u8 SYS_Lamp_mode = 0;//0���� 1�ʺ� 2���洫���� 3APP���� 4�ȴ�
static u8 Lamp_conversion = 0;//�ʺ���ɫת��
void Lamp_mode(void);//�ƴ�ģʽ����
void Key_event_resolution(void);//�����¼�����

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	WS2812_Init();
	KEY_Init();
	TIM3_Int_Init(1000-1,720-1);//����ɨ�� 10ms
//	Set_ALL_Color(0);
//	LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
//	delay_us(400);//�ȴ��ȶ�
//	ColorToColor(0x000000,0xFF0000,1);
//	ColorToColor(0xFF0000,0x000000,1);
//	ColorToColor(0x000000,0xFFFFFF,1);
	while(1)
	{
		Lamp_mode();//�ƹ�ģʽ����
		
		Key_event_resolution();//�����¼�����
	}
 }
 
 void Lamp_mode(void)//�ƴ�ģʽ����
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
 
 void Key_event_resolution(void)//�����¼�����
{
	if(MY_KEY.KEY_Event==1)
	{
		MY_KEY.KEY_Event=0;//��������¼�
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

