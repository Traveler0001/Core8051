#include "core51_it.h"
#include "Fingerprint.H"

#if STC89C52RC

/*******************************************************************************
* 函 数 名       	: All_Parameter_Init(void)
* 函数功能		 	: 参数配置函数
* 输    入       	: 无
* 输    出    	 	: 无
*******************************************************************************/
void All_Parameter_Init(void)
{
	#if (Ex_Interrupt0 | Ex_Interrupt1 | Timer0 | Timer1 | Uart)
		_Parameter_Set Par;
	#endif
	
	//外部中断0配置，下降沿触发方式
	#if Ex_Interrupt0
		Par.Ex_Int0_Parameter.IT0_Set = 1;						//跳变沿触发方式 下降沿触发。
		Par.Ex_Int0_Parameter.EX0_Set = 1;						//打开INT0的中断允许。	
		Par.EA_Set = 1;												//打开总中断
		Ex_Interrupt0_Init(&Par);									//初始化参数
	#endif
	
	//外部中断1配置，下降沿触发方式
	#if Ex_Interrupt1
		Par.Ex_Int1_Parameter.IT1_Set = 1;						//跳变沿触发方式 下降沿触发
		Par.Ex_Int1_Parameter.EX1_Set = 1;						//打开INT0的中断允许。	
		Par.EA_Set = 1;												//打开总中断	
		Ex_Interrupt1_Init(&Par);									//初始化参数
	#endif
	
	//定时器0配置 16位定时器，时间为10000us，注意TX0_Set_Xus最大值  12Mhz最大为65535us 24Mhz下为32767us 
	#if Timer0
		Par.TMOD_Set = 0x01;											//设置定时器模式为方式1,16位定时器计数器模式
		TX0_Set_Xus = 10000;											//设置定时时长为10000us
		Par.Timer0_Parameter.ET0_Set = 1;						//打开定时器T0中断
		Par.Timer0_Parameter.TR0_Set = 1;						//开启定时器T0
		Par.EA_Set = 1;												//开启总中断
		Timer0_Init(&Par);											//初始化参数
	#endif
	
	//定时器1配置 16位定时器，时间为10000us，注意TX0_Set_Xus最大值  12Mhz最大为65535us 24Mhz下为32767us 
	#if Timer1
		Par.TMOD_Set = 0x10;											//设置定时器模式为方式1,16位定时器计数器模式
		TX1_Set_Xus = 10000;											//设置定时时长为10000us
		Par.Timer1_Parameter.ET1_Set = 1;						//打开定时器T1中断
		Par.Timer1_Parameter.TR1_Set = 1;						//开启定时器T1
		Par.EA_Set = 1;												//开启总中断
		Timer1_Init(&Par);											//初始化参数
	#endif
	
	//串口配置，此模式为倍速模式，11.0592MHz下最大为57600  赋值时应缩小1000倍。
	//由于计算得出的值不是整数而进行了强制转换，所以会有误差，统计以下常见波特率供参考
	//11.0596Mhz 300  6000  1200 1800 2400 3600 4800 7200  9600  14400 19200 28800 57600	
	//12Mhz   300 600 1200 2400 4800 （均有误差 0.16%）
	//24Mhz  1200  2400  4800 9600 （均有误差 0.16%）
	#if Uart
		Par.TMOD_Set = 0x20;											//设置定时器T1模式为方式2,8位自动装载定时器计数器模式
		Par.Usart_Parameter.SCON_Set = 0x50;					//设置8位数据，可变波特率，无奇偶校验位
		Par.Usart_Parameter.PCON_Set = 0x80;					//使能波特率倍速，最高波特率57600
		Par.Usart_Parameter.Baud_Rate = 57.600;				//设置波特率，57600/1000  此值只是缩小了1000倍便于后面计算
		Par.Usart_Parameter.ES_Set = 1;							//使能串口中断允许
		Par.Timer1_Parameter.TR1_Set = 1;						//开启定时器T1
		Par.EA_Set = 1;												//打开总中断
		Usart_Init(&Par);												//初始化参数
	#endif
}

/*******************************************************************************
* 函数名         : Ex_Int0()	interrupt 0
* 函数功能		  : 外部中断0服务函数
* 输入           : 无
* 输出           : 无
*******************************************************************************/
#if Ex_Interrupt0
void Ex_Int0(void) interrupt 0		
{
//	Test_ExIT0();
}
#endif

/*******************************************************************************
* 函数名         : Timer_Int0() interrupt 1
* 函数功能		  : 定时器0中断函数
* 输入           : 无
* 输出           : 无
*******************************************************************************/
#if Timer0
void Timer_Int0(void) interrupt 1
{
	TH0 = TH0_Reload;	//给定时器赋装载值
	TL0 = TL0_Reload;
//	Test_Timer0();
}
#endif

/*******************************************************************************
* 函数名         : void Ex_Int1()	interrupt 2	
* 函数功能		  : 外部中断1函数
* 输入           : 无
* 输出           : 无
*******************************************************************************/
#if Ex_Interrupt1
//外部中断1的中断实现
void Ex_Int1(void) interrupt 2		
{
//	Test_ExIT1();
}
#endif

/*******************************************************************************
* 函数名         : Timer_Int1() interrupt 3
* 函数功能		  : 定时器1中断函数
* 输入           : 无
* 输出           : 无
*******************************************************************************/
#if Timer1 
void Timer_Int1(void) interrupt 3
{
	TH1 = TH1_Reload;	//给定时器赋装载值
	TL1 = TL1_Reload;
//	Test_Timer1();
}
#endif

/*******************************************************************************
* 函数名         : Usart() interrupt 4
* 函数功能		  : 串口通信中断函数
* 输入           : 无
* 输出           : 无
*******************************************************************************/
#if Uart 
void Uart_Int(void) interrupt 4
{
//	Test_UART1();
}
#endif

#endif
