#include "ds1302.h"

//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
static unsigned char code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
static unsigned char code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

/*******************************************************************************
* �� �� ��         : Ds1302Write
* ��    ��         : addr ��ַ������ dat
* ��    ��         :	��
* ��������		    : ��DS1302д����ַ+���ݣ�
* ��ϸ����			 :	��DS1302д���ݻ�������
*******************************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	
	RST = 0;
	_nop_();

	SCLK = 0;								//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1; 								//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for(n = 0;n < 8;n ++)				//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;				//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;							//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();
	}
	
	for(n = 0;n < 8;n ++)				//д��8λ����
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;							//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;									//�������ݽ���
	_nop_();
}

/*******************************************************************************
* �� �� ��         : Ds1302Read
* ��    ��         : addr ��ַ
* ��    ��         :	dat ���ص�����
* ��������		    : ��ȡһ����ַ������
* ��ϸ����			 :	��ȡһ����ַ������
*******************************************************************************/
uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	
	RST = 0;
	_nop_();

	SCLK = 0;							//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1;								//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for(n = 0;n < 8;n ++)			//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;			//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;						//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;						//DS1302�½���ʱ����������
		_nop_();
	}
	
	_nop_();
	
	for(n = 0;n < 8;n ++)			//��ȡ8λ����
	{
		dat1 = DSIO;					//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;						//DS1302�½���ʱ����������
		_nop_();
	}

	RST = 0;
	_nop_();								//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	
	return dat;	
}

/*******************************************************************************
* �� �� ��         : DEC_BCD
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ʮ����תBCD��
* ��ϸ����			 :	��
*******************************************************************************/
unsigned char DEC_BCD(unsigned char DEC)
{
	return ((DEC / 10) << 4) | (DEC % 10); 
}

/*******************************************************************************
* �� �� ��         : BCD_DEC
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : BCD��תʮ����
* ��ϸ����			 :	��
*******************************************************************************/
unsigned char BCD_DEC(unsigned char BCD)
{
	return ((BCD >> 4) * 10) + (BCD & 0x0F); 
}

/*******************************************************************************
* �� �� ��         : Ds1302Init
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ��ʼ��DS1302
* ��ϸ����			 :	��ʼ��DS1302
*******************************************************************************/
void Ds1302Init(DS1302_Time_Struct *Time)
{
	unsigned char n;
	unsigned char Time_Buffer[7];
	
	Time_Buffer[0] = Time->Sec;
	Time_Buffer[1] = Time->Min;
	Time_Buffer[2] = Time->Hou;
	Time_Buffer[3] = Time->Day;
	Time_Buffer[4] = Time->Mon;
	Time_Buffer[5] = Time->Week;
	Time_Buffer[6] = Time->Year - 2000;
	
	Ds1302Write(0x8E,0X00);		//��ֹд���������ǹر�д��������
	
	for (n = 0; n < 7; n ++)	//д��7���ֽڵ�ʱ���źţ����ʱ��������
		Ds1302Write(WRITE_RTC_ADDR[n],DEC_BCD(Time_Buffer[n]));	

	Ds1302Write(0x8E,0x80);		//��д��������
}

/*******************************************************************************
* �� �� ��         : Ds1302ReadTime
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ��ȡʱ����Ϣ
* ��ϸ����			 :	��ȡʱ����Ϣ��������TIME�������У����Ⱥ�˳��Ϊ���ʱ��������
*******************************************************************************/
void Ds1302ReadTime(DS1302_Time_Struct *Time)
{
	unsigned char n;
	unsigned char Time_Buffer[7];
	
	for (n = 0;n < 7;n ++ )		//��ȡ7���ֽڵ�ʱ���źţ����ʱ��������
		Time_Buffer[n] = BCD_DEC(Ds1302Read(READ_RTC_ADDR[n]));
	
	Time->Sec = Time_Buffer[0];
	Time->Min = Time_Buffer[1];
	Time->Hou = Time_Buffer[2];
	Time->Day = Time_Buffer[3];
	Time->Mon = Time_Buffer[4];
	Time->Week = Time_Buffer[5];
	Time->Year = Time_Buffer[6] + 2000;
}

/*******************************************************************************
* �� �� ��         : Test_ds1302_UART
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ����DS1302
* ��ϸ����			 :	��ȡʱ�䲢��ͨ�����ڴ�ӡ��ʾ
*******************************************************************************/
void Test_ds1302_UART(void)
{
	char DisplayData[18];
	DS1302_Time_Struct DS1302_Time;

	DS1302_Time.Year = 2020;
	DS1302_Time.Mon  = 1;
	DS1302_Time.Day  = 20;
	DS1302_Time.Week = 1;
	DS1302_Time.Hou  = 0;
	DS1302_Time.Min  = 32;
	DS1302_Time.Sec  = 0;
	Ds1302Init(&DS1302_Time);
	
	while(1)
	{
   	Ds1302ReadTime(&DS1302_Time);

		DisplayData[0] = DS1302_Time.Year / 1000;		//��
		DisplayData[1] = DS1302_Time.Year % 1000 / 100;
		DisplayData[2] = DS1302_Time.Year % 100 / 10;				
		DisplayData[3] = DS1302_Time.Year % 10;
		DisplayData[4] ='-';
		DisplayData[5] = DS1302_Time.Mon / 10;			//��
		DisplayData[6] = DS1302_Time.Mon % 10;	
		DisplayData[7] ='-';
		DisplayData[8] = DS1302_Time.Day / 10;			//��
		DisplayData[9] = DS1302_Time.Day % 10;	
		DisplayData[10] = ' ';
		DisplayData[1] = DS1302_Time.Hou / 10;			//ʱ
		DisplayData[12] = DS1302_Time.Hou % 10;				 
		DisplayData[13] ='-';
		DisplayData[14] = DS1302_Time.Min / 10;		//��
		DisplayData[15] = DS1302_Time.Min % 10;	
		DisplayData[16] ='-';
		DisplayData[17] = DS1302_Time.Sec / 10;		//��
		DisplayData[18] = DS1302_Time.Sec % 10;
		
		#if Uart
		UART1_Send_Data(DisplayData,18);
		#endif
		
		Delay_Xms(500);
	}
}
