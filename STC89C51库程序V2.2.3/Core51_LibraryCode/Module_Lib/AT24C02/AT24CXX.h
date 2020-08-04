#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "sim_i2c.h"
#include "core51_uart.h"

//-----------EEPROM�ĵ�ַ����-----------//
#define AT24C02_ADD 0xA0

void At24c02Write(unsigned char addr,unsigned char dat);		//��24c02��һ����ַд��һ������
unsigned char At24c02Read(unsigned char addr);					//��ȡ24c02��һ����ַ��һ������
void Test_AT24C02_UART(void);											//AT24C02���Գ���

#endif
