#ifndef __SMG_H
#define __SMG_H

#include "reg52.h"
#include "delay.h"

//---�ض���ؼ���---//
#ifndef u8
#define u8 unsigned char
#endif
#ifndef u16 
#define u16 unsigned int
#endif
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint 
#define uint unsigned int
#endif

//��λ�����Ƕ���138��������������
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

//�������GPIO
#define Seg_codeGPIO P0

void Dig_Display(u8 i,u8 Segment);		//�����ɨ��
void Test_smg(void);							//��������ܺ;�����󰴼��ĳ���

#endif
