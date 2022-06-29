#include "WS2812.h"
#define __WS2812_C

uint16_t Lamp_control_DATA[WS2812B_AMOUNT*24] = {0};//灯带控制数据发送缓存
uint16_t PixelPointer = 0;//灯带控制数据当前寻址位置
unsigned long  Lamp_control_DATA_BUFF[WS2812B_AMOUNT] = {
	0xAA0000,0x770000,0x330000,0x110000,
	0x050000,0x000000,0x00AA00,0x007700,
	0x003300,0x001100,0x000500,0x000000,
	0x0000AA,0x000077,0x000033,0x000011,
	0x000005,0x000000,0xAA0000,0x770000,
	0x330000,0x110000,0x050000,0x000000,
	0x00AA00,0x007700,0x003300,0x001100,
	0x000500,0x000000,0x0000AA,0x000077,
	0x000033,0x000011,0x000005,0x000000,	
	0xAA0000,0x770000,0x330000,0x110000,
	0x050000,0x000000,0x00AA00,0x007700,
	0x003300,0x001100,0x000500,0x000000,
	0x0000AA,0x000077,0x000033,0x000011,
	0x000005,0x000000,0xAA0000,0x770000,
	0x330000,0x110000,0x050000,0x000000,
	0x00AA00,0x007700,0x003300,0x001100,
	0x000500,0x000000,0x0000AA,0x000077,
	0x000033,0x000011,0x000005,0x000000,
	0xAA0000,0x770000,0x330000,0x110000,
	0x050000,0x000000,0x00AA00,0x007700,
	0x003300,0x001100,0x000500,0x000000,
	0x0000AA,0x000077,0x000033,0x000011,
	0x000005,0x000000,0xAA0000,0x770000,
	0x330000,0x110000,0x050000,0x000000,
	0x00AA00,0x007700,0x003300,0x001100,
	0x000500,0x000000,0x0000AA,0x000077,
	0x000033,0x000011,0x000005,0x000000

};//灯带颜色控制数据缓存 R G B

void WS2812_Init(void)//初始化
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Lamp_control_DATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure); /* DMA1 CH3 = MEM -> DR */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

		/*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//MOSI
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
		*/

    SPI_I2S_DeInit(SPI1);

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; /* 72MHz / 4 = 18MHz / 16bit*/
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

    SPI_Cmd(SPI1, ENABLE);
}

void LED_SPI_WriteByte(uint16_t Data)
{

    Lamp_control_DATA[PixelPointer] = Data;
    PixelPointer++;

    /* Send the byte */
}

void LED_SPI_SendBits(uint8_t bits)
{
    u8 i = 0x00;

    for (i = 0x80; i >= 0x01; i >>= 1) //16bit数据
    {
        LED_SPI_WriteByte((bits & i) ? CODE_1 : CODE_0);
    }
}

void LED_SPI_SendPixel(uint32_t color)
{
    /*
     r7,r6,r5,r4,r3,r2,r1,r0,g7,g6,g5,g4,g3,g2,g1,g0,b7,b6,b5,b4,b3,b2,b1,b0
     \_____________________________________________________________________/
                               |      _________________...
                               |     /   __________________...
                               |    /   /   ___________________...
                               |   /   /   /
                              RGB,RGB,RGB,RGB,...,STOP
    */

    /*
    	BUG Fix : Actual is RGB,datasheet is something wrong.
    */
	  uint8_t Red, Green, Blue;  // 三原色
		// 红 绿 蓝 三原色分解
		Red = color>>16;
	  Green  = color>>8;
	  Blue  = color;
    LED_SPI_SendBits(Red);
    LED_SPI_SendBits(Green);
    LED_SPI_SendBits(Blue);
}



void LED_SPI_Update(unsigned long buffer[], uint32_t length)
{
    uint8_t i = 0;
    if(DMA_GetCurrDataCounter(DMA1_Channel3) == 0)
    {
        for (i = 0; i < length; i++)
        {
            LED_SPI_SendPixel(buffer[i]);
        }
        PixelPointer = 0;

//        DMA_Cmd(DMA1_Channel3, DISABLE);
//        DMA_ClearFlag(DMA1_FLAG_TC3);
        DMA_SetCurrDataCounter(DMA1_Channel3, 24*length+20);
        DMA_Cmd(DMA1_Channel3, ENABLE);
				while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)) ; 	// wait until transfer complete
				DMA_Cmd(DMA1_Channel3, DISABLE); 			// disable DMA channel 6
				DMA_ClearFlag(DMA1_FLAG_TC3); 				// clear DMA1 Channel 6 transfer complete flag
    }
}

/**************************************************************************************
* 灯珠颜色左移位
**************************************************************************************/
void Bead_color_shifts_L(void)
{
	unsigned char j;
	unsigned long temp;

		temp=Lamp_control_DATA_BUFF[0];
		for(j=0;j<(WS2812B_AMOUNT-1);j++)
		{
			Lamp_control_DATA_BUFF[j]=Lamp_control_DATA_BUFF[j+1];
		}
		Lamp_control_DATA_BUFF[WS2812B_AMOUNT-1]=temp;
}

/**************************************************************************************
* 灯珠颜色右移位
**************************************************************************************/
void Bead_color_shifts_R(void)
{
	unsigned char j;
	unsigned long temp;

		temp=Lamp_control_DATA_BUFF[WS2812B_AMOUNT-1];
		for(j=(WS2812B_AMOUNT-1);j>0;j--)
		{
			Lamp_control_DATA_BUFF[j]=Lamp_control_DATA_BUFF[j-1];
		}
		Lamp_control_DATA_BUFF[0]=temp;
}

void Set_ALL_Color(unsigned long color_data)
{
	unsigned char j;
	for(j=0; j<WS2812B_AMOUNT; j++)
	{
		Lamp_control_DATA_BUFF[j] = color_data;  //
	}
}

/********************************************
* 求绝对值
********************************************/
unsigned char abs0(int num)
{
	if(num>0) return num;
	
	num = -num;
	return (unsigned char) num;
}

/***********************************************************************************
* 颜色渐变算法
* 误差 <= 2
************************************************************************************/
u32 ColorToColor(unsigned long color0, unsigned long color1,u16 nus)
{
	unsigned char Red0, Green0, Blue0;  // 起始三原色
	unsigned char Red1, Green1, Blue1;  // 结果三原色
	int			  RedMinus, GreenMinus, BlueMinus;	// 颜色差（color1 - color0）
	unsigned char NStep; 							// 需要几步
	float		  RedStep, GreenStep, BlueStep;		// 各色步进值
	unsigned long color;							// 结果色
	unsigned char i,j;
	// 红 绿 蓝 三原色分解
	Red0 = color0>>16;
	Green0   = color0>>8;	
	Blue0  = color0;
	
	Red1 = color1>>16;
	Green1   = color1>>8;
	Blue1  = color1;
	
	// 计算需要多少步（取差值的最大值）
	RedMinus   = (Red1 - Red0); 
	GreenMinus = (Green1 - Green0); 
	BlueMinus  = (Blue1 - Blue0);
	
	NStep = ( abs0(RedMinus) > abs0(GreenMinus) ) ? abs0(RedMinus):abs0(GreenMinus);
	NStep = ( NStep > abs0(BlueMinus) ) ? NStep:abs0(BlueMinus);
	
	// 计算出各色步进值
	RedStep   = (float)RedMinus   / NStep;
	GreenStep = (float)GreenMinus / NStep;
	BlueStep  = (float)BlueMinus  / NStep;

	// 渐变开始
	for(i=0; i<NStep; i++)
	{
		Red1   = Red0   + (int)(RedStep   * i);
		Green1 = Green0 + (int)(GreenStep * i);
		Blue1  = Blue0  + (int)(BlueStep  * i);
		
		color  = Red1<<16 | Green1<<8 | Blue1; 	// 合成  红绿蓝
		for(j=0; j<WS2812B_AMOUNT; j++)
		{
			Lamp_control_DATA_BUFF[j] = color;  // j / 0
		}
		LED_SPI_Update(Lamp_control_DATA_BUFF,WS2812B_AMOUNT);
		delay_us(nus);						// 渐变速度
	}
	// 渐变结束
	
	return color;
}







