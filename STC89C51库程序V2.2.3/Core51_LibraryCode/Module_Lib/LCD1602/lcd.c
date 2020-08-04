#include "lcd.h"

/*******************************************************************************
* �� �� ��         : LcdWriteCom
* ��    ��         : com
* ��    ��         :	��
* ��������		    : ��LCDд��һ���ֽڵ�����
* ��ϸ����			 :	��LCDд��һ���ֽڵ�����
*******************************************************************************/
#ifndef LCD1602_4PINS	 					//��û�ж������LCD1602_4PINSʱ
void LcdWriteCom(uchar com)	  			//д������
{
	LCD1602_E = 0;    					 	//ʹ��
	LCD1602_RS = 0;	   					//ѡ��������
	LCD1602_RW = 0;	   					//ѡ��д��
	
	LCD1602_DATAPINS = com;     			//��������
	Lcd1602_Delay1ms(1);						//�ȴ������ȶ�

	LCD1602_E = 1;	          				//д��ʱ��
	Lcd1602_Delay1ms(5);	  					//����ʱ��
	LCD1602_E = 0;
}
#else 
void LcdWriteCom(uchar com)	  			//д������
{
	LCD1602_E = 0;	 							//ʹ������
	LCD1602_RS = 0;	 						//ѡ��д������
	LCD1602_RW = 0;	 						//ѡ��д��

	LCD1602_DATAPINS = com;					//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 							//д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = com << 4; 			//���͵���λ
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 							//д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif

/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��    ��         : dat
* ��    ��         :	��
* ��������		    : ��LCDд��һ���ֽڵ�����
* ��ϸ����			 :	��LCDд��һ���ֽڵ�����
*******************************************************************************/
#ifndef LCD1602_4PINS		   
void LcdWriteData(uchar dat)				//д������
{
	LCD1602_E = 0;								//ʹ������
	LCD1602_RS = 1;							//ѡ����������
	LCD1602_RW = 0;							//ѡ��д��

	LCD1602_DATAPINS = dat; 				//д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   							//д��ʱ��
	Lcd1602_Delay1ms(5);   					//����ʱ��
	LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)				//д������
{
	LCD1602_E = 0;	  							//ʹ������
	LCD1602_RS = 1;	  						//ѡ��д������
	LCD1602_RW = 0;	  						//ѡ��д��

	LCD1602_DATAPINS = dat;					//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  							//д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; 			//д�����λ
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  							//д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
  
/*******************************************************************************
* �� �� ��         : LcdInit
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ��ʼ��LCD��
* ��ϸ����			 :	��ʼ��LCD��
*******************************************************************************/
#ifndef LCD1602_4PINS
void LcdInit(void)						  	//LCD��ʼ���ӳ���
{
 	LcdWriteCom(0x38); 						//����ʾ
	LcdWriteCom(0x0c);  						//����ʾ����ʾ���
	LcdWriteCom(0x06);  						//дһ��ָ���1
	LcdWriteCom(0x01);  						//����
	LcdWriteCom(0x80);  						//��������ָ�����
}
#else
void LcdInit(void)						  	//LCD��ʼ���ӳ���
{
	LcdWriteCom(0x32);	 					//��8λ����תΪ4λ����
	LcdWriteCom(0x28);	 					//����λ���µĳ�ʼ��
	LcdWriteCom(0x0c);  						//����ʾ����ʾ���
	LcdWriteCom(0x06);  						//дһ��ָ���1
	LcdWriteCom(0x01);  						//����
	LcdWriteCom(0x80);  						//��������ָ�����
}
#endif
//========================================================================
// ����: void ClearLine(u8 row)
// ����: ���1��
// ����: row: ��(0��1)
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void ClearLine(u8 row)
{
    u8 i;
    LcdWriteCom(((row & 1) << 6) | 0x80);
    for(i=0; i<LineLength; i++) LcdWriteData(' ');
}
//========================================================================
// ����: void   WriteChar(u8 row, u8 column, u8 dat)
// ����: ָ���С��к��ַ�, дһ���ַ�
// ����: row: ��(0��1),  column: �ڼ����ַ�(0~15),  dat: Ҫд���ַ�.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void WriteChar(u8 row, u8 column, u8 dat)
{
    LcdWriteCom((((row & 1) << 6) + column) | 0x80);
    LcdWriteData(dat);
}
//========================================================================
// ����: void PutString(u8 row, u8 column, u8 *puts)
// ����: дһ���ַ�����ָ���С��к��ַ����׵�ַ
// ����: row: ��(0��1),  column: �ڼ����ַ�(0~15),  puts: Ҫд���ַ���ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void PutString(u8 row, u8 column, u8 *puts)
{
    LcdWriteCom((((row & 1) << 6) + column) | 0x80);
    for ( ;  *puts != 0;  puts++)       //����ֹͣ��0����
    {
        LcdWriteData(*puts);
        if(++column >= LineLength)  break;
    }
}
/*******************************************************************************
* �� �� ��         : Test_LCD1602
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ����LCD1602
* ��ϸ����			 : LCD1602����ʾ:LCD Test
*******************************************************************************/
void Test_LCD1602(void)
{
	char code Display_Data[] = "LCD Test";
	
	LcdInit();

	LcdWriteCom(0x80);			//������ʾλ��
	LcdWriteData(Display_Data[0]);
	LcdWriteCom(0x80+1);			//������ʾλ��
	LcdWriteData(Display_Data[1]);
	LcdWriteCom(0x80+2);			//������ʾλ��
	LcdWriteData(Display_Data[2]);
	LcdWriteCom(0x80+3);			//������ʾλ��
	LcdWriteData(Display_Data[3]);
	
	LcdWriteCom(0x80+4);			//������ʾλ��
	LcdWriteData(Display_Data[4]);
	LcdWriteCom(0x80+5);			//������ʾλ��
	LcdWriteData(Display_Data[5]);
	LcdWriteCom(0x80+6);			//������ʾλ��
	LcdWriteData(Display_Data[6]);	
	LcdWriteCom(0x80+7);			//������ʾλ��
	LcdWriteData(Display_Data[7]);

	while(1);
}