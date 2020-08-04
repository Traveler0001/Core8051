#ifndef __DS1302_H_
#define __DS1302_H_

//---包含头文件---//
#include <reg52.h>
#include <intrins.h>
#include "stdio.h"
#include "string.h"
#include "core51_uart.h"

//---重定义关键词---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

//---定义ds1302使用的IO口---//
sbit DSIO = P3^4;
sbit RST  = P3^5;
sbit SCLK = P3^6;

typedef struct{
	unsigned int Year;
	unsigned char Mon;
	unsigned char Day;
	unsigned char Week;
	unsigned char Hou;
	unsigned char Min;
	unsigned char Sec;
}DS1302_Time_Struct;
	
//---定义全局函数---//
void Ds1302Write(uchar addr, uchar dat);			//向DS1302写（地址+数据）
uchar Ds1302Read(uchar addr);							//读取一个地址的数据
unsigned char DEC_BCD(unsigned char DEC);			//十进制转BCD码
unsigned char BCD_DEC(unsigned char BCD);			//BCD码转十进制
void Ds1302Init(DS1302_Time_Struct *Time);		//初始化DS1302
void Ds1302ReadTime(DS1302_Time_Struct *Time);	//读取时钟信息
void Test_ds1302_UART(void);							//DS1302测试程序

#endif
