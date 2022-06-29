#include "WS2812.h"
#define __WS2812_C


//灯条显存SPI数据缓存
uint8_t gWs2812bDat_SPI[WS2812B_AMOUNT * 24] = {0}; 
//灯条显存
tWs2812bCache_TypeDef gWs2812bDat[WS2812B_AMOUNT] = {
//R    G      B
0X0F, 0X0F, 0X0F, //0
0X0F, 0X0F, 0X0F, //1
0X0F, 0X0F, 0X0F, //2
0X0F, 0X0F, 0X0F, //3
0X0F, 0X0F, 0X0F, //4
0X0F, 0X0F, 0X0F, //5
0X0F, 0X0F, 0X0F, //6
0X0F, 0X0F, 0X0F, //7
};


void WS2812_Init(void)//初始化
{
	SPI2_Init();
}

void WS2812b_Set(uint16_t Ws2b812b_NUM, uint8_t r,uint8_t g,uint8_t b)
{
  uint8_t *pR = &gWs2812bDat_SPI[(Ws2b812b_NUM) * 24 + 8];
  uint8_t *pG = &gWs2812bDat_SPI[(Ws2b812b_NUM) * 24];
  uint8_t *pB = &gWs2812bDat_SPI[(Ws2b812b_NUM) * 24 + 16];
	u8 i;

  for(i = 0; i <  8; i++) {
    if(g & 0x80) {
      *pG = CODE_1;
    }           
    else {           
      *pG = CODE_0;
    }           
    if(r & 0x80) {           
      *pR = CODE_1;
    }           
    else {           
      *pR = CODE_0;
    }           
    if(b & 0x80) {           
      *pB = CODE_1;
    }           
    else {           
      *pB = CODE_0;
    }
    r <<= 1;
    g <<= 1;
    b <<= 1;
    pR++;
    pG++;
    pB++;
  }
}
void WS2812B_Task(void)
{
  u8 dat = 0;
	u8 iLED = 0;

  //将gWs2812bDat数据解析成SPI数据
  for(iLED = 0; iLED < WS2812B_AMOUNT; iLED++)
  {
    WS2812b_Set(iLED, gWs2812bDat[iLED].R, gWs2812bDat[iLED].G, gWs2812bDat[iLED].B);
  }
  //总线输出数据
  SPI_Transmit_DATA(gWs2812bDat_SPI, sizeof(gWs2812bDat_SPI));
  //使总线输出低电平
  SPI_Transmit_DATA(&dat, 1);
  //帧信号：一个大于50us的低电平
  delay_ms(1); 
}




