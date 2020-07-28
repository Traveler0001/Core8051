#include "core51_uart.h"

#if STC89C52RC
/*******************************************************************************
* 函 数 名         : Usart_Init
* 输    入         : 结构体变量地址
* 输    出         :	空
* 函数功能		    : 串口配置函数
* 详细描述			 :	由于计算得出的值不是整数而进行了强制转换，所以会有误差，统计
							以下常见波特率供参考。
							11.0596Mhz 300  6000  1200 1800 2400 3600 4800 7200  9600  
										  14400 19200 28800 57600	。
							12Mhz   300 600 1200 2400 4800 （均有误差 0.16%）。
							24Mhz  1200  2400  4800 9600 （均有误差 0.16%）。
*******************************************************************************/
#if Uart 
void Usart_Init(_Parameter_Set *Par_Set)
{
	TMOD  |= Par_Set->TMOD_Set;																							//设置波特率发生器的定时器T1的模式
	SCON  |= Par_Set->Usart_Parameter.SCON_Set;																		//设置波特率的寄存器
	PCON  |= Par_Set->Usart_Parameter.PCON_Set;																		//设置PCON电源管理寄存器，该寄存器有一位管理着串口波特率是否倍速
	
	if(Par_Set->Usart_Parameter.PCON_Set!=0x80)																		//判断是否倍速，并计算串口波特率
	{
		TH1 = 256 - (u8)(XTAL / (Par_Set->Usart_Parameter.Baud_Rate * 0.384));								//不倍速  波特率为 XTAL/(波特率*32*12)  XTAL单位应为Hz  这里计算时各约去了1000
		TL1 = 256 - (u8)(XTAL / (Par_Set->Usart_Parameter.Baud_Rate * 0.384));					
	}else
	{
		TH1 = 256 - (u8)((2 * XTAL) / (Par_Set->Usart_Parameter.Baud_Rate * 0.384));						//倍速   波特率为 （2*XTAL）/(波特率*32*12)  XTAL单位应为Hz  这里计算时各约去了1000
		TL1 = 256 - (u8)((2 * XTAL) / (Par_Set->Usart_Parameter.Baud_Rate * 0.384));
	}
	
	ES		 = Par_Set->Usart_Parameter.ES_Set;																			//串口中断允许配置
	TR1	 = Par_Set->Timer1_Parameter.TR1_Set;																		//定时器T1开启位设置			
	EA     = Par_Set->EA_Set;																								//中断总开关开启位
}

/*******************************************************************************
* 函 数 名         : UART1_Wait_Receive
* 输    入         : Wait_Time：等待时间
* 输    出         :	0：接收到一个字节
*							1：接收超时
*
* 函数功能		    : 等待接收一个字节
* 详细描述			 :	空
*******************************************************************************/
unsigned char UART1_Wait_Receive(unsigned int Wait_Time)
{
	while((Wait_Time > 0) && (RI == 0))
	{
		Wait_Time --;
		Delay_X10us(1);
	}
	
	RI = 0;
	
	if(Wait_Time == 0)
		return 1;
	else
		return 0;
}

/*******************************************************************************
* 函 数 名         : UART1_Wait_Send
* 输    入         : Wait_Time：等待时间
* 输    出         :	0：发送一个字节成功
*							1：发送超时
*
* 函数功能		    : 等待发送一个字节完成
* 详细描述			 :	空
*******************************************************************************/
unsigned char UART1_Wait_Send(unsigned int Wait_Time)
{
	while((Wait_Time > 0) && (TI == 0))
	{
		Wait_Time --;
		Delay_X10us(1);
	}
	
	TI = 0;
	
	if(Wait_Time == 0)
		return 1;
	else
		return 0;
}

/*******************************************************************************
* 函 数 名         : UART1_Send_Data
* 输    入         : Data：需要发送的数据
*							Len：需要发送的长度
*
* 输    出         :	0：发送成功
*							1：发送超时
*
* 函数功能		    : 发送一串数据
* 详细描述			 :	空
*******************************************************************************/
unsigned char UART1_Send_Data(unsigned char *Data,unsigned int Len)
{
	unsigned int i;

	for(i = 0;i < Len;i ++)
	{
		SBUF = Data[i];
		
		if(UART1_Wait_Send(500))
			return 0;
	}
	
	return 1;
}

/*******************************************************************************
* 函 数 名         : UART1_Send_Data
* 输    入         : Data：需要发送的数据
*							Len：需要发送的长度
*
* 输    出         :	0：接收失败
*							其他：接收数据的长度
*
* 函数功能		    : 接收一串数据
* 详细描述			 :	此函数一般放在串口中断服务函数中
*******************************************************************************/
unsigned int UART1_Receive_Data(unsigned char *Data,unsigned int Max_Len)		
{
	unsigned int i;

	RI = 0;										

	for(i = 0;i < Max_Len;i ++)		
	{
		Data[i] = SBUF;						

		if(UART1_Wait_Receive(500) != 0)			
			return i + 1;					
	}
	
	return 0;								
}

/*******************************************************************************
* 函 数 名         : UART1_Test
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 串口1测试程序
* 详细描述			 :	此函数一般放在串口中断服务函数中，串口助手发送一字节数据，单片机返回一样的数据
*******************************************************************************/
void Test_UART1(void)
{
	u8 receiveData;
	
	receiveData = SBUF;			//出去接收到的数据
	RI = 0;							//清除接收中断标志位
	SBUF = receiveData;			//将接收到的数据放入到发送寄存器
	while(!TI);			 			//等待发送数据完成
	TI = 0;							//清除发送完成标志位
}

#endif

#endif
