#ifndef __OLED12864_H
#define __OLED12864_H

#include "sim_i2c.h"
#include "delay.h"

#define OLED_Length 128			//OLED横向像素
#define OLED_Width  8			//OLED纵向像素

#define OLED_Center 0			//居中
#define OLED_Left   1			//左
#define OLED_Right  2			//右

extern unsigned char code ASCII7_8[2];

static void OLED12864_WriteData(unsigned char Data);		//向OLED12864写数据
static void OLED12864_WriteCmd(unsigned char Cmd);			//向OLED12864写命令

void OLED12864_Init(void);											//OLED12864初始化
void OLED12864_SetPos(unsigned char X, unsigned char Y);	//向OLED12864写坐标
unsigned char OLED12864_CharMap(unsigned char *FontName,unsigned char Char);	//字符映射

void OLED12864_Fill(unsigned char x0,							//区域清除
					unsigned char y0,
					unsigned char Length,
					unsigned char Width,
					unsigned char Data);
					
void OLED12864_PrintfChar(unsigned char x0,					//打印单个字符
						 unsigned char y0,
						 unsigned char *Font,
						 unsigned char Char);	

void OLED12864_PrintfString(unsigned char x0,				//打印字符串
							unsigned char y0,
							unsigned char *FontName,
							unsigned char Mode,
							unsigned char *Str);		

void Test_OLED12864(void);											//测试OLED12864	
						
#endif
