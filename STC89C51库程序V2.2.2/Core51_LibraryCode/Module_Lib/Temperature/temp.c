#include "temp.h"

/*******************************************************************************
* 函 数 名         : Ds18b20Init
* 输    入         : 空
* 输    出         :	初始化成功返回1，失败返回0
* 函数功能		    : DS18B20初始化
* 详细描述			 : 初始化函数
*******************************************************************************/
uchar Ds18b20Init(void)
{
	uchar i;
	
	DSPORT = 0;			 			//将总线拉低480us~960us
	
	i = 70;	
	while(i--);						//延时642us
	
	DSPORT = 1;						//然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
	
	i = 0;
	while(DSPORT)					//等待DS18B20拉低总线
	{
		Delay1ms(1);
		i ++;
		if(i > 5)					//等待>5MS
			return 0;				//初始化失败
	}
	
	return 1;						//初始化成功
}

/*******************************************************************************
* 函 数 名         : Ds18b20WriteByte
* 输    入         : 需要写入的数据
* 输    出         :	空
* 函数功能		    : 写字节函数
* 详细描述			 : 向18B20写入一个字节
*******************************************************************************/
void Ds18b20WriteByte(uchar dat)
{
	uint i, j;

	for(j = 0;j < 8;j ++)
	{
		DSPORT = 0;	     	  		//每写入一位数据之前先把总线拉低1us
		i ++;
		DSPORT = dat & 0x01;  	//然后写入一个数据，从最低位开始
		
		i = 6;
		while(i --); 				//延时68us，持续时间最少60us
		
		DSPORT = 1;					//然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		dat >>= 1;
	}
}

/*******************************************************************************
* 函 数 名         : Ds18b20ReadByte
* 输    入         : 空
* 输    出         :	返回的数据
* 函数功能		    : 读取一个字节
* 详细描述			 : 读出的一个字节的数据
*******************************************************************************/
uchar Ds18b20ReadByte(void)
{
	uchar byte, bi;
	uint i, j;	
	
	for(j = 8;j > 0;j --)
	{
		DSPORT = 0;					//先将总线拉低1us
		i ++;
		DSPORT = 1;					//然后释放总线
		i ++;
		i ++;							//延时6us等待数据稳定
		bi = DSPORT;	 			//读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte = (byte >> 1) | (bi << 7);						  
		i = 4;						//读取完之后等待48us再接着读取下一个数
		while(i --);
	}			
	
	return byte;
}

/*******************************************************************************
* 函 数 名         : Ds18b20ChangTemp
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 让18b20开始转换温度
* 详细描述			 : 让18b20开始转换温度
*******************************************************************************/
void Ds18b20ChangTemp(void)
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//跳过ROM操作命令		 
	Ds18b20WriteByte(0x44);	    //温度转换命令
}

/*******************************************************************************
* 函 数 名         : Ds18b20ReadTempCom
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 读取温度的命令
* 详细描述			 : 发送读取温度命令
*******************************************************************************/
void Ds18b20ReadTempCom(void)
{	
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xCC);	 //跳过ROM操作命令
	Ds18b20WriteByte(0xBE);	 //发送读取温度命令
}

/*******************************************************************************
* 函 数 名         : Ds18b20ReadTemp
* 输    入         : 空
* 输    出         :	一个int的数据  为温度的原始数值
* 函数功能		    : 读取温度
* 详细描述			 : 一个int的数据  为温度的原始数值，需要经过装换
*******************************************************************************/
int Ds18b20ReadTemp(void)
{
	int temp = 0;
	uchar tmh, tml;
	
	Ds18b20ChangTemp();			 	//先写入转换命令
	Ds18b20ReadTempCom();			//然后等待转换完后发送读取温度命令
	tml = Ds18b20ReadByte();		//读取温度值共16位，先读低字节
	tmh = Ds18b20ReadByte();		//再读高字节
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	
	return temp;
}

/*******************************************************************************
* 函 数 名         : Test_Temp_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 显示温度值
* 详细描述			 : 在串口上显示温度的值
*******************************************************************************/
void Test_Temp_UART(void)
{
	char Display_Buffer[8];
	float tp;
	int temp;

	while(1)
	{
		temp = Ds18b20ReadTemp();
		
		if(temp < 0)							//当温度值为负数
		{
			Display_Buffer[0] = '-'; 	  	//   -
													//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
			temp = temp - 1;
			temp = ~temp;
			tp = temp;
			temp = tp * 0.0625 * 100 + 0.5;//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
													//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
													//算加上0.5，还是在小数点后面。
		}
		else
		{			
			Display_Buffer[0] ='+';
			tp = temp;							//因为数据处理有小数点所以将温度赋给一个浮点型变量
													//如果温度是正的那么，那么正数的原码就是补码它本身
			
			temp = tp * 0.0625 * 100 + 0.5;//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
													//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
													//算加上0.5，还是在小数点后面。
		}

		Display_Buffer[1] = (temp / 10000) + '0';
		Display_Buffer[2] = (temp % 10000 / 1000) + '0';
		Display_Buffer[3] = (temp % 1000 / 100) + '0';
		Display_Buffer[4] = (temp % 100 / 10) + '0';
		Display_Buffer[5] = '.';
		Display_Buffer[6] = (temp % 100 / 10) + '0';
		Display_Buffer[7] = (temp % 10) + '0';
		
		#if Uart
		UART1_Send_Data(Display_Buffer,8);
		UART1_Send_Data("\r\n",2);
		#endif
		
		Delay_Xms(300);
	}
}
