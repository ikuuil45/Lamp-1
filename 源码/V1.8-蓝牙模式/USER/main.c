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
u8 APP_mode = 0;//0��ͨ�ƹ� 1׷��
static u8 Lamp_conversion = 0;//�ʺ���ɫת��
void Lamp_mode(void);//�ƴ�ģʽ����
void Key_event_resolution(void);//�����¼�����
void Bluetooth_comm_resolution(void);//�����������
void APP_mode_cont(void);//APP�ƹ�ģʽ����

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
		Bluetooth_comm_resolution();
		APP_mode_cont();
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

					//���������ʷ����
				Bluetooth_DATA.STA = 0;Bluetooth_DATA.Rx_len = 0;
				Bluetooth_DATA_BUFF.STA = 0;Bluetooth_DATA_BUFF.Rx_len = 0;
					SYS_Lamp_mode = 3;
					break;
				default :
					break;

			}
	}
}

//RGBʮ�������ַ���ת��
unsigned long htoi(char s[])
{
	unsigned long totalNum = 0;
	int c;
	int position = 0;
	unsigned long num;
	u8 i;	

		for (i=6;i>0;i--) 
		{
			c = s[position+i];
			// ��дA��Fת����10-15,�����ַ�ת����0-9
			if (c >= 'A' && c <= 'F') {
				num = c - 'A' + 10;
			} else if (c >= '0' && c <= '9'){
				num = c - '0';
			}else num = 0;
			num = num<<((6-i)*4);
			totalNum += num;
		}
	return totalNum;
}
 

void Bluetooth_comm_resolution(void)//�����������
{
	u16 i;
	if(Bluetooth_DATA.STA)//���յ��������� 
		{
//			Bluetooth_send_str((u8 *)"RX232_DATA:",strlen("RX232_DATA:"));
//			Bluetooth_send_str((u8 *)Bluetooth_DATA.Rx_Buffer,Bluetooth_DATA.Rx_len);
//			Bluetooth_send_str((u8 *)"\r\n",2);
				
				for(i=0;i<Bluetooth_DATA.Rx_len;i++)//�ո����0�ַ�λ
				{
					if(Bluetooth_DATA.Rx_Buffer[i]==0) Bluetooth_DATA.Rx_Buffer[i] = ' ';
				}
				
				Bluetooth_DATA.Rx_Buffer[Bluetooth_DATA.Rx_len] = '\0';//����ַ���������
				
				if(strstr((const char*)Bluetooth_DATA.Rx_Buffer,(const char*)"#"))//������ɫ�趨
				{
							Current_color = htoi(strchr((const char*)Bluetooth_DATA.Rx_Buffer,'#'));
							Set_ALL_Color(Current_color);
							LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);				
				}
				
				if(strstr((const char*)Bluetooth_DATA.Rx_Buffer,(const char*)"SET_MODE")) 
				{
					APP_mode = !APP_mode;//����ģʽװ��
				}
				
			Bluetooth_DATA.STA = 0;Bluetooth_DATA.Rx_len = 0;
		}
		
	if(Bluetooth_DATA_BUFF.STA)//���յ��������� 
		{
//			Bluetooth_send_str((u8 *)"RX232_DATA_BUFF:",strlen("RX232_DATA_BUFF:"));
//			Bluetooth_send_str((u8 *)Bluetooth_DATA_BUFF.Rx_Buffer,Bluetooth_DATA_BUFF.Rx_len);
//			Bluetooth_send_str((u8 *)"\r\n",2);
			
			for(i=0;i<Bluetooth_DATA_BUFF.Rx_len;i++)//�ո����0�ַ�λ
			{
				if(Bluetooth_DATA_BUFF.Rx_Buffer[i]==0) Bluetooth_DATA_BUFF.Rx_Buffer[i] = ' ';
			}
			
			Bluetooth_DATA_BUFF.Rx_Buffer[Bluetooth_DATA_BUFF.Rx_len] = '\0';//����ַ���������
			
			if(strstr((const char*)Bluetooth_DATA_BUFF.Rx_Buffer,(const char*)"#"))//������ɫ�趨
			{
						Current_color = htoi(strchr((const char*)Bluetooth_DATA_BUFF.Rx_Buffer,'#'));
						Set_ALL_Color(Current_color);
						LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);				
			}
			
			if(strstr((const char*)Bluetooth_DATA_BUFF.Rx_Buffer,(const char*)"SET_MODE")) 
			{
				APP_mode = !APP_mode;//����ģʽװ��
			}
			
			Bluetooth_DATA_BUFF.STA = 0;Bluetooth_DATA_BUFF.Rx_len = 0;
		}
}	

void APP_mode_cont(void)//APP�ƹ�ģʽ����
{
//	APP_mode = !APP_mode;
	
	if(APP_mode)//׷��
	{
		
	}
	else //��ͨ��ɫ
	{
		
	}
	
	
}







