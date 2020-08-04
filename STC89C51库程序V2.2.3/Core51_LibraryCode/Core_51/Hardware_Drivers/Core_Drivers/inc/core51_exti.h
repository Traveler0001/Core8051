#ifndef __CORE51_EXTI_H
#define __CORE51_EXTI_H

#include "include_drives.h"
#include "delay.h"

sbit Test_ExIT0_k   = P3^2;	//����P32�ⲿ�жϰ���Test_ExIT0_k
sbit Test_ExIT1_k   = P3^3;	//����P33�ⲿ�жϰ���Test_ExIT1_k
sbit Test_ExIT0_led = P0^0;	//����P20����Test_ExIT0_led
sbit Test_ExIT1_led = P0^1;	//����P20����Test_ExIT1_led

#if STC89C52RC
	#if Ex_Interrupt0
		void Ex_Interrupt0_Init(_Parameter_Set *Par_Set);	//�ⲿ�ж�0��ʼ��
		void Test_ExIT0(void);										//�ⲿ�ж�0���Գ���
	#endif	

	#if Ex_Interrupt1
		void Ex_Interrupt1_Init(_Parameter_Set *Par_Set);	//�ⲿ�ж�1��ʼ��
		void Test_ExIT1(void);										//�ⲿ�ж�1���Գ���
	#endif
#endif

#endif