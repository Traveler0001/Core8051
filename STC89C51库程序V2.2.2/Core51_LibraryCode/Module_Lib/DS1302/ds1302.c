#include "ds1302.h"

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
static unsigned char code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
static unsigned char code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

/*******************************************************************************
* 函 数 名         : Ds1302Write
* 输    入         : addr 地址，数据 dat
* 输    出         :	空
* 函数功能		    : 向DS1302写（地址+数据）
* 详细描述			 :	向DS1302写数据或者命令
*******************************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	
	RST = 0;
	_nop_();

	SCLK = 0;								//先将SCLK置低电平。
	_nop_();
	RST = 1; 								//然后将RST(CE)置高电平。
	_nop_();

	for(n = 0;n < 8;n ++)				//开始传送八位地址命令
	{
		DSIO = addr & 0x01;				//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;							//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}
	
	for(n = 0;n < 8;n ++)				//写入8位数据
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;							//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;									//传送数据结束
	_nop_();
}

/*******************************************************************************
* 函 数 名         : Ds1302Read
* 输    入         : addr 地址
* 输    出         :	dat 返回的数据
* 函数功能		    : 读取一个地址的数据
* 详细描述			 :	读取一个地址的数据
*******************************************************************************/
uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	
	RST = 0;
	_nop_();

	SCLK = 0;							//先将SCLK置低电平。
	_nop_();
	RST = 1;								//然后将RST(CE)置高电平。
	_nop_();

	for(n = 0;n < 8;n ++)			//开始传送八位地址命令
	{
		DSIO = addr & 0x01;			//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;						//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;						//DS1302下降沿时，放置数据
		_nop_();
	}
	
	_nop_();
	
	for(n = 0;n < 8;n ++)			//读取8位数据
	{
		dat1 = DSIO;					//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;						//DS1302下降沿时，放置数据
		_nop_();
	}

	RST = 0;
	_nop_();								//以下为DS1302复位的稳定时间,必须的。
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	
	return dat;	
}

/*******************************************************************************
* 函 数 名         : DEC_BCD
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 十进制转BCD码
* 详细描述			 :	空
*******************************************************************************/
unsigned char DEC_BCD(unsigned char DEC)
{
	return ((DEC / 10) << 4) | (DEC % 10); 
}

/*******************************************************************************
* 函 数 名         : BCD_DEC
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : BCD码转十进制
* 详细描述			 :	空
*******************************************************************************/
unsigned char BCD_DEC(unsigned char BCD)
{
	return ((BCD >> 4) * 10) + (BCD & 0x0F); 
}

/*******************************************************************************
* 函 数 名         : Ds1302Init
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 初始化DS1302
* 详细描述			 :	初始化DS1302
*******************************************************************************/
void Ds1302Init(DS1302_Time_Struct *Time)
{
	unsigned char n;
	unsigned char Time_Buffer[7];
	
	Time_Buffer[0] = Time->Sec;
	Time_Buffer[1] = Time->Min;
	Time_Buffer[2] = Time->Hou;
	Time_Buffer[3] = Time->Day;
	Time_Buffer[4] = Time->Mon;
	Time_Buffer[5] = Time->Week;
	Time_Buffer[6] = Time->Year - 2000;
	
	Ds1302Write(0x8E,0X00);		//禁止写保护，就是关闭写保护功能
	
	for (n = 0; n < 7; n ++)	//写入7个字节的时钟信号：秒分时日月周年
		Ds1302Write(WRITE_RTC_ADDR[n],DEC_BCD(Time_Buffer[n]));	

	Ds1302Write(0x8E,0x80);		//打开写保护功能
}

/*******************************************************************************
* 函 数 名         : Ds1302ReadTime
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 读取时钟信息
* 详细描述			 :	读取时钟信息，并放在TIME的数组中，且先后顺序为秒分时日月周年
*******************************************************************************/
void Ds1302ReadTime(DS1302_Time_Struct *Time)
{
	unsigned char n;
	unsigned char Time_Buffer[7];
	
	for (n = 0;n < 7;n ++ )		//读取7个字节的时钟信号：秒分时日月周年
		Time_Buffer[n] = BCD_DEC(Ds1302Read(READ_RTC_ADDR[n]));
	
	Time->Sec = Time_Buffer[0];
	Time->Min = Time_Buffer[1];
	Time->Hou = Time_Buffer[2];
	Time->Day = Time_Buffer[3];
	Time->Mon = Time_Buffer[4];
	Time->Week = Time_Buffer[5];
	Time->Year = Time_Buffer[6] + 2000;
}

/*******************************************************************************
* 函 数 名         : Test_ds1302_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 测试DS1302
* 详细描述			 :	读取时间并在通过串口打印显示
*******************************************************************************/
void Test_ds1302_UART(void)
{
	char DisplayData[18];
	DS1302_Time_Struct DS1302_Time;

	DS1302_Time.Year = 2020;
	DS1302_Time.Mon  = 1;
	DS1302_Time.Day  = 20;
	DS1302_Time.Week = 1;
	DS1302_Time.Hou  = 0;
	DS1302_Time.Min  = 32;
	DS1302_Time.Sec  = 0;
	Ds1302Init(&DS1302_Time);
	
	while(1)
	{
   	Ds1302ReadTime(&DS1302_Time);

		DisplayData[0] = DS1302_Time.Year / 1000;		//年
		DisplayData[1] = DS1302_Time.Year % 1000 / 100;
		DisplayData[2] = DS1302_Time.Year % 100 / 10;				
		DisplayData[3] = DS1302_Time.Year % 10;
		DisplayData[4] ='-';
		DisplayData[5] = DS1302_Time.Mon / 10;			//月
		DisplayData[6] = DS1302_Time.Mon % 10;	
		DisplayData[7] ='-';
		DisplayData[8] = DS1302_Time.Day / 10;			//日
		DisplayData[9] = DS1302_Time.Day % 10;	
		DisplayData[10] = ' ';
		DisplayData[1] = DS1302_Time.Hou / 10;			//时
		DisplayData[12] = DS1302_Time.Hou % 10;				 
		DisplayData[13] ='-';
		DisplayData[14] = DS1302_Time.Min / 10;		//分
		DisplayData[15] = DS1302_Time.Min % 10;	
		DisplayData[16] ='-';
		DisplayData[17] = DS1302_Time.Sec / 10;		//秒
		DisplayData[18] = DS1302_Time.Sec % 10;
		
		#if Uart
		UART1_Send_Data(DisplayData,18);
		#endif
		
		Delay_Xms(500);
	}
}
