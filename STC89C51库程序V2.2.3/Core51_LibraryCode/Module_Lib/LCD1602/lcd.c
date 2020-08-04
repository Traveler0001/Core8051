#include "lcd.h"

/*******************************************************************************
* 函 数 名         : LcdWriteCom
* 输    入         : com
* 输    出         :	空
* 函数功能		    : 向LCD写入一个字节的命令
* 详细描述			 :	向LCD写入一个字节的命令
*******************************************************************************/
#ifndef LCD1602_4PINS	 					//当没有定义这个LCD1602_4PINS时
void LcdWriteCom(uchar com)	  			//写入命令
{
	LCD1602_E = 0;    					 	//使能
	LCD1602_RS = 0;	   					//选择发送命令
	LCD1602_RW = 0;	   					//选择写入
	
	LCD1602_DATAPINS = com;     			//放入命令
	Lcd1602_Delay1ms(1);						//等待数据稳定

	LCD1602_E = 1;	          				//写入时序
	Lcd1602_Delay1ms(5);	  					//保持时间
	LCD1602_E = 0;
}
#else 
void LcdWriteCom(uchar com)	  			//写入命令
{
	LCD1602_E = 0;	 							//使能清零
	LCD1602_RS = 0;	 						//选择写入命令
	LCD1602_RW = 0;	 						//选择写入

	LCD1602_DATAPINS = com;					//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 							//写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = com << 4; 			//发送低四位
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 							//写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif

/*******************************************************************************
* 函 数 名         : LcdWriteData
* 输    入         : dat
* 输    出         :	空
* 函数功能		    : 向LCD写入一个字节的数据
* 详细描述			 :	向LCD写入一个字节的数据
*******************************************************************************/
#ifndef LCD1602_4PINS		   
void LcdWriteData(uchar dat)				//写入数据
{
	LCD1602_E = 0;								//使能清零
	LCD1602_RS = 1;							//选择输入数据
	LCD1602_RW = 0;							//选择写入

	LCD1602_DATAPINS = dat; 				//写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   							//写入时序
	Lcd1602_Delay1ms(5);   					//保持时间
	LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)				//写入数据
{
	LCD1602_E = 0;	  							//使能清零
	LCD1602_RS = 1;	  						//选择写入数据
	LCD1602_RW = 0;	  						//选择写入

	LCD1602_DATAPINS = dat;					//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  							//写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; 			//写入低四位
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  							//写入时序
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
  
/*******************************************************************************
* 函 数 名         : LcdInit
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 初始化LCD屏
* 详细描述			 :	初始化LCD屏
*******************************************************************************/
#ifndef LCD1602_4PINS
void LcdInit(void)						  	//LCD初始化子程序
{
 	LcdWriteCom(0x38); 						//开显示
	LcdWriteCom(0x0c);  						//开显示不显示光标
	LcdWriteCom(0x06);  						//写一个指针加1
	LcdWriteCom(0x01);  						//清屏
	LcdWriteCom(0x80);  						//设置数据指针起点
}
#else
void LcdInit(void)						  	//LCD初始化子程序
{
	LcdWriteCom(0x32);	 					//将8位总线转为4位总线
	LcdWriteCom(0x28);	 					//在四位线下的初始化
	LcdWriteCom(0x0c);  						//开显示不显示光标
	LcdWriteCom(0x06);  						//写一个指针加1
	LcdWriteCom(0x01);  						//清屏
	LcdWriteCom(0x80);  						//设置数据指针起点
}
#endif
//========================================================================
// 函数: void ClearLine(u8 row)
// 描述: 清除1行
// 参数: row: 行(0或1)
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void ClearLine(u8 row)
{
    u8 i;
    LcdWriteCom(((row & 1) << 6) | 0x80);
    for(i=0; i<LineLength; i++) LcdWriteData(' ');
}
//========================================================================
// 函数: void   WriteChar(u8 row, u8 column, u8 dat)
// 描述: 指定行、列和字符, 写一个字符
// 参数: row: 行(0或1),  column: 第几个字符(0~15),  dat: 要写的字符.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void WriteChar(u8 row, u8 column, u8 dat)
{
    LcdWriteCom((((row & 1) << 6) + column) | 0x80);
    LcdWriteData(dat);
}
//========================================================================
// 函数: void PutString(u8 row, u8 column, u8 *puts)
// 描述: 写一个字符串，指定行、列和字符串首地址
// 参数: row: 行(0或1),  column: 第几个字符(0~15),  puts: 要写的字符串指针.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void PutString(u8 row, u8 column, u8 *puts)
{
    LcdWriteCom((((row & 1) << 6) + column) | 0x80);
    for ( ;  *puts != 0;  puts++)       //遇到停止符0结束
    {
        LcdWriteData(*puts);
        if(++column >= LineLength)  break;
    }
}
/*******************************************************************************
* 函 数 名         : Test_LCD1602
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 测试LCD1602
* 详细描述			 : LCD1602上显示:LCD Test
*******************************************************************************/
void Test_LCD1602(void)
{
	char code Display_Data[] = "LCD Test";
	
	LcdInit();

	LcdWriteCom(0x80);			//设置显示位置
	LcdWriteData(Display_Data[0]);
	LcdWriteCom(0x80+1);			//设置显示位置
	LcdWriteData(Display_Data[1]);
	LcdWriteCom(0x80+2);			//设置显示位置
	LcdWriteData(Display_Data[2]);
	LcdWriteCom(0x80+3);			//设置显示位置
	LcdWriteData(Display_Data[3]);
	
	LcdWriteCom(0x80+4);			//设置显示位置
	LcdWriteData(Display_Data[4]);
	LcdWriteCom(0x80+5);			//设置显示位置
	LcdWriteData(Display_Data[5]);
	LcdWriteCom(0x80+6);			//设置显示位置
	LcdWriteData(Display_Data[6]);	
	LcdWriteCom(0x80+7);			//设置显示位置
	LcdWriteData(Display_Data[7]);

	while(1);
}