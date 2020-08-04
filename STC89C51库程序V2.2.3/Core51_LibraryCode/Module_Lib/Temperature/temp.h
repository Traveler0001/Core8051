#ifndef __TEMP_H_
#define __TEMP_H_

#include<reg52.h>
#include "delay.h"
#include "core51_uart.h"

//---重定义关键词---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

#define  Delay1ms Delay_Xms

sbit DSPORT = P3^7;

uchar Ds18b20Init(void);				//DS18B20初始化
void  Ds18b20WriteByte(uchar com);	//写一个字节
uchar Ds18b20ReadByte(void);			//读取一个字节
void  Ds18b20ChangTemp(void);			//让18b20开始转换温度
void  Ds18b20ReadTempCom(void);		//读取温度的命令
int   Ds18b20ReadTemp(void);			//读取温度
void  Test_Temp_UART(void);			//DS18B20测试程序
		
#endif
