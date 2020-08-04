#ifndef	  __HC_SR04_H_
#define   __HC_SR04_H_

//---包含头文件---//
#include "core51_it.h"
#include "delay.h"
#include "stdio.h"

//---重定义关键词---//
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong  unsigned long
#endif

//---定义使用的IO口---//
sbit 	  TX = P2^6;
sbit 	  RX = P2^7;

float Hc_sr04GetDistance(void);				//超声波测距程序，返回值为测得的距离
void Test_Hc_sr04GetDistance_UART(void);	//超声波测距测试程序

#endif
