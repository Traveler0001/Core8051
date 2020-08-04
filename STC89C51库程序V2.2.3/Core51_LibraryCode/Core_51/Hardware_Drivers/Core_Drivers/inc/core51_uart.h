#ifndef __CORE51_UART_H
#define __CORE51_UART_H

#include "include_drives.h"
#include "intrins.h"

#if STC89C52RC
	#include "delay.h"

	//晶振的外部变量，无需干预。
	//extern float XTAL;

	#if Uart
		void Usart_Init(_Parameter_Set *Par_Set);												//串口初始化函数
		unsigned char UART1_Wait_Receive(unsigned int Wait_Time);						//等待接收一个字节
		unsigned char UART1_Wait_Send(unsigned int Wait_Time);							//等待发送一个字节完成
		unsigned char UART1_Send_Data(unsigned char *Data,unsigned int Len);			//发送一串数据
		unsigned int UART1_Receive_Data(unsigned char *Data,unsigned int Max_Len);	//接收一串数据
		void Test_UART1(void);																		//串口1测试程序
	#endif
#endif

#endif
