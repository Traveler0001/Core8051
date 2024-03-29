#ifndef __OLED12864_H
#define __OLED12864_H

#include "sim_i2c.h"
#include "delay.h"

/*ѡ��OLED��ĻͨѶ��ʽ*/
#define OLED_SPI 0
#define OLED_IIC 1
#define OLED_CtrlType OLED_SPI

#if (OLED_CtrlType == OLED_SPI)
	#include "sim_spi.h"
	sbit OLED12864_RST = P2^2;
	sbit OLED12864_DC  = P2^1;
#endif

#define OLED_Length 128			//OLED��������
#define OLED_Width  8			//OLED��������

#define OLED_Center 0			//����
#define OLED_Left   1			//��
#define OLED_Right  2			//��

extern unsigned char code ASCII7_8[2];

static void OLED12864_WriteData(unsigned char Data);		//��OLED12864д����
static void OLED12864_WriteCmd(unsigned char Cmd);			//��OLED12864д����

void OLED12864_Init(void);											//OLED12864��ʼ��
void OLED12864_SetPos(unsigned char X, unsigned char Y);	//��OLED12864д����
unsigned char OLED12864_CharMap(unsigned char *FontName,unsigned char Char);	//�ַ�ӳ��

void OLED12864_Fill(unsigned char x0,							//�������
					unsigned char y0,
					unsigned char Length,
					unsigned char Width,
					unsigned char Data);
					
void OLED12864_PrintfChar(unsigned char x0,					//��ӡ�����ַ�
						 unsigned char y0,
						 unsigned char *Font,
						 unsigned char Char);	

void OLED12864_PrintfString(unsigned char x0,				//��ӡ�ַ���
							unsigned char y0,
							unsigned char *FontName,
							unsigned char Mode,
							unsigned char *Str);		

void Test_OLED12864(void);											//����OLED12864	
						
#endif
