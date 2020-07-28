#ifndef __MATRIX_BUTTONS_H
#define __MATRIX_BUTTONS_H

#include "delay.h"
#include "reg52.h"
#include "core51_uart.h"

//---�ض���ؼ���---//
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

//���þ��󰴼�ʹ�õ�GPIO��
#define 	GPIO_KEY 	P1

u8 Matrix_Buttons(u8 Sustain);				//���󰴼����
void Test_Matrix_Buttons_UART(void);		//���󰴼����Գ���
	
#endif