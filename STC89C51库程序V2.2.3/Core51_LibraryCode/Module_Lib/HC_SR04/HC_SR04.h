#ifndef	  __HC_SR04_H_
#define   __HC_SR04_H_

//---����ͷ�ļ�---//
#include "core51_it.h"
#include "delay.h"
#include "stdio.h"

//---�ض���ؼ���---//
//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong  unsigned long
#endif

//---����ʹ�õ�IO��---//
sbit 	  TX = P2^6;
sbit 	  RX = P2^7;

float Hc_sr04GetDistance(void);				//�����������򣬷���ֵΪ��õľ���
void Test_Hc_sr04GetDistance_UART(void);	//�����������Գ���

#endif
