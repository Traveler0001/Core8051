#include "XPT2046.h"

/*******************************************************************************
* �� �� ��         : Read_AD_Data
* ��    ��         : cmd����ȡ��X����Y  0x94ΪX  0xA4ΪVBAT  0xD4ΪY  0xE4ΪAUXin
* ��    ��         :	endValue	�����źŴ���󷵻ص�ֵ
* ��������		    : ��ȡ��������
* ��ϸ����			 : ���͵�����Ϊ��ȡ���ĸ�ͨ����ֵ��һ���ĸ�ͨ����
*******************************************************************************/
unsigned int Read_AD_Data(uchar cmd)
{
	uchar i;
	uint AD_Value;

	CLK = 0;
	CS  = 0;								//��Ӧ��ֱ�Ӳ���SPI�ӿ�
	SPI_Write(cmd);
	
	for(i = 6; i> 0; i--); 			//��ʱ�ȴ�ת�����
	
	CLK = 1;	  							//����һ��ʱ�����ڣ����BUSY
	_nop_();
	_nop_();
	CLK = 0;
	_nop_();
	_nop_();
	AD_Value = SPI_Read();
	CS = 1;
	
	return AD_Value;	
}

/*******************************************************************************
* �� �� ��         : Test_XPT2046_UART
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : ���Դ���оƬ
* ��ϸ����			 :	��ȡ����оƬADC�ɼ���ֵ�ڴ�������ʾ
*******************************************************************************/
void Test_XPT2046_UART(void)
{
	char Display_Buffer[4];
	unsigned int temp;

	while(1)
	{
		temp = Read_AD_Data(0x94);

		Display_Buffer[0] = (temp / 1000) + '0';
		Display_Buffer[1] = (temp % 1000 / 100) + '0';
		Display_Buffer[2] = (temp % 1000 % 100 / 10) + '0';
		Display_Buffer[3] = (temp % 1000 % 100 % 10) + '0';
		
		#if Uart
		UART1_Send_Data(Display_Buffer,4);
		UART1_Send_Data("\r\n",2);
		#endif
		
		Delay_Xms(100);
	}
}
