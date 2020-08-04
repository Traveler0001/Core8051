#ifndef __SIM_I2C_H
#define __SIM_I2C_H

//-----------ͷ�ļ�-----------//
#include "include_drives.h"

#if STC89C52RC
	#include <reg52.h>
	#include "delay.h"

	#define I2c_Delay  											//I2cʱ����ʱ�궨�壬STC89C52�ٶ�
																		//������ֱ�Ӻ궨��һ���յ���ʱ
	//-----------IIC�Ľӿ�����-----------//
	sbit SCL = P2^4;
	sbit SDA = P2^3;
	
	//-----------IIC��ʱ����-----------//
	void I2c_Start(void);										//��ʼ�ź�
	void I2c_Stop(void);											//ֹͣ�ź�
	unsigned char I2c_SendByte(unsigned char dat);		//����һ���ֽ�
	unsigned char I2c_ReadByte(void);						//��һ���ֽ�

#endif

#endif
