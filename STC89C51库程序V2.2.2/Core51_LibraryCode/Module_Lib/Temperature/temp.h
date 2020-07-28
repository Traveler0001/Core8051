#ifndef __TEMP_H_
#define __TEMP_H_

#include<reg52.h>
#include "delay.h"
#include "core51_uart.h"

//---�ض���ؼ���---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

#define  Delay1ms Delay_Xms

sbit DSPORT = P3^7;

uchar Ds18b20Init(void);				//DS18B20��ʼ��
void  Ds18b20WriteByte(uchar com);	//дһ���ֽ�
uchar Ds18b20ReadByte(void);			//��ȡһ���ֽ�
void  Ds18b20ChangTemp(void);			//��18b20��ʼת���¶�
void  Ds18b20ReadTempCom(void);		//��ȡ�¶ȵ�����
int   Ds18b20ReadTemp(void);			//��ȡ�¶�
void  Test_Temp_UART(void);			//DS18B20���Գ���
		
#endif
