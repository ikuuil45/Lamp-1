#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
 
 				  	    													  
void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void SPI_Transmit_DATA(u8 *spi_tx,u16 spi_len);
		 
#endif

