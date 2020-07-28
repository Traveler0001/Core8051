#ifndef __CORE51_TIM_H
#define __CORE51_TIM_H

#include "include_drives.h"
#include "delay.h"

#if STC89C52RC
	//外部变量，增强可见域，当其他文件需要使用到定时器时需要拷贝注释中的变量
	//定时器0的重装值配置变量，数值是多少就是多少us，注意最大值为12Mhz最大为65535us 24Mhz下为32767us。
	//定时器1的重装值配置变量，数值是多少就是多少us，注意最大值为65535。
	/*
			extern u16 TX0_Set_Xus;
			extern u16 TX1_Set_Xus;
			extern u8 TH0_Reload;
			extern u8 TL0_Reload;
	*/
	sbit Test_Timer0_led = P1^0;	 //定义P10口是Test_Timer0_led
	sbit Test_Timer1_led = P1^1;	 //定义P11口是Test_Timer1_led

	#if Timer0
		void Timer0_Init(_Parameter_Set *Par_Set);	//定时器0初始化
		void Test_Timer0(void);								//定时器0测试程序
	#endif
	
	#if Timer1
		void Timer1_Init(_Parameter_Set *Par_Set);	//定时器1初始化
		void Test_Timer1(void);								//定时器1测试程序
	#endif
#endif

#endif

