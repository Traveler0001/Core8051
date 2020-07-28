#ifndef __HARDWARE_DRIVERS_H
#define __HARDWARE_DRIVERS_H

//���������ͽ�����������
typedef unsigned int u16;	  
typedef unsigned char u8;

//---�ض���ؼ���---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

#if 	STC89C52RC

typedef struct{
	//��ʱ��ģʽ���ñ���
	u8 TMOD_Set;				//��ʱ��ģʽ����
	//���ж����ñ���
	u8	EA_Set;					//���ж��������
	
	//�ⲿ�ж�0��Ƕ�ṹ��
#if Ex_Interrupt0
	struct{
		u8 IT0_Set:1;			//����������λ
		u8 EX0_Set:1;			//�ⲿ�ж�0��������λ
	}Ex_Int0_Parameter;
#endif	
	//�ⲿ�ж�1��Ƕ�ṹ��
#if Ex_Interrupt1
	struct{
		u8 IT1_Set:1;			//����������Ϊ
		u8 EX1_Set:1;			//�ⲿ�ж�1��������Ϊ
	}Ex_Int1_Parameter;
#endif	
	//��ʱ����������ж�0��Ƕ�ṹ��
#if Timer0 
	struct{
		u8 ET0_Set:1;			//�Ƿ�򿪶�ʱ��0�ж�����λ
		u8 TR0_Set:1;			//�Ƿ�򿪶�ʱ��λ
	}Timer0_Parameter;
#endif	
	//��ʱ����������ж�0��Ƕ�ṹ��
#if (Timer1 || Uart)
	struct{
		u8 ET1_Set:1;			//�Ƿ�򿪶�ʱ��0�ж�����
		u8 TR1_Set:1;			//�Ƿ�򿪶�ʱ��λ	
	}Timer1_Parameter;
#endif	
	//�����ж���Ƕ�ṹ��
#if Uart 
	struct{
		u8 SCON_Set;			//����Ϊ���ڹ�����ʽ����
		u8 PCON_Set;			//��Դ����Ĵ���
		u8 ES_Set;				//���ڽ����жϱ���
		float Baud_Rate;		//�����ʲ������ñ���
	}Usart_Parameter;
#endif
}_Parameter_Set;

#endif

#endif
