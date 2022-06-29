#ifndef __Bluetooth_H
#define __Bluetooth_H	 
#include "sys.h"
#include "led.h"
		 			
#define Bluetooth_REC_LEN  			(u16)32  	//定义最大接收字节数 32

typedef struct {

	u8 Rx_Buffer[Bluetooth_REC_LEN]; //接收数据缓存
	u16 Rx_len;//收到数据的长度
	u8 STA;//接收完成标志
}INIT_Bluetooth;

#ifndef __Bluetooth_C

extern INIT_Bluetooth Bluetooth_DATA;
extern INIT_Bluetooth Bluetooth_DATA_BUFF;
#endif

void Bluetooth_init(u32 bound);
//Bluetooth发送一个字节
void Bluetooth_send_byte(u8 val);
//Bluetooth发送一个字符串
void Bluetooth_send_str(u8 *str,u8 Bluetooth_len);



#endif

