#ifndef __HARDWARE_CONFIG_H
#define __HARDWARE_CONFIG_H

#include "ic_config.h"

/**************************************************************************************
*		              宏配置											  *
描述：通过配置以下宏定义，即可轻松实现51内核资源的配置
注意事项：无																				  
***************************************************************************************/

#if 	STC89C52RC

#include "reg52.h"

/**************************内核配置***************************/
//晶振宏配置
#define XTAL_24MHz 			 0
#define XTAL_12MHz 			 0
#define XTAL_110592MHz 	 	 1

//外部中断0宏开关
#define Ex_Interrupt0		 0

//外部中断1宏开关
#define Ex_Interrupt1		 0

//定时器0宏开关
#define Timer0 				 0

//定时器1宏开关 
#define Timer1 			  	 0

//串口宏开关
#define Uart				 	 0

#if ((Timer1 + Uart) > 1)
	#error Timer1 And Uart Configuration conflicts
#endif

#endif

#endif