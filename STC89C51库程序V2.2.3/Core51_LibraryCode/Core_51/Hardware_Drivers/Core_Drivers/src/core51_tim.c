#include "core51_tim.h"

#if STC89C52RC

//该变量是定时器0和定时器1的装载值变量，由于51无自动装载，所以提供了如下两个全局变量
//作为装载值变量,该变量整个工程有效。
#if Timer0
	u16 TX0_Set_Xus;
	u8  TH0_Reload;
	u8  TL0_Reload;
#endif

#if Timer1
	u16 TX1_Set_Xus;
	u8  TH1_Reload;
	u8  TL1_Reload;
#endif

/******************************************************************************
* 函 数 名         : Timer0_Init
* 输    入         : 结构体变量地址
* 输    出         :	空
* 函数功能		    : 定时器1中断配置函数
* 详细描述			 :	该函数是初始化定时器0的配置函数
*******************************************************************************/
#if Timer0 
void Timer0_Init(_Parameter_Set *Par_Set)
{
	TMOD      |= Par_Set->TMOD_Set;								//设置定时器模式。
	TH0_Reload = (u16)(65536 - (u16)((XTAL / 12.0) * TX0_Set_Xus)) / 256;	//给定时器赋初值
	TL0_Reload = (u16)(65536 - (u16)((XTAL / 12.0) * TX0_Set_Xus)) % 256;
	TH0        = TH0_Reload;
	TL0        = TL0_Reload;
	ET0	     = Par_Set->Timer0_Parameter.ET0_Set;			//打开定时器T0中断允许
	TR0	     = Par_Set->Timer0_Parameter.TR0_Set;			//打开定时器
	EA		     = Par_Set->EA_Set;									//打开总中断
}

/*******************************************************************************
* 函 数 名         : Test_Timer0
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 定时器0中断测试程序
* 详细描述			 :	该函数是放在定时器0中断服务函数中使用
*******************************************************************************/
void Test_Timer0(void)
{
	static u8 i;
	
	i ++;
	if(i == 100)
	{
		i = 0;
		Test_Timer0_led = ~Test_Timer0_led;
	}
}
#endif

#if Timer1 
/*******************************************************************************
* 函 数 名         : Timer1_Init
* 输    入         : 结构体变量地址
* 输    出         :	空
* 函数功能		    : 定时器1中断配置函数
* 详细描述			 :	该函数是初始化定时器1的配置函数
*******************************************************************************/
void Timer1_Init(_Parameter_Set *Par_Set)
{
	TMOD 			|= Par_Set->TMOD_Set;													//设置定时器模式。
	TH1_Reload	 = (u16)(65536 - (u16)((XTAL / 12.0) * TX1_Set_Xus)) / 256;	//给定时器赋初值
	TL1_Reload   = (u16)(65536 - (u16)((XTAL / 12.0) * TX1_Set_Xus)) % 256;
	TH1			 = TH1_Reload;
	TL1			 = TL1_Reload;			
	ET1			 = Par_Set->Timer1_Parameter.ET1_Set;								//打开定时器T1中断允许
	TR1	       = Par_Set->Timer1_Parameter.TR1_Set;								//打开定时器
	EA		       = Par_Set->EA_Set;														//打开总中断
}

/*******************************************************************************
* 函 数 名         : Test_Timer1
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 定时器1中断测试程序
* 详细描述			 :	该函数是放在定时器1中断服务函数中使用
*******************************************************************************/
void Test_Timer1(void)
{
	static u8 i;
	
	i++;
	if(i == 100)
	{
		i = 0;
		Test_Timer1_led = ~Test_Timer1_led;
	}
}
#endif

#endif

