#ifndef __MATRIX_BUTTONS_H
#define __MATRIX_BUTTONS_H

#include "delay.h"
#include "reg52.h"
#include "core51_uart.h"

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

u8 Matrix_Buttons(u8 Sustain);				//矩阵按键检测
void Test_Matrix_Buttons_UART(void);		//矩阵按键测试程序
	
#endif