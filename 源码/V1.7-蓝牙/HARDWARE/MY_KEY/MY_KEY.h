#ifndef __MY_KEY_H
#define __MY_KEY_H
#include "sys.h"

#define Shaking_time 8 //����������ʱ��8*��ʱ�ж�ʱ��

#define KEY1 PBin(5)   	
#define KEY2 PBin(10)	 
#define KEY3 PBin(11)	

typedef struct
{
	u8 KEY_STA;//��ǰ����״̬
	
	u8 Previous_STA;//������һ��״̬
	
	u8 Counter;//������
	
	u8 KEY_flag;//������־ 0δ���£����� 1�Ѱ��� 2���� 
	
	u8 key_value;//��ֵ����
	
	u8 KEY_Event;//��¼�����¼� 0�Ѵ������¼� 1����δ����İ����¼�
	u8 KEY_Event_type;//�������� 1�̰� 2���� 
	u8 KEY_Event_content;//����ֵ
	/*************************************************************
	//�������ӳ���
		0x00,				0x04,				0x02,				0x01,				0x06,				0x05,				0x03,				0x07
	//Ϊ����			KEY3����		KEY2����		KEY1����		KEY3��2����	KEY3��1����	KEY1��2����	KEY3��2��1����
	*************************************************************///
	
	
}My_key_InitTypeDef;

#ifndef __MY_KEY_C

extern My_key_InitTypeDef MY_KEY;

#endif
//����IO��ʼ��
void KEY_Init(void);
//����ɨ�躯��--�����жϵ���
void KEY_Scan(void);

#endif











