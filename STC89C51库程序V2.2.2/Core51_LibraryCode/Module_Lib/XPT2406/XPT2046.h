#ifndef	  __XPT2046_H_
#define   __XPT2046_H_

//---����ͷ�ļ�---//
#include <reg52.h>
#include <intrins.h>
#include "sim_spi.h"
#include "core51_uart.h"

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

unsigned int Read_AD_Data(uchar cmd);	//��ȡ��������
void Test_XPT2046_UART(void);				//���Դ���оƬ

#endif





