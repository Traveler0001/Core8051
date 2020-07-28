#include "XPT2046.h"

/*******************************************************************************
* 函 数 名         : Read_AD_Data
* 输    入         : cmd：读取的X或者Y  0x94为X  0xA4为VBAT  0xD4为Y  0xE4为AUXin
* 输    出         :	endValue	最终信号处理后返回的值
* 函数功能		    : 读取触摸数据
* 详细描述			 : 发送的命令为读取的哪个通道的值，一共四个通道。
*******************************************************************************/
unsigned int Read_AD_Data(uchar cmd)
{
	uchar i;
	uint AD_Value;

	CLK = 0;
	CS  = 0;								//不应该直接操作SPI接口
	SPI_Write(cmd);
	
	for(i = 6; i> 0; i--); 			//延时等待转换结果
	
	CLK = 1;	  							//发送一个时钟周期，清除BUSY
	_nop_();
	_nop_();
	CLK = 0;
	_nop_();
	_nop_();
	AD_Value = SPI_Read();
	CS = 1;
	
	return AD_Value;	
}

/*******************************************************************************
* 函 数 名         : Test_XPT2046_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 测试触摸芯片
* 详细描述			 :	读取触摸芯片ADC采集的值在串口上显示
*******************************************************************************/
void Test_XPT2046_UART(void)
{
	char Display_Buffer[4];
	unsigned int temp;

	while(1)
	{
		temp = Read_AD_Data(0x94);

		Display_Buffer[0] = (temp / 1000) + '0';
		Display_Buffer[1] = (temp % 1000 / 100) + '0';
		Display_Buffer[2] = (temp % 1000 % 100 / 10) + '0';
		Display_Buffer[3] = (temp % 1000 % 100 % 10) + '0';
		
		#if Uart
		UART1_Send_Data(Display_Buffer,4);
		UART1_Send_Data("\r\n",2);
		#endif
		
		Delay_Xms(100);
	}
}
