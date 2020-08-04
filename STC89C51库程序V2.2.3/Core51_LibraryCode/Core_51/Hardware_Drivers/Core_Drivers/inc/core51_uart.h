#ifndef __CORE51_UART_H
#define __CORE51_UART_H

#include "include_drives.h"
#include "intrins.h"

#if STC89C52RC
	#include "delay.h"

	//������ⲿ�����������Ԥ��
	//extern float XTAL;

	#if Uart
		void Usart_Init(_Parameter_Set *Par_Set);												//���ڳ�ʼ������
		unsigned char UART1_Wait_Receive(unsigned int Wait_Time);						//�ȴ�����һ���ֽ�
		unsigned char UART1_Wait_Send(unsigned int Wait_Time);							//�ȴ�����һ���ֽ����
		unsigned char UART1_Send_Data(unsigned char *Data,unsigned int Len);			//����һ������
		unsigned int UART1_Receive_Data(unsigned char *Data,unsigned int Max_Len);	//����һ������
		void Test_UART1(void);																		//����1���Գ���
	#endif
#endif

#endif
