#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
 
 				  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void SPI_Transmit_DATA(u8 *spi_tx,u16 spi_len);
		 
#endif

