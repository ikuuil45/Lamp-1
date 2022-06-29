#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"

void SPI1_Init(void);
u8 SPI1_WriteByte(u8 byte);
u8 SPI1_ReadByte(void);

#endif /* __SPI_FLASH_H */

