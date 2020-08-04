#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "delay.h"
#include "reg52.h"
#include "core51_uart.h"
#include "stdio.h"

//---重定义关键词---//
#ifndef u8
	#define u8 unsigned char
#endif
	
#ifndef u16 
	#define u16 unsigned int
#endif
	
#ifndef uchar
	#define uchar unsigned char
#endif
	
#ifndef uint 
	#define uint unsigned int
#endif

//设置矩阵按键使用的GPIO组
#define 	GPIO_KEY 	P1
//设置独立按键使用的GPIO
sbit KEY1 = P1^4;
sbit KEY2 = P1^5;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;
	
u8 Matrix_Buttons(u8 Sustain);				//矩阵按键检测
u8 Separate_Buttons(u8 Sustain);				//独立按键检测
void Test_Matrix_Buttons_UART(void);		//矩阵按键测试程序
void Test_Separate_Buttons_UART(void);		//矩阵按键测试程序
	
#endif
	