#include "led.h"
#include "delay.h"
#include "sys.h"
#include "WS2812.h"
#include "timer.h"
#include "MY_KEY.h"
#include "bsp_adc.h"
#include "Bluetooth.h"
#include "string.h" 
#include "math.h" 
#include "stdio.h" 
#include "stdlib.h"


u8 SYS_Lamp_mode = 0;//0���� 1�ʺ� 2���洫���� 3APP���� 4�ȴ�
static u8 Lamp_conversion = 0;//�ʺ���ɫת��
void Lamp_mode(void);//�ƴ�ģʽ����
void Key_event_resolution(void);//�����¼�����

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	ADCx_Init();
	WS2812_Init();
	KEY_Init();
	TIM3_Int_Init(1000-1,720-1);//����ɨ�� 10ms
	Bluetooth_init(9600);//��ʼ������ģ��
	while(1)
	{
		Lamp_mode();//�ƹ�ģʽ����
		
		Key_event_resolution();//�����¼�����
		
		if(Bluetooth_DATA.STA)//���յ��������� 
		{
			Bluetooth_send_str((u8 *)"RX232_DATA:",strlen("RX232_DATA:"));
			Bluetooth_send_str((u8 *)Bluetooth_DATA.Rx_Buffer,Bluetooth_DATA.Rx_len);
			Bluetooth_send_str((u8 *)"\r\n",2);
			Bluetooth_DATA.STA = 0;Bluetooth_DATA.Rx_len = 0;
		}
		
		if(Bluetooth_DATA_BUFF.STA)//���յ��������� 
		{
			Bluetooth_send_str((u8 *)"RX232_DATA_BUFF:",strlen("RX232_DATA_BUFF:"));
			Bluetooth_send_str((u8 *)Bluetooth_DATA_BUFF.Rx_Buffer,Bluetooth_DATA_BUFF.Rx_len);
			Bluetooth_send_str((u8 *)"\r\n",2);
			Bluetooth_DATA_BUFF.STA = 0;Bluetooth_DATA_BUFF.Rx_len = 0;
		}
	}
 }
 
 void Lamp_mode(void)//�ƴ�ģʽ����
 {
	 float Brightness_proportion;
	switch(SYS_Lamp_mode){
		case 0:
		LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
		delay_ms(100);
		Bead_color_shifts_R();
		break;
		case 1://�ʺ�ɫ�任
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
		case 2://���ȵ��� ADC 3000(�ƴ��) - 4000(�ƴ�����)
			if(Collect_complete)
			{
				Collect_complete = 0;
				//��������ȵ��ڱ���
				if(Brightness_data<=3000)
					Brightness_proportion = 0;
				else if(Brightness_data>=4000)
					Brightness_proportion = 1;
				else 
				{
					Brightness_proportion = Brightness_data - 3000;
					Brightness_proportion = Brightness_proportion/1000;
				}
				
				//��������ֵ
				Luminance_proportional_control(Current_color,Brightness_proportion);
			}
		
		break;
		case 3://APP����
		;
		break;
		case 4://�ȴ�����
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
				if(SYS_Lamp_mode==2) SYS_Lamp_mode = 4;
				else 
				{
					SYS_Lamp_mode = 2;
				}
					break;
				case 0x04:
//					printf("KEY3");
				if(SYS_Lamp_mode==3) SYS_Lamp_mode = 4;
				else 
				{
					//���������ʷ����
					SYS_Lamp_mode = 3;
				}
					break;
				default :
					break;

			}
	}
}

