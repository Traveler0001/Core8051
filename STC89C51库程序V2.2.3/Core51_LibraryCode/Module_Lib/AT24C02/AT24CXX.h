#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "sim_i2c.h"
#include "core51_uart.h"

//-----------EEPROM的地址定义-----------//
#define AT24C02_ADD 0xA0

void At24c02Write(unsigned char addr,unsigned char dat);		//往24c02的一个地址写入一个数据
unsigned char At24c02Read(unsigned char addr);					//读取24c02的一个地址的一个数据
void Test_AT24C02_UART(void);											//AT24C02测试程序

#endif
