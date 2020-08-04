#include "Buttons.h"

/*******************************************************************************
* 函 数 名         : Matrix_Buttons
* 输    入         : u8 Sustain  0 不支持一直按下  1 支持一直按下
* 输    出         :	u8  KeyValue   按键值
* 函数功能		    : 矩阵按键检测
* 详细描述			 : 检测按键是否按下，按下会返回一次KeyValue值，之后KeyValue会
							复位到初始化的值。当按下时会返回一个KeyValue值，之后KeyValue会
							复位成0xFF。当无按键按下，KeyValue一直都是0xFF，用于判断按键是
							否处于空闲，因为0x00已经被作为按键0的返回值，因此用0xFF
							下面是4*4矩阵按键应该返回的值，因接线不同，可能会导致返回值
							不同，具体还需要根据实际情况判断。
							0 	1 	2 	3
							4 	5 	6 	7
							8 	9 	A	B
							C 	D 	E 	F
*******************************************************************************/
u8 Matrix_Buttons(u8 Sustain)
{
	u8 KeyValue = 0xFF;			//用来存放读取到的键值,初始化时读出的值为0xFF；
	u8 Sustain_flag = 0;
	
	Sustain_flag = !Sustain;
	GPIO_KEY = 0x0f;	
	
	if(GPIO_KEY != 0x0f)			//读取按键是否按下
	{
		Delay_Xms(10);				//延时10ms进行消抖
		
		if(GPIO_KEY != 0x0F)		//再次检测键盘是否按下
		{	
			//测试列
			GPIO_KEY = 0X0F;
			switch(GPIO_KEY)
			{
				case(0x07):	KeyValue = 0;break;
				case(0x0B):	KeyValue = 1;break;
				case(0x0D): KeyValue = 2;break;
				case(0x0E):	KeyValue = 3;break;
			}
			
			//测试行
			GPIO_KEY = 0XF0;
			switch(GPIO_KEY)
			{
				case(0x70):	KeyValue = KeyValue;break;
				case(0xB0):	KeyValue += 4;break;
				case(0xD0): KeyValue += 8;break;
				case(0xE0):	KeyValue += 12;break;
			}
			while(Sustain_flag && (GPIO_KEY != 0xF0));	 //检测按键松手检测
		}
	}
	
	return KeyValue;
}

/*******************************************************************************
* 函 数 名         : Separate_Buttons
* 输    入         : u8 Sustain  0 不支持一直按下  1 支持一直按下
* 输    出         :	u8  KeyValue   按键值
* 函数功能		    : 独立按键检测
* 详细描述			 : 检测按键是否按下，按下会返回一次KeyValue值，之后KeyValue会
							复位到初始化的值。当按下时会返回一个KeyValue值，之后KeyValue会
							复位成0xFF。当无按键按下，KeyValue一直都是0xFF，用于判断按键是
							否处于空闲，因为0x00已经被作为按键0的返回值，因此用0xFF。
							下面是4位独立按键应该返回的值，因接线不同，可能会导致返回值
							不同，具体还需要根据实际情况判断。
							0 	1 	2 	3
*******************************************************************************/
u8 Separate_Buttons(u8 Sustain)
{
	u8 KeyValue = 0xFF;			//用来存放读取到的键值,初始化时读出的值为0xFF；
	u8 Sustain_flag = 0;
	
	Sustain_flag = !Sustain;
	
	
	if(KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)			//读取按键是否按下
	{
		Delay_Xms(10);				//延时10ms进行消抖
		
		if(KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)		//再次检测键盘是否按下
		{	
			if(KEY1 == 0) KeyValue = 0;
			if(KEY2 == 0) KeyValue = 1;
			if(KEY3 == 0) KeyValue = 2;
			if(KEY4 == 0) KeyValue = 3;
			while(Sustain_flag && (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1));	 //检测按键松手检测
		}
	}
	
	return KeyValue;
}

/*******************************************************************************
* 函 数 名         : Test_Matrix_Buttons_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 按键测试程序
* 详细描述			 :	按下按键串口打印对应按键编号
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
* 函 数 名         : Test_Separate_Buttons_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 按键测试程序
* 详细描述			 :	按下按键串口打印对应按键编号
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
