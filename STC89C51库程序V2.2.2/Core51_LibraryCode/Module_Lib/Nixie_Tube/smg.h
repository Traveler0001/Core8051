#ifndef __SMG_H
#define __SMG_H

#include "reg52.h"
#include "delay.h"

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

//此位定义是定义138译码器译码引脚
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

//定义段码GPIO
#define Seg_codeGPIO P0

void Dig_Display(u8 i,u8 Segment);		//数码管扫描
void Test_smg(void);							//测试数码管和矩阵矩阵按键的程序

#endif
