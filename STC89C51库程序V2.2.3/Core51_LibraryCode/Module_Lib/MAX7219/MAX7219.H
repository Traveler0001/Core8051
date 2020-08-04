#ifndef __MAX7219_H
#define __MAX7219_H

#include "reg52.h"
#include "delay.h"
#include "intrins.h"

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

//此位定义是定义MAX7219引脚
sbit MAX7219_DIN  = P2^0;
sbit MAX7219_LOAD = P2^2;
sbit MAX7219_CLK  = P2^1;

void MAX7219_Init(void);	//初始化MAX7219
void MAX7219_Clear(void);	//清数码管显示
void MAX7219_Test(void);	//数码管测试函数
	
#endif
