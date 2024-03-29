#include "hc_sr04.h"

/*******************************************************************************
* 函 数 名			 : StartModule
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 启动Hc_sr04超声波模块
* 详细描述			 : 启动Hc_sr04超声波模块,模块的Trig(控制端)端口拉高10us以上�
							模块就会自动发出8个40khz的方波。
*******************************************************************************/
void  StartModule(void)					//启动模块
{
	TX = 1;									//启动一次模块
	Delay_X10us(2);						//10us以上高电平
	TX = 0;
}

/*******************************************************************************
* 函 数 名			 : Hc_sr04GetDistance
* 输    入			 : 无
* 输    出			 : 距离（单位cm）
* 函数功能			 : 测量一次距离
* 详细描述			 : 启动Hc_sr04超声波模块,模块的Trig(控制端)端口拉高10us以上，就
							可以在Echo(接收端)等待高电平变为低电平，并计算这段时间，进而
							计算出距离，使用此函数需要开启计数器0并设置最大计数时间。
*******************************************************************************/
float Hc_sr04GetDistance(void)
{
	uint Time;
	float Distance;
	
	TH0 = 0;
	TL0 = 0;
	StartModule();
	
	while(!RX);								//当RX为零时等待
	TR0 = 1;									//开启计数
	
	while(RX && TF0 == 0);				//当RX为1计数并等待
	TR0 = 0;									//关闭计数
	
	if(TF0 == 1)							//超出量程显示999
	{
		TF0 = 0;
		Distance = 999;
	}
	else
	{
		Time = TH0 * 256 + TL0;
		Distance = (Time * 17) / 1000;//算出来是CM
	}
	
	return Distance;
}

/*******************************************************************************
* 函 数 名			 : Test_Separate_Buttons_UART
* 输    入			 : 空
* 输    出			 :	空
* 函数功能			 : 超声波测试程序
* 详细描述			 :	串口显示超声波测量的距离，仅显示3位数，使用此函数需要开启计
							数器0和串口
*******************************************************************************/
void Test_Hc_sr04GetDistance_UART(void)
{
	unsigned int Distance;
	
	#if Uart
	unsigned char Buf[] = {"Distance = xxx \r\n"};
	#endif
	
	while(1)
	{
		Distance = Hc_sr04GetDistance();
		
		#if Uart
		sprintf(&Buf[11],"%3d",((int)Distance)%1000);
		UART1_Send_Data(&Buf,17);
		#endif
		
		Delay_Xms(100);
	}
}