#ifndef __WS2812_H
#define __WS2812_H	 
#include "sys.h"
#include "delay.h"

//            编码 0 
#define CODE_0     (u16)0xF000  //1111 0000 0000 0000
//            编码 1 
#define CODE_1     (u16)0xFFF0  //1111 1111 1110 0000

/*ws2812b灯珠数量*/
#define WS2812B_AMOUNT    108
 
extern unsigned long  Lamp_control_DATA_BUFF[WS2812B_AMOUNT];

#ifndef __WS2812_C


#endif

void WS2812_Init(void);//初始化

void LED_SPI_Update(unsigned long buffer[], uint32_t length);

void Bead_color_shifts_L(void);
void Bead_color_shifts_R(void);
u32 ColorToColor(unsigned long color0, unsigned long color1,u16 nus);
void Set_ALL_Color(unsigned long color_data);
		 				    
#endif


