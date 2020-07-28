#ifndef __CORE51_TIM_H
#define __CORE51_TIM_H

#include "include_drives.h"
#include "delay.h"

#if STC89C52RC
	//�ⲿ��������ǿ�ɼ��򣬵������ļ���Ҫʹ�õ���ʱ��ʱ��Ҫ����ע���еı���
	//��ʱ��0����װֵ���ñ�������ֵ�Ƕ��پ��Ƕ���us��ע�����ֵΪ12Mhz���Ϊ65535us 24Mhz��Ϊ32767us��
	//��ʱ��1����װֵ���ñ�������ֵ�Ƕ��پ��Ƕ���us��ע�����ֵΪ65535��
	/*
			extern u16 TX0_Set_Xus;
			extern u16 TX1_Set_Xus;
			extern u8 TH0_Reload;
			extern u8 TL0_Reload;
	*/
	sbit Test_Timer0_led = P1^0;	 //����P10����Test_Timer0_led
	sbit Test_Timer1_led = P1^1;	 //����P11����Test_Timer1_led

	#if Timer0
		void Timer0_Init(_Parameter_Set *Par_Set);	//��ʱ��0��ʼ��
		void Test_Timer0(void);								//��ʱ��0���Գ���
	#endif
	
	#if Timer1
		void Timer1_Init(_Parameter_Set *Par_Set);	//��ʱ��1��ʼ��
		void Test_Timer1(void);								//��ʱ��1���Գ���
	#endif
#endif

#endif

