#ifndef __CORE51_EXTI_H
#define __CORE51_EXTI_H

#include "include_drives.h"
#include "delay.h"

sbit Test_ExIT0_k   = P3^2;	//定义P32外部中断按键Test_ExIT0_k
sbit Test_ExIT1_k   = P3^3;	//定义P33外部中断按键Test_ExIT1_k
sbit Test_ExIT0_led = P0^0;	//定义P20口是Test_ExIT0_led
sbit Test_ExIT1_led = P0^1;	//定义P20口是Test_ExIT1_led

#if STC89C52RC
	#if Ex_Interrupt0
		void Ex_Interrupt0_Init(_Parameter_Set *Par_Set);	//外部中断0初始化
		void Test_ExIT0(void);										//外部中断0测试程序
	#endif	

	#if Ex_Interrupt1
		void Ex_Interrupt1_Init(_Parameter_Set *Par_Set);	//外部中断1初始化
		void Test_ExIT1(void);										//外部中断1测试程序
	#endif
#endif

#endif