#include "OLED12864.H"
#include "OLEDFont.H"

unsigned char code ASCII7_8[2] = {7,8};	//7*8 ASCII字库

/*******************************************************************************
* 函 数 名         : OLED12864_WriteData
* 输    入         : Data：需要写进去的数据
* 输    出         :	空
* 函数功能		    : 向OLED12864写数据
* 详细描述			 :	空
*******************************************************************************/
static void OLED12864_WriteData(unsigned char Data)
{
	I2c_Start();
	I2c_SendByte(0x78);
	I2c_SendByte(0x40);
	I2c_SendByte(Data);
	I2c_Stop();
}

/*******************************************************************************
* 函 数 名         : OLED12864_WriteCmd
* 输    入         : Data：需要写进去的命令
* 输    出         :	空
* 函数功能		    : 向OLED12864写命令
* 详细描述			 :	空
*******************************************************************************/
static void OLED12864_WriteCmd(unsigned char Cmd)
{
	I2c_Start();
	I2c_SendByte(0x78);
	I2c_SendByte(0x00);
	I2c_SendByte(Cmd);
	I2c_Stop();
}

/*******************************************************************************
* 函 数 名         : OLED12864_Init
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : OLED12864初始化
* 详细描述			 :	空
*******************************************************************************/
void OLED12864_Init(void)
{               
	OLED12864_WriteCmd(0xAE); 	//display off
	OLED12864_WriteCmd(0x20);	//Set Memory Addressing Mode	
	OLED12864_WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED12864_WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED12864_WriteCmd(0xc8);	//Set COM Output Scan Direction
	OLED12864_WriteCmd(0x00); 	//---set low column address
	OLED12864_WriteCmd(0x10); 	//---set high column address
	OLED12864_WriteCmd(0x40); 	//--set start line address
	OLED12864_WriteCmd(0x81); 	//--set contrast control register
	OLED12864_WriteCmd(0xff); 	//áá?èμ÷?ú 0x00~0xff
	OLED12864_WriteCmd(0xa1); 	//--set segment re-map 0 to 127
	OLED12864_WriteCmd(0xa6); 	//--set normal display
	OLED12864_WriteCmd(0xa8); 	//--set multiplex ratio(1 to 64)
	OLED12864_WriteCmd(0x3F); 	//
	OLED12864_WriteCmd(0xa4); 	//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED12864_WriteCmd(0xd3); 	//-set display offset
	OLED12864_WriteCmd(0x00); 	//-not offset
	OLED12864_WriteCmd(0xd5); 	//--set display clock divide ratio/oscillator frequency
	OLED12864_WriteCmd(0xf0); 	//--set divide ratio
	OLED12864_WriteCmd(0xd9); 	//--set pre-charge period
	OLED12864_WriteCmd(0x22); 	//
	OLED12864_WriteCmd(0xda); 	//--set com pins hardware configuration
	OLED12864_WriteCmd(0x12);
	OLED12864_WriteCmd(0xdb); 	//--set vcomh
	OLED12864_WriteCmd(0x20); 	//0x20,0.77xVcc
	OLED12864_WriteCmd(0x8d); 	//--set DC-DC enable
	OLED12864_WriteCmd(0x14); 	//
	OLED12864_WriteCmd(0xaf); 	//--turn on oled panel
	OLED12864_Fill(0,0,128,8,0x00);                              
	OLED12864_SetPos(0,0);
}

/*******************************************************************************
* 函 数 名         : OLED12864_SetPos
* 输    入         : X：X坐标
*					 		Y：Y坐标
*
* 输    出         :	空
* 函数功能		    : 向OLED12864写坐标
* 详细描述			 :	空
*******************************************************************************/
void OLED12864_SetPos(unsigned char X, unsigned char Y)
{ 
	OLED12864_WriteCmd(0xB0 + Y);
	OLED12864_WriteCmd(((X & 0xF0) >> 4) | 0x10);
	OLED12864_WriteCmd((X & 0x0F) | 0x01);
} 

/*******************************************************************************
* 函 数 名         : OLED12864_Fill
* 输    入         : x0：x坐标原点
*						 	y0：y坐标原点
*					 		Lenght：长度
*					 		Wight：宽度
*					 		Data：底色
*
* 输    出         :	空
* 函数功能		    : 区域清除
* 详细描述			 :	空
*******************************************************************************/
void OLED12864_Fill(unsigned char x0,
					unsigned char y0,
					unsigned char Length,
					unsigned char Width,
					unsigned char Data)
{
	unsigned char x = 0,y = 0;
	
	if( (Width == 0) || 
		(Length == 0) || 
		(x0 >= OLED_Length) || 
		(y0 >= OLED_Width))
		return;

	for(y = 0;y < Width;y ++)
	{
		if((y0 + y) >= OLED_Width)
			return;
			
		OLED12864_SetPos(x0,y0 + y);
	
		for(x = 0;x < Length;x ++)
		{
			if((x0 + x) >= OLED_Length)
				break;
			
			OLED12864_WriteData(Data);
		}
	}
}

/*******************************************************************************
* 函 数 名         : OLED12864_CharMap
* 输    入         : 空
* 输    出         :	对应字符在字库中的编号
* 函数功能		    : 字符映射
* 详细描述			 :	空
*******************************************************************************/
unsigned char OLED12864_CharMap(unsigned char *FontName,unsigned char Char)
{
	if(FontName == ASCII7_8)
	{
		switch(Char)
		{
			case '!':return 0;
			case '"':return 1;
			case '#':return 2;
			case '$':return 3;
			case '%':return 4;
			case '&':return 5;
			case '\'':return 6;
			case '(':return 7;
			case ')':return 8;
			case '*':return 9;
			case '+':return 10;
			case ',':return 11;
			case '-':return 12;
			case '.':return 13;
			case '/':return 14;
			case '0':return 15;
			case '1':return 16;
			case '2':return 17;
			case '3':return 18;
			case '4':return 19;
			case '5':return 20;
			case '6':return 21;
			case '7':return 22;
			case '8':return 23;
			case '9':return 24;
			case ':':return 25;
			case ';':return 26;
			case '<':return 27;
			case '=':return 28;
			case '>':return 29;
			case '?':return 30;
			case '@':return 31;
			case 'A':return 32;
			case 'B':return 33;
			case 'C':return 34;
			case 'D':return 35;
			case 'E':return 36;
			case 'F':return 37;
			case 'G':return 38;
			case 'H':return 39;
			case 'I':return 40;
			case 'J':return 41;
			case 'K':return 42;
			case 'L':return 43;
			case 'M':return 44;
			case 'N':return 45;
			case 'O':return 46;
			case 'P':return 47;
			case 'Q':return 48;
			case 'R':return 49;
			case 'S':return 50;
			case 'T':return 51;
			case 'U':return 52;
			case 'V':return 53;
			case 'W':return 54;
			case 'X':return 55;
			case 'Y':return 56;
			case 'Z':return 57;
			case '[':return 58;
			case '\\':return 59;
			case ']':return 60;
			case '^':return 61;
			case '_':return 62;
			case '`':return 63;
			case 'a':return 64;
			case 'b':return 65;
			case 'c':return 66;
			case 'd':return 67;
			case 'e':return 68;
			case 'f':return 69;
			case 'g':return 70;
			case 'h':return 71;
			case 'i':return 72;
			case 'j':return 73;
			case 'k':return 74;
			case 'l':return 75;
			case 'm':return 76;
			case 'n':return 77;
			case 'o':return 78;
			case 'p':return 79;
			case 'q':return 80;
			case 'r':return 81;
			case 's':return 82;
			case 't':return 83;
			case 'u':return 84;
			case 'v':return 85;
			case 'w':return 86;
			case 'x':return 87;
			case 'y':return 88;
			case 'z':return 89;
			case '{':return 90;
			case '|':return 91;
			case '}':return 92;
			case '~':return 93;
			case ' ':return 94;
		}
	}
	
	return 0;
}

/*******************************************************************************
* 函 数 名         : OLED12864_PrintfChar
* 输    入         : x0：x坐标原点
*					 		y0：y坐标原点
*					 		FontName：字库名称
*					 		Char：需要显示的字符
*
* 输    出         :	空
* 函数功能		    : 打印单个字符
* 详细描述			 :	空
*******************************************************************************/
void OLED12864_PrintfChar(unsigned char x0,
						 unsigned char y0,
						 unsigned char *FontName,
						 unsigned char Char)
{
	unsigned char x = 0,y = 0,Width = 0;

	if((x0 >= OLED_Length) || (y0 >= OLED_Width))
		return;

	if((FontName[1] % 8) != 0)
		Width = (FontName[1] / 8) + 1;
	else
		Width = FontName[1] / 8;

	for(y = 0;y < Width;y ++)
	{
		OLED12864_SetPos(x0,y0 + y);
	
		for(x = 0;x < FontName[0];x ++)
			OLED12864_WriteData(ASCII_7_8[OLED12864_CharMap(FontName,Char)][x]);
	}
}

/*******************************************************************************
* 函 数 名         : OLED12864_PrintfChar
* 输    入         : x0：x坐标原点
*					 		y0：y坐标原点
*					 		FontName：字库名称
*							Mode：显示模式{0->居中对齐   1->左对齐   2->右对齐}
*					 		Str：需要显示的字符串
*
* 输    出         :	空
* 函数功能		    : 打印字符串
* 详细描述			 :	空
*******************************************************************************/
void OLED12864_PrintfString(unsigned char x0,
							unsigned char y0,
							unsigned char *FontName,
							unsigned char Mode,
							unsigned char *Str)
{
	unsigned char Count = 0;
	
	switch (Mode)	
	{
		case 0:	//居中对齐
			for(Count = 0;(Str[Count] != '\0') && (Count < 255);Count ++);	//计算字符串长度
			Count *= (FontName[0] + 1);										//计算占用的屏幕横向像素个数
			
			if((Count + x0) < OLED_Length)									//判断是否有剩余横向像素
				x0 += (OLED_Length - x0 - Count) / 2;						//计算居中偏移量
		break;
			
		case 1:	//左对齐模式
			
		break;
		
		case 2:	//右对齐模式
			for(Count = 0;(Str[Count] != '\0') && (Count < 255);Count ++);	//计算字符串长度
			Count *= (FontName[0] + 1);										//计算占用的屏幕横向像素个数
			
			if((Count + x0) < OLED_Length)									//判断是否有剩余横向像素
				x0 += (OLED_Length - x0 - Count);							//计算右对齐偏移量
		break;
	}
	
	for(Count = 0;(Str[Count] != '\0') && (Count < 255);Count ++)
	{
		OLED12864_PrintfChar(x0,y0,FontName,Str[Count]);
		x0 += FontName[0] + 1;
	}
}

/*******************************************************************************
* 函 数 名         : Test_OLED12864
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 测试OLED12864
* 详细描述			 :	在屏幕中央打印“OLED Test!”
*******************************************************************************/
void Test_OLED12864(void)
{
	OLED12864_Init();									//OLED12864初始化
	OLED12864_PrintfString(0,4,ASCII7_8,OLED_Center,"OLED Test!");	
	while(1);
}
