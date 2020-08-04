#ifndef __LCD_H_
#define __LCD_H_
/**********************************
��ʹ�õ���4λ���ݴ����ʱ���壬
ʹ��8λȡ���������
**********************************/
//#define LCD1602_4PINS

/**********************************
����ͷ�ļ�
**********************************/
#include<reg52.h>
#include "delay.h"

//---�ض���ؼ���---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

#define Lcd1602_Delay1ms Delay_Xms
	
#define LCD1602_DATAPINS P0
#define LineLength  16      //16x2
sbit	  LCD1602_E  = P2^7;
sbit    LCD1602_RW = P2^6;
sbit    LCD1602_RS = P2^5;

void LcdWriteCom(uchar com);		//LCD1602д��8λ�����Ӻ���	
void LcdWriteData(uchar dat);		//LCD1602д��8λ�����Ӻ���
void LcdInit(void);					//LCD1602��ʼ���ӳ���  
void Test_LCD1602(void);			//����LCD1602
void ClearLine(u8 row);//���һ��
void WriteChar(u8 row, u8 column, u8 dat);//д�ַ�
void PutString(u8 row, u8 column, u8 *puts);//д�ַ���
	
#endif
