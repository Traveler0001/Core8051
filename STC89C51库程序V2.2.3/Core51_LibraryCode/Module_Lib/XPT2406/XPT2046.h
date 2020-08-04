#ifndef	  __XPT2046_H_
#define   __XPT2046_H_

//---包含头文件---//
#include <reg52.h>
#include <intrins.h>
#include "sim_spi.h"
#include "core51_uart.h"

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

unsigned int Read_AD_Data(uchar cmd);	//读取触摸数据
void Test_XPT2046_UART(void);				//测试触摸芯片

#endif





