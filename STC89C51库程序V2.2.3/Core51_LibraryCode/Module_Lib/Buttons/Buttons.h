#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "delay.h"
#include "reg52.h"
#include "core51_uart.h"
#include "stdio.h"

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
//���ö�������ʹ�õ�GPIO
sbit KEY1 = P1^4;
sbit KEY2 = P1^5;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;
	
u8 Matrix_Buttons(u8 Sustain);				//���󰴼����
u8 Separate_Buttons(u8 Sustain);				//�����������
void Test_Matrix_Buttons_UART(void);		//���󰴼����Գ���
void Test_Separate_Buttons_UART(void);		//���󰴼����Գ���
	
#endif
	