#ifndef __HARDWARE_DRIVERS_H
#define __HARDWARE_DRIVERS_H

//对数据类型进行声明定义
typedef unsigned int u16;	  
typedef unsigned char u8;

//---重定义关键词---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint 
	#define uint unsigned int
#endif

#if 	STC89C52RC

typedef struct{
	//定时器模式配置变量
	u8 TMOD_Set;				//定时器模式变量
	//总中断配置变量
	u8	EA_Set;					//总中断允许变量
	
	//外部中断0内嵌结构体
#if Ex_Interrupt0
	struct{
		u8 IT0_Set:1;			//触发沿配置位
		u8 EX0_Set:1;			//外部中断0允许配置位
	}Ex_Int0_Parameter;
#endif	
	//外部中断1内嵌结构体
#if Ex_Interrupt1
	struct{
		u8 IT1_Set:1;			//触发沿配置为
		u8 EX1_Set:1;			//外部中断1允许配置为
	}Ex_Int1_Parameter;
#endif	
	//定时器或计数器中断0内嵌结构体
#if Timer0 
	struct{
		u8 ET0_Set:1;			//是否打开定时器0中断允许位
		u8 TR0_Set:1;			//是否打开定时器位
	}Timer0_Parameter;
#endif	
	//定时器或计数器中断0内嵌结构体
#if (Timer1 || Uart)
	struct{
		u8 ET1_Set:1;			//是否打开定时器0中断允许
		u8 TR1_Set:1;			//是否打开定时器位	
	}Timer1_Parameter;
#endif	
	//串口中断内嵌结构体
#if Uart 
	struct{
		u8 SCON_Set;			//设置为串口工作方式变量
		u8 PCON_Set;			//电源管理寄存器
		u8 ES_Set;				//串口接收中断变量
		float Baud_Rate;		//波特率参数配置变量
	}Usart_Parameter;
#endif
}_Parameter_Set;

#endif

#endif
