/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_spi.h"
#include "bsp_led.h" 
#include "delay.h" 
#include "WS2812B.h" 
#include "WS2811.h"
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
//	  int i;
	  /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	  delay_ms(500);

	  //WS_Init();
	  LED_SPI_LowLevel_Init();
  	while(1)
		{  
			  WS_SetAll();
        LED_SPI_Update(WsDat,18);
//			ColorToColor(0x000000zh,0x0f4400);
//			ColorToColor(0x0f4400,0x000000);
//		  ColorToColor(0x000000,0x440044);
//			ColorToColor(0x440044,0x000000);
//			ColorToColor(0x000000,0x004444);
//			ColorToColor(0x004444,0x000000);
//      ColorToColor(0x000000,0x444400);
//			ColorToColor(0x444400,0x000000);
//			ColorToColor(0x000000,0x440000);
//			ColorToColor(0x440000,0x000000);
//			ColorToColor(0x000000,0x004400);
//			ColorToColor(0x004400,0x000000);
//			ColorToColor(0x000000,0x000044);
//			ColorToColor(0x000044,0x000000);
//			for(i=0;i<96;i++)
//			{
//				WS_SetAll();
				delay_ms(100);
//			}
//			
			
		}
	  
}

/*********************************************END OF FILE**********************/

