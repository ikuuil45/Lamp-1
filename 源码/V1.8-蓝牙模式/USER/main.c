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


u8 SYS_Lamp_mode = 0;//0开机 1彩虹 2跟随传感器 3APP控制 4等待
u8 APP_mode = 0;//0普通灯光 1追逐
static u8 Lamp_conversion = 0;//彩虹颜色转换
void Lamp_mode(void);//灯带模式控制
void Key_event_resolution(void);//按键事件解析
void Bluetooth_comm_resolution(void);//蓝牙命令解析
void APP_mode_cont(void);//APP灯光模式控制

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	ADCx_Init();
	WS2812_Init();
	KEY_Init();
	TIM3_Int_Init(1000-1,720-1);//按键扫描 10ms
	Bluetooth_init(9600);//初始化蓝牙模块
	while(1)
	{
		Lamp_mode();//灯光模式控制
		
		Key_event_resolution();//按键事件解析
	}
 }
 
 void Lamp_mode(void)//灯带模式控制
 {
	 float Brightness_proportion;
	switch(SYS_Lamp_mode){
		case 0:
		LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
		delay_ms(100);
		Bead_color_shifts_R();
		break;
		case 1://彩虹色变换
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
		case 2://亮度调节 ADC 3000(灯带最暗) - 4000(灯带最亮)
			if(Collect_complete)
			{
				Collect_complete = 0;
				//计算出亮度调节比例
				if(Brightness_data<=3000)
					Brightness_proportion = 0;
				else if(Brightness_data>=4000)
					Brightness_proportion = 1;
				else 
				{
					Brightness_proportion = Brightness_data - 3000;
					Brightness_proportion = Brightness_proportion/1000;
				}
				
				//更新亮度值
				Luminance_proportional_control(Current_color,Brightness_proportion);
			}
		
		break;
		case 3://APP控制
		Bluetooth_comm_resolution();
		APP_mode_cont();
		break;
		case 4://等待命令
		;
		break;
		default:
			break;
	}
 }
 
 void Key_event_resolution(void)//按键事件解析
{
	if(MY_KEY.KEY_Event==1)
	{
		MY_KEY.KEY_Event=0;//清除按键事件
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

					//清除蓝牙历史数据
				Bluetooth_DATA.STA = 0;Bluetooth_DATA.Rx_len = 0;
				Bluetooth_DATA_BUFF.STA = 0;Bluetooth_DATA_BUFF.Rx_len = 0;
					SYS_Lamp_mode = 3;
					break;
				default :
					break;

			}
	}
}

//RGB十六进制字符串转换
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
			// 大写A～F转换成10-15,数字字符转数字0-9
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
 

void Bluetooth_comm_resolution(void)//蓝牙命令解析
{
	u16 i;
	if(Bluetooth_DATA.STA)//接收到蓝牙数据 
		{
//			Bluetooth_send_str((u8 *)"RX232_DATA:",strlen("RX232_DATA:"));
//			Bluetooth_send_str((u8 *)Bluetooth_DATA.Rx_Buffer,Bluetooth_DATA.Rx_len);
//			Bluetooth_send_str((u8 *)"\r\n",2);
				
				for(i=0;i<Bluetooth_DATA.Rx_len;i++)//空格填充0字符位
				{
					if(Bluetooth_DATA.Rx_Buffer[i]==0) Bluetooth_DATA.Rx_Buffer[i] = ' ';
				}
				
				Bluetooth_DATA.Rx_Buffer[Bluetooth_DATA.Rx_len] = '\0';//添加字符串结束符
				
				if(strstr((const char*)Bluetooth_DATA.Rx_Buffer,(const char*)"#"))//命令颜色设定
				{
							Current_color = htoi(strchr((const char*)Bluetooth_DATA.Rx_Buffer,'#'));
							Set_ALL_Color(Current_color);
							LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);				
				}
				
				if(strstr((const char*)Bluetooth_DATA.Rx_Buffer,(const char*)"SET_MODE")) 
				{
					APP_mode = !APP_mode;//命令模式装换
				}
				
			Bluetooth_DATA.STA = 0;Bluetooth_DATA.Rx_len = 0;
		}
		
	if(Bluetooth_DATA_BUFF.STA)//接收到蓝牙数据 
		{
//			Bluetooth_send_str((u8 *)"RX232_DATA_BUFF:",strlen("RX232_DATA_BUFF:"));
//			Bluetooth_send_str((u8 *)Bluetooth_DATA_BUFF.Rx_Buffer,Bluetooth_DATA_BUFF.Rx_len);
//			Bluetooth_send_str((u8 *)"\r\n",2);
			
			for(i=0;i<Bluetooth_DATA_BUFF.Rx_len;i++)//空格填充0字符位
			{
				if(Bluetooth_DATA_BUFF.Rx_Buffer[i]==0) Bluetooth_DATA_BUFF.Rx_Buffer[i] = ' ';
			}
			
			Bluetooth_DATA_BUFF.Rx_Buffer[Bluetooth_DATA_BUFF.Rx_len] = '\0';//添加字符串结束符
			
			if(strstr((const char*)Bluetooth_DATA_BUFF.Rx_Buffer,(const char*)"#"))//命令颜色设定
			{
						Current_color = htoi(strchr((const char*)Bluetooth_DATA_BUFF.Rx_Buffer,'#'));
						Set_ALL_Color(Current_color);
						LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);				
			}
			
			if(strstr((const char*)Bluetooth_DATA_BUFF.Rx_Buffer,(const char*)"SET_MODE")) 
			{
				APP_mode = !APP_mode;//命令模式装换
			}
			
			Bluetooth_DATA_BUFF.STA = 0;Bluetooth_DATA_BUFF.Rx_len = 0;
		}
}	

void APP_mode_cont(void)//APP灯光模式控制
{
//	APP_mode = !APP_mode;
	
	if(APP_mode)//追逐
	{
		
	}
	else //普通颜色
	{
		
	}
	
	
}







