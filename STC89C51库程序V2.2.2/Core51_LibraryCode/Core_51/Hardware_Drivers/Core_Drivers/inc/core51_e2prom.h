#ifndef __CORE51_E2PROM_H
#define __CORE51_E2PROM_H

#include "include_drives.h"
#include "intrins.h"
#include "core51_uart.h"

void IAP_Disable(void);														//关闭IAP控制
unsigned char E2PROM_ReadByte(unsigned int Addr);					//从E2PROM中读取数据
void E2PROM_WriteByte(unsigned int Addr, unsigned char Data);	//写一个数据到E2PROM中
void Erase_Sector(unsigned int Addr);									//擦除扇区
void Test_E2PROM(void);														//E2PROM测试程序员

#endif
