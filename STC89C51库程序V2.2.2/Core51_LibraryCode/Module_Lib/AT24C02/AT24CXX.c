#include "at24cxx.h"

/*******************************************************************************
* 函 数 名         : At24c02Write
* 输    入         : addr 地址  dat  数据
* 输    出         :	空
* 函数功能		    : 往24c02的一个地址写入一个数据
* 详细描述			 :	寻址范围为00-0xFF  2K Bit 32页 每页 8 Byte  共256Byte
*******************************************************************************/
void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2c_Start();
	I2c_SendByte(AT24C02_ADD);					//发送写器件地址
	I2c_SendByte(addr);							//发送要写入内存地址
	I2c_SendByte(dat);							//发送数据
	I2c_Stop();
}

/*******************************************************************************
* 函 数 名         : At24c02Read
* 输    入         : 需要读取的地址 数值为 0x00-0xFF
* 输    出         :	读出的数据
* 函数功能		    : 读取24c02的一个地址的一个数据
* 详细描述			 :	发送器件地址加1 为读取模式
*******************************************************************************/
unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	
	I2c_Start();
	I2c_SendByte(AT24C02_ADD); 				//发送写器件地址
	I2c_SendByte(addr); 							//发送要读取的地址
	I2c_Start();
	I2c_SendByte(AT24C02_ADD + 1); 			//发送读器件地址
	num=I2c_ReadByte(); 							//读取数据
	I2c_Stop();
	
	return num;	
}

/*******************************************************************************
* 函 数 名         : Test_AT24C02_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : AT24C02测试程序
* 详细描述			 :	按下复位键或者断电重启单片机打印在串口助手上的计数值会自加1
*******************************************************************************/
void Test_AT24C02_UART(void)
{
	unsigned char Count = 0;
	
	Count = At24c02Read(0);				//读取保存好的值
	
	#if Uart
	UART1_Send_Data(&Count,1);
	#endif
	
	Count ++;								//修改计数值
	At24c02Write(0,Count);				//保存计数值
	
	while(1);
}
