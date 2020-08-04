#ifndef __HARDWARE_CONFIG_H
#define __HARDWARE_CONFIG_H

#include "ic_config.h"

/**************************************************************************************
*		              ������											  *
������ͨ���������º궨�壬��������ʵ��51�ں���Դ������
ע�������																				  
***************************************************************************************/

#if 	STC89C52RC

#include "reg52.h"

/**************************�ں�����***************************/
//���������
#define XTAL_24MHz 			 0
#define XTAL_12MHz 			 0
#define XTAL_110592MHz 	 	 1

//�ⲿ�ж�0�꿪��
#define Ex_Interrupt0		 0

//�ⲿ�ж�1�꿪��
#define Ex_Interrupt1		 0

//��ʱ��0�꿪��
#define Timer0 				 0

//��ʱ��1�꿪�� 
#define Timer1 			  	 0

//���ں꿪��
#define Uart				 	 0

#if ((Timer1 + Uart) > 1)
	#error Timer1 And Uart Configuration conflicts
#endif

#endif

#endif