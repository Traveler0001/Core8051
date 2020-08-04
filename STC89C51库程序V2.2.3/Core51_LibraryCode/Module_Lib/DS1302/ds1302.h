#ifndef __DS1302_H_
#define __DS1302_H_

//---����ͷ�ļ�---//
#include <reg52.h>
#include <intrins.h>
#include "stdio.h"
#include "string.h"
#include "core51_uart.h"

//---�ض���ؼ���---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

//---����ds1302ʹ�õ�IO��---//
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
	
//---����ȫ�ֺ���---//
void Ds1302Write(uchar addr, uchar dat);			//��DS1302д����ַ+���ݣ�
uchar Ds1302Read(uchar addr);							//��ȡһ����ַ������
unsigned char DEC_BCD(unsigned char DEC);			//ʮ����תBCD��
unsigned char BCD_DEC(unsigned char BCD);			//BCD��תʮ����
void Ds1302Init(DS1302_Time_Struct *Time);		//��ʼ��DS1302
void Ds1302ReadTime(DS1302_Time_Struct *Time);	//��ȡʱ����Ϣ
void Test_ds1302_UART(void);							//DS1302���Գ���

#endif
