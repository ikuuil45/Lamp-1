#ifndef __Bluetooth_H
#define __Bluetooth_H	 
#include "sys.h"
#include "led.h"
		 			
#define Bluetooth_REC_LEN  			(u16)32  	//�����������ֽ��� 32

typedef struct {

	u8 Rx_Buffer[Bluetooth_REC_LEN]; //�������ݻ���
	u16 Rx_len;//�յ����ݵĳ���
	u8 STA;//������ɱ�־
}INIT_Bluetooth;

#ifndef __Bluetooth_C

extern INIT_Bluetooth Bluetooth_DATA;
extern INIT_Bluetooth Bluetooth_DATA_BUFF;
#endif

void Bluetooth_init(u32 bound);
//Bluetooth����һ���ֽ�
void Bluetooth_send_byte(u8 val);
//Bluetooth����һ���ַ���
void Bluetooth_send_str(u8 *str,u8 Bluetooth_len);



#endif

