#ifndef __SIM_SPI_H
#define __SIM_SPI_H

#include "include_drives.h"

#if STC89C52RC
	//---包含头文件---//
	#include <reg52.h>
	#include <intrins.h>

	//---重定义关键词---//
	#ifndef uchar
		#define uchar unsigned char
	#endif
		
	#ifndef uint
		#define uint  unsigned int
	#endif
		
	#ifndef ulong
		#define ulong unsigned long
	#endif

	//---定义SPI使用的IO口---//
	sbit DOUT = P3^4;	  			//输出
	sbit CLK  = P3^6;	  			//时钟
	sbit DIN  = P3^7;	  			//输入
	sbit CS   = P3^5;	  			//片选

	#define SPI_Delay 			//SPI时序延时宏定义，STC89C52速度
										//慢所以直接宏定义一个空的延时

	//-----------SPI的实现函数-----------//
	void SPI_Start(void);		//SPI初始化
	uint SPI_Read(void);			//SPI读
	void SPI_Write(uchar dat);	//SPI写
#endif
		
#endif
		