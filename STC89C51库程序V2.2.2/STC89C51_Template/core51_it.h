#ifndef	 __CORE51_IT_H_
#define   __CORE51_IT_H_

#include "include_drives.h"

//---�ض���ؼ���---//
#ifndef uchar
	#define uchar unsigned char
#endif

#ifndef uint
	#define uint  unsigned int
#endif

#ifndef ulong
	#define ulong  unsigned long
#endif

#if STC89C52RC
	//----����ͷ�ļ�----//
	#include <reg52.h>
	#include "core51_exti.h"
	#include "core51_tim.h"
	#include "core51_uart.h"

	#if Timer0
		extern u16 TX0_Set_Xus;
		extern u8 TH0_Reload;
		extern u8 TL0_Reload;
	#endif

	#if Timer1
		extern u16 TX1_Set_Xus;
		extern u8 TH1_Reload;
		extern u8 TL1_Reload;
	#endif

	//----�ں���Դ��ʼ������----//
	void All_Parameter_Init(void);
#endif

#endif
