#include "Buttons.h"

/*******************************************************************************
* �� �� ��         : Matrix_Buttons
* ��    ��         : u8 Sustain  0 ��֧��һֱ����  1 ֧��һֱ����
* ��    ��         :	u8  KeyValue   ����ֵ
* ��������		    : ���󰴼����
* ��ϸ����			 : ��ⰴ���Ƿ��£����»᷵��һ��KeyValueֵ��֮��KeyValue��
							��λ����ʼ����ֵ��������ʱ�᷵��һ��KeyValueֵ��֮��KeyValue��
							��λ��0xFF�����ް������£�KeyValueһֱ����0xFF�������жϰ�����
							���ڿ��У���Ϊ0x00�Ѿ�����Ϊ����0�ķ���ֵ�������0xFF
							������4*4���󰴼�Ӧ�÷��ص�ֵ������߲�ͬ�����ܻᵼ�·���ֵ
							��ͬ�����廹��Ҫ����ʵ������жϡ�
							0 	1 	2 	3
							4 	5 	6 	7
							8 	9 	A	B
							C 	D 	E 	F
*******************************************************************************/
u8 Matrix_Buttons(u8 Sustain)
{
	u8 KeyValue = 0xFF;			//������Ŷ�ȡ���ļ�ֵ,��ʼ��ʱ������ֵΪ0xFF��
	u8 Sustain_flag = 0;
	
	Sustain_flag = !Sustain;
	GPIO_KEY = 0x0f;	
	
	if(GPIO_KEY != 0x0f)			//��ȡ�����Ƿ���
	{
		Delay_Xms(10);				//��ʱ10ms��������
		
		if(GPIO_KEY != 0x0F)		//�ٴμ������Ƿ���
		{	
			//������
			GPIO_KEY = 0X0F;
			switch(GPIO_KEY)
			{
				case(0x07):	KeyValue = 0;break;
				case(0x0B):	KeyValue = 1;break;
				case(0x0D): KeyValue = 2;break;
				case(0x0E):	KeyValue = 3;break;
			}
			
			//������
			GPIO_KEY = 0XF0;
			switch(GPIO_KEY)
			{
				case(0x70):	KeyValue = KeyValue;break;
				case(0xB0):	KeyValue += 4;break;
				case(0xD0): KeyValue += 8;break;
				case(0xE0):	KeyValue += 12;break;
			}
			while(Sustain_flag && (GPIO_KEY != 0xF0));	 //��ⰴ�����ּ��
		}
	}
	
	return KeyValue;
}

/*******************************************************************************
* �� �� ��         : Separate_Buttons
* ��    ��         : u8 Sustain  0 ��֧��һֱ����  1 ֧��һֱ����
* ��    ��         :	u8  KeyValue   ����ֵ
* ��������		    : �����������
* ��ϸ����			 : ��ⰴ���Ƿ��£����»᷵��һ��KeyValueֵ��֮��KeyValue��
							��λ����ʼ����ֵ��������ʱ�᷵��һ��KeyValueֵ��֮��KeyValue��
							��λ��0xFF�����ް������£�KeyValueһֱ����0xFF�������жϰ�����
							���ڿ��У���Ϊ0x00�Ѿ�����Ϊ����0�ķ���ֵ�������0xFF��
							������4λ��������Ӧ�÷��ص�ֵ������߲�ͬ�����ܻᵼ�·���ֵ
							��ͬ�����廹��Ҫ����ʵ������жϡ�
							0 	1 	2 	3
*******************************************************************************/
u8 Separate_Buttons(u8 Sustain)
{
	u8 KeyValue = 0xFF;			//������Ŷ�ȡ���ļ�ֵ,��ʼ��ʱ������ֵΪ0xFF��
	u8 Sustain_flag = 0;
	
	Sustain_flag = !Sustain;
	
	
	if(KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)			//��ȡ�����Ƿ���
	{
		Delay_Xms(10);				//��ʱ10ms��������
		
		if(KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)		//�ٴμ������Ƿ���
		{	
			if(KEY1 == 0) KeyValue = 0;
			if(KEY2 == 0) KeyValue = 1;
			if(KEY3 == 0) KeyValue = 2;
			if(KEY4 == 0) KeyValue = 3;
			while(Sustain_flag && (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1));	 //��ⰴ�����ּ��
		}
	}
	
	return KeyValue;
}

/*******************************************************************************
* �� �� ��         : Test_Matrix_Buttons_UART
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : �������Գ���
* ��ϸ����			 :	���°������ڴ�ӡ��Ӧ�������
*******************************************************************************/
void Test_Matrix_Buttons_UART(void)
{
	unsigned char Key_Val = 0;
	
	#if Uart
	unsigned char Buf[] = {"key = xx \r\n"};
	#endif
	
	while(1)
	{
		Key_Val = Matrix_Buttons(0);
		
		if(Key_Val != 0xff)
		{
			#if Uart
			sprintf(&Buf[6],"%2d",((int)Key_Val)%100);
			UART1_Send_Data(&Buf,11);
			#endif
		}
		
		Delay_Xms(10);
	}
}

/*******************************************************************************
* �� �� ��         : Test_Separate_Buttons_UART
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : �������Գ���
* ��ϸ����			 :	���°������ڴ�ӡ��Ӧ�������
*******************************************************************************/
void Test_Separate_Buttons_UART(void)
{
	unsigned char Key_Val = 0;
	
	#if Uart
	unsigned char Buf[] = {"key = xx \r\n"};
	#endif
	
	while(1)
	{
		Key_Val = Separate_Buttons(0);
		
		if(Key_Val != 0xff)
		{
			#if Uart
			sprintf(&Buf[6],"%2d",((int)Key_Val)%100);
			UART1_Send_Data(&Buf,11);
			#endif
		}
		
		Delay_Xms(10);
	}
}
