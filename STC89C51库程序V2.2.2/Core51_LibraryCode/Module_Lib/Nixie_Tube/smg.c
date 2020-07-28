#include "smg.h"

//共阳极段码表 ，下表是数组下标对应的数码管显示值
/*		
		数组下标：0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
		对应段码：0 1 2 3 4 5 6 7 8 9 A  B	C	D	E	F	.	-	灭	
*/
u8 code smgduan[19] = {
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x80,0x40,0x00
};

/*******************************************************************************
* 函 数 名         : DigDisplay
* 输    入         : i：第几位显示  Segment：显示的值  支持0-F
* 输    出         :	空
* 函数功能		    : 数码管扫描
* 详细描述			 :	数码管动态扫描函数，检测8个数码管哪个显示。且电路是在138译码
							器，和573锁存器器的硬件电路下实现的。
*******************************************************************************/
void Dig_Display(u8 i,u8 Segment)
{
		switch(i)	 								//位选，选择点亮的数码管，
		{
			case(0):
				LSA = 0;LSB = 0;LSC = 0;break;//显示第0位
			case(1):
				LSA = 1;LSB = 0;LSC = 0;break;//显示第1位
			case(2):
				LSA = 0;LSB = 1;LSC = 0;break;//显示第2位
			case(3):
				LSA = 1;LSB = 1;LSC = 0;break;//显示第3位
			case(4):
				LSA = 0;LSB = 0;LSC = 1;break;//显示第4位
			case(5):
				LSA = 1;LSB = 0;LSC = 1;break;//显示第5位
			case(6):
				LSA = 0;LSB = 1;LSC = 1;break;//显示第6位
			case(7):
				LSA = 1;LSB = 1;LSC = 1;break;//显示第7位	
		}
		
		Seg_codeGPIO = smgduan[Segment];		//发送段码
		Delay_X10us(50); 							//间隔一段时间扫描	
		Seg_codeGPIO = 0x00;						//消隐
}

/*******************************************************************************
* 函 数 名         : Test_smg
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 测试数码管程序
* 详细描述			 : Count以1s为周期自加，数码管上会显示Count的值
*******************************************************************************/
void Test_smg(void)
{
	unsigned char Count = 0;
	
	while(1)
	{
		Count ++;
		Dig_Display(0,Count);
		Delay_Xs(1);
	}
}
