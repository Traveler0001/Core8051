#ifndef __LCD_H_
#define __LCD_H_
/**********************************
当使用的是4位数据传输的时候定义，
使用8位取消这个定义
**********************************/
//#define LCD1602_4PINS

/**********************************
包含头文件
**********************************/
#include<reg52.h>
#include "delay.h"

//---重定义关键词---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

#define Lcd1602_Delay1ms Delay_Xms
	
#define LCD1602_DATAPINS P0
sbit 	  LCD1602_E  = P2^7;
sbit    LCD1602_RW = P2^5;
sbit    LCD1602_RS = P2^6;

void LcdWriteCom(uchar com);		//LCD1602写入8位命令子函数	
void LcdWriteData(uchar dat);		//LCD1602写入8位数据子函数
void LcdInit(void);					//LCD1602初始化子程序  
void Test_LCD1602(void);			//测试LCD1602
	
#endif
