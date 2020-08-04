#ifndef __SIM_I2C_H
#define __SIM_I2C_H

//-----------头文件-----------//
#include "include_drives.h"

#if STC89C52RC
	#include <reg52.h>
	#include "delay.h"

	#define I2c_Delay  											//I2c时序延时宏定义，STC89C52速度
																		//慢所以直接宏定义一个空的延时
	//-----------IIC的接口配置-----------//
	sbit SCL = P2^4;
	sbit SDA = P2^3;
	
	//-----------IIC的时序函数-----------//
	void I2c_Start(void);										//起始信号
	void I2c_Stop(void);											//停止信号
	unsigned char I2c_SendByte(unsigned char dat);		//发送一个字节
	unsigned char I2c_ReadByte(void);						//读一个字节

#endif

#endif
