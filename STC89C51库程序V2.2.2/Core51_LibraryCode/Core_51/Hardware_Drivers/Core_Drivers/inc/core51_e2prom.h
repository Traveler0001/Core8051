#ifndef __CORE51_E2PROM_H
#define __CORE51_E2PROM_H

#include "include_drives.h"
#include "intrins.h"
#include "core51_uart.h"

void IAP_Disable(void);														//�ر�IAP����
unsigned char E2PROM_ReadByte(unsigned int Addr);					//��E2PROM�ж�ȡ����
void E2PROM_WriteByte(unsigned int Addr, unsigned char Data);	//дһ�����ݵ�E2PROM��
void Erase_Sector(unsigned int Addr);									//��������
void Test_E2PROM(void);														//E2PROM���Գ���Ա

#endif
