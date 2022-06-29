#include "MY_KEY.h"
#define __MY_KEY_C
/*************************************************************
//�������ӳ���
	0x00,				0x04,				0x02,				0x01,				0x06,				0x05,				0x03,				0x07
//δ����			KEY3����		KEY2����		KEY1����		KEY3��2����	KEY3��1����	KEY1��2����	KEY3��2��1����
*************************************************************///

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
 
} 

//���尴����������ʼ�� 
My_key_InitTypeDef MY_KEY = {0};

//����������--��Ҫ�ڶ�ʱ���е��� �Ƽ���ʱ��ʱ��10ms
//֧�� �̰� ���� ��ϰ�
void KEY_Scan(void)
{	 
	
	/*****���°���״ֵ̬****/
	if(KEY1==0) MY_KEY.KEY_STA |= 0x01;//��һλ��һ
		else MY_KEY.KEY_STA &= 0xfe;//��һλ����
	if(KEY2==0) MY_KEY.KEY_STA |= 0x02;//�ڶ�λ��һ
		else MY_KEY.KEY_STA &= 0xfd;//�ڶ�λ����
	if(KEY3==0) MY_KEY.KEY_STA |= 0x04;//����λ��һ
		else MY_KEY.KEY_STA &= 0xfb;//����λ����
	
	/******�ж�******/
	if(MY_KEY.KEY_STA!=MY_KEY.Previous_STA)//����һ״̬�����
	{
		MY_KEY.Counter = 0;//����������
		MY_KEY.Previous_STA = MY_KEY.KEY_STA;//����״ֵ̬
	}
	else //����һ��״̬���
	{
		if(MY_KEY.Counter<200)	//С��2S
		{
			MY_KEY.Counter++;//����������
			if(MY_KEY.Counter==Shaking_time) //����ʱ����ڰ�������ʱ�䣬��Ϊ�������ȶ�
			{
				if(MY_KEY.KEY_STA==0x00)//�Ƿ��ǰ�������
				{
					MY_KEY.Counter = 0;//����������
					
						if(MY_KEY.KEY_flag==1)//�¼�ˢ��
						{
							MY_KEY.KEY_Event = 1;//����¼�
							MY_KEY.KEY_Event_type = 1;//����¼�����
							MY_KEY.KEY_Event_content = MY_KEY.key_value;//����¼�����
						}

					MY_KEY.KEY_flag = 0;//��¼��ʱ����״̬  ����
					MY_KEY.key_value = 0;//��������
				}
				else 
				{
					MY_KEY.KEY_flag = 1;//��¼��ʱ����״̬  �̰� ����
					if(MY_KEY.key_value<MY_KEY.KEY_STA) MY_KEY.key_value = MY_KEY.KEY_STA;//ˢ��ֻ��¼��ϼ����ֵ
//					MY_KEY.key_value = MY_KEY.KEY_STA;//ˢ�¼�¼�ͷŰ���ʱ��ֵ
				}
				
			}
		}
		else //����ʱ�䳬��2S���ж�Ϊ����
		{
				if(MY_KEY.KEY_flag!=2)//���δ˾��������
				{
					MY_KEY.KEY_Event = 1;//����¼�
					MY_KEY.KEY_Event_type = 2;//����¼�����
					MY_KEY.KEY_Event_content = MY_KEY.key_value;//����¼�����
					MY_KEY.KEY_flag = 2;//��¼��ʱ����״̬  ����
				}
		}
	}

	
}











