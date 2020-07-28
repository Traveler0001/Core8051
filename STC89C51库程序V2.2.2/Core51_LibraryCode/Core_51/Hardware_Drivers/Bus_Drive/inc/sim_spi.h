#ifndef __SIM_SPI_H
#define __SIM_SPI_H

#include "include_drives.h"

#if STC89C52RC
	//---����ͷ�ļ�---//
	#include <reg52.h>
	#include <intrins.h>

	//---�ض���ؼ���---//
	#ifndef uchar
		#define uchar unsigned char
	#endif
		
	#ifndef uint
		#define uint  unsigned int
	#endif
		
	#ifndef ulong
		#define ulong unsigned long
	#endif

	//---����SPIʹ�õ�IO��---//
	sbit DOUT = P3^4;	  			//���
	sbit CLK  = P3^6;	  			//ʱ��
	sbit DIN  = P3^7;	  			//����
	sbit CS   = P3^5;	  			//Ƭѡ

	#define SPI_Delay 			//SPIʱ����ʱ�궨�壬STC89C52�ٶ�
										//������ֱ�Ӻ궨��һ���յ���ʱ

	//-----------SPI��ʵ�ֺ���-----------//
	void SPI_Start(void);		//SPI��ʼ��
	uint SPI_Read(void);			//SPI��
	void SPI_Write(uchar dat);	//SPIд
#endif
		
#endif
		