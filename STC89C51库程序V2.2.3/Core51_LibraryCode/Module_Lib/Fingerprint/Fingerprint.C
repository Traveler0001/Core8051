#include "Fingerprint.H"

FP_Struct *FP;

/*******************************************************************************
* 函 数 名         : FP_Handler
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 指纹模块中断回调函数
* 详细描述			 :	此函数一般放在串口中断服务函数中，用于接收指纹模块数据
*******************************************************************************/
void FP_Handler(void)
{
	FP->RevFlag = FP_Receive_Data();
}

/*******************************************************************************
* 函 数 名         : FP_TOUCH_Init
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 接指纹模块触摸输出的引脚初始化
* 详细描述			 :	空
*******************************************************************************/
static void FP_TOUCH_Init(void)
{
	TOUCH = 1;
}

/*******************************************************************************
* 函 数 名         : FP_Receive_Data
* 输    入         : 空
* 输    出         :	0：接收失败
*							1：接收成功
*
* 函数功能		    : 接收指纹模块数据
* 详细描述			 :	空
*******************************************************************************/
static u8 FP_Receive_Data(void)
{
	u16 Count;
	
	RI = 0;					//清除接收中断
	
	if(SBUF == 0xEF)		//判断包头
	{
		#if Uart
		if(UART1_Wait_Receive(300))
			return 0;
		#endif

		if(SBUF == 0x01)	//判断包头
		{
			for(Count = 0;(Count < (FP->RData[6] + 7)) && (Count < FP_RData_Size);Count ++)	//接收数据
			{
				#if Uart
				if(UART1_Wait_Receive(300))
					return 0;
				#endif
				
				FP->RData[Count] = SBUF;
			}
		}
		else
			return 0;
	}
	else
		return 0;
	
	return 1;
}

/*******************************************************************************
* 函 数 名         : FP_Verify
* 输    入         : 空
* 输    出         :	0：校验成功
*							1：校验和错误
*
* 函数功能		    : 数据包校验
* 详细描述			 :	空
*******************************************************************************/
static u8 FP_Verify(u8 *Data)
{
	u16 Count;
	u16 Sum = 0;
	
	for(Count = 0;Count < (Data[6] + 1);Count ++)		//校验和
		Sum += Data[4 + Count];
	
	if((Data[FP->RData[6] + 7 - 2] != ((Sum >> 8) & 0xFF)) || 
		(Data[FP->RData[6] + 7 - 1] != (Sum & 0xFF)))
		return 1;	//校验和错误
	
	return 0;
}

/*******************************************************************************
* 函 数 名         : Wait_FP_Ack
* 输    入         : 空
* 输    出         :	0xFE：数据包校验错误
*							0xFF：应答超时
*							其他：应答正常
*
* 函数功能		    : 等待指纹模块应答
* 详细描述			 :	空
*******************************************************************************/
static u8 Wait_FP_Ack(void)
{
	u16 Count = 500;
	
	while((FP->RevFlag == 0) && (Count > 0))	//等待接收
	{
		Count --;
		Delay_Xms(1);
	}
	
	FP->RevFlag = 0;
	
	if(FP_Verify(FP->RData) != 0)		//数据包校验错误
		return 0xFE;				
	
	if(Count > 0)
	{
		FP->Affirm = FP->RData[7];		//保存确认码
		
		for(Count = 0;Count < (((FP->RData[5] << 8) | FP->RData[6]) - 3);Count ++)	//保存返回参数
			FP->Param[Count] = FP->RData[Count + 8];
		
		return FP->Affirm;
	}
	else
		return 0xFF;						//等待超时
}

/*******************************************************************************
* 函 数 名         : FP_Send_Command
* 输    入         : 空
* 输    出         :	0：发送成功
*							1：发送失败
*
* 函数功能		    : 发送指令
* 详细描述			 :	空
*******************************************************************************/
u8 FP_Send_Command(u8 Command,u8 *Param,u8 Len)
{
	u8 Count,Data[20]={0xEF,0x01};
	u16 Sum;
	
	Data[2] = FP->Addr[3];						//模块地址
	Data[3] = FP->Addr[2];
	Data[4] = FP->Addr[1];
	Data[5] = FP->Addr[0];
	Data[6] = 0x01;								//包标识
	Data[7] = ((Len + 3) >> 8) & 0xFF;		//包长度
	Data[8] = (Len + 3) & 0xFF;			
	Data[9] = Command;							//指令
	
	for(Count = 0;Count < Len;Count ++)		//参数
		Data[Count + 10] = *(Param + Count);
	
	for(Count = Sum = 0;Count < (Len + 4);Count ++)	//数据累加和
		Sum += Data[Count + 6];
	
	Data[Len + 10] = (Sum >> 8) & 0xFF;		//校验和
	Data[Len + 11] = Sum & 0xFF;
	
	#if Uart
	if(UART1_Send_Data(Data,Len + 12) == 1)
		return 1;
	#endif

	return 0;
}

/*******************************************************************************
* 函 数 名         : Set_FP_Addr
* 输    入         : StartPage：扫描起始页
*							EndPage：扫描结束页
*
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 扫描指纹
* 详细描述			 :	空
*******************************************************************************/
u8 Scan_Fingerprint(u16 StartPage,u16 EndPage)
{
	u8 Temp[5];
	
	FP_Send_Command(0x01,NULL,0);	//获取图像
	
	if(Wait_FP_Ack())					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	Temp[0] = 1;							//生成特征存放于CharBuffer1
	FP_Send_Command(0x02,Temp,1);	//生成特征
	
	if(Wait_FP_Ack())					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	Temp[0] = 1;
	Temp[1] = (StartPage >> 8) & 0xFF;
	Temp[2] = StartPage & 0xFF;
	Temp[3] = ((EndPage - StartPage) >> 8) & 0xFF;
	Temp[4] = (EndPage - StartPage) & 0xFF;
	FP_Send_Command(0x04,Temp,5);	//搜索指纹
	
	if(Wait_FP_Ack())					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	return 0;
}

/*******************************************************************************
* 函 数 名         : Add_Fingerprint
* 输    入         : Step：步骤
*							Addr：指纹存储地址
*
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 添加指纹
* 详细描述			 :	空
*******************************************************************************/
#if (FP_Model == YX_F112)
u8 Add_Fingerprint(u8 Step,u16 Addr)
{
	u8 Temp[10];
	
	switch(Step)
	{
		case 0:
			FP_Send_Command(0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
	
			Temp[0] = 1;						//生成特征存放于CharBuffer1
			FP_Send_Command(0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
			
		case 1:	
			FP_Send_Command(0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
			
			Temp[0] = 2;						//生成特征存放于CharBuffer2
			FP_Send_Command(0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
			
		case 2:	
			FP_Send_Command(0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
			
			Temp[0] = 3;						//生成特征存放于CharBuffer2
			FP_Send_Command(0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
			
		case 3:	
			FP_Send_Command(0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
			
			Temp[0] = 4;						//生成特征存放于CharBuffer2
			FP_Send_Command(0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
			
		case 4:	
			FP_Send_Command(0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
			
			Temp[0] = 5;						//生成特征存放于CharBuffer2
			FP_Send_Command(0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
			
		case 5:	
			FP_Send_Command(0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
			
			Temp[0] = 6;						//生成特征存放于CharBuffer2
			FP_Send_Command(0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
			
		case 6:
			FP_Send_Command(0x05,NULL,0);	//将CharBuffer1和CharBuffer2中的特征文件合并，生成模板
		
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
			
			Temp[0] = 2;
			Temp[1] = (Addr >> 8) & 0xFF;
			Temp[2] = Addr & 0xFF;
			FP_Send_Command(0x06,Temp,3);	//将CharBuffer2中的模板 存储于Flash数据库Addr中
			
			if(Wait_FP_Ack())					//等待应答
				return FP->Affirm;			//应答超时或指令执行失败返回确认码
		break;
	}
	
	return 0;
}
#endif

#if (FP_Model == AS608)
u8 Add_Fingerprint(u8 Step,u16 Addr)
{
	u8 Temp[10];
	
	switch(Step)
	{
		case 0:
			FP_Send_Command(FP,0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
			Temp[0] = 1;							//生成特征存放于CharBuffer1
			FP_Send_Command(FP,0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
		break;
			
		case 1:	
			FP_Send_Command(FP,0x01,NULL,0);	//获取图像
		
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
			
			Temp[0] = 2;							//生成特征存放于CharBuffer2
			FP_Send_Command(FP,0x02,Temp,1);	//生成特征
			
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
		break;
		
		case 2:	
			FP_Send_Command(FP,0x03,NULL,0);	//将CharBuffer1和CharBuffer2中的特征文件合并，生成模板
		
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
		break;
			
		case 3:
			FP_Send_Command(FP,0x05,NULL,0);	//将CharBuffer1和CharBuffer2中的特征文件合并，生成模板
		
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
			
			Temp[0] = 2;
			Temp[1] = (Addr >> 8) & 0xFF;
			Temp[2] = Addr & 0xFF;
			FP_Send_Command(FP,0x06,Temp,3);	//将CharBuffer2中的模板 存储于Flash数据库Addr中
			
			if(Wait_FP_Ack(FP))					//等待应答
				return FP->Affirm;				//应答超时或指令执行失败返回确认码
		break;
	}
	
	return 0;
}
#endif

/*******************************************************************************
* 函 数 名         : Del_Fingerprint
* 输    入         : Addr：指纹存储地址
*							Num：指纹地址开始的N个模板
*
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 删除指纹
* 详细描述			 :	空
*******************************************************************************/
u8 Del_Fingerprint(u16 Addr,u16 Num)
{
	u8 Temp[10];
	
	Temp[0] = (Addr >> 8) & 0xFF;
	Temp[1] = Addr & 0xFF;			
	Temp[2] = (Num >> 8) & 0xFF;
	Temp[3] = Num & 0xFF;
	FP_Send_Command(0x0C,Temp,4);	//生成特征
	
	if(Wait_FP_Ack())					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	return 0;
}

/*******************************************************************************
* 函 数 名         : Cls_Del_Fingerprint
* 输    入         : 空
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 清空指纹库
* 详细描述			 :	空
*******************************************************************************/
u8 Cls_Del_Fingerprint(void)
{
	FP_Send_Command(0x0D,NULL,0);	//生成特征
	
	if(Wait_FP_Ack())					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	return 0;
}

/*******************************************************************************
* 函 数 名         : Read_Valid_Templete_Num
* 输    入         : Fd：指纹结构体指针
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 指纹模块初始化
* 详细描述			 :	空
*******************************************************************************/
u8 FP_Init(FP_Struct *Fd)
{
	Delay_Xms(700);	//等待模块上电稳定
	FP_TOUCH_Init();
	
	FP = Fd;
	FP->Addr[0] = 0xFF;
	FP->Addr[1] = 0xFF;
	FP->Addr[2] = 0xFF;
	FP->Addr[3] = 0xFF;
	
#if (Drive_Mode == Entirety)
	return Read_SysPara(FP);
#endif
	
#if (Drive_Mode == Simplification)
	return 0;
#endif
}

#if (Drive_Mode == Entirety)
/*******************************************************************************
* 函 数 名         : Read_SysPara
* 输    入         : 空
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 写系统参数
* 详细描述			 :	空
*******************************************************************************/
u8 Read_SysPara(void)
{
	FP_Send_Command(FP,0x0F,NULL,0);		//读取系统参数
	
	if((FP->Affirm = Wait_FP_Ack(FP)))//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	FP->System_State = (u16)(FP->Param[0] << 8) | FP->Param[1];		//系统状态寄存器
	FP->Discern_Number = (u16)(FP->Param[2] << 8) | FP->Param[3];	//识别码
	FP->Capacity = (u16)(FP->Param[4] << 8) | FP->Param[5];			//指纹库容量
	FP->Safety = (u16)(FP->Param[6] << 8) | FP->Param[7];				//安全等级 
	FP->Addr[3] = FP->Param[8];												//设备地址
	FP->Addr[2] = FP->Param[9];
	FP->Addr[1] = FP->Param[10];
	FP->Addr[0] = FP->Param[11];
	FP->Data_Byte = (u16)(FP->Param[12] << 8) | FP->Param[13];		//数据包大小
	FP->Baud = (u16)(FP->Param[14] << 8) | FP->Param[15];				//波特率
	
	return 0;
}
/*******************************************************************************
* 函 数 名         : Write_Reg
* 输    入         : Reg_Number：寄存器号（4-6）
*							Data：数据
*
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 写系统参数
* 详细描述			 :	空
*******************************************************************************/
u8 Write_Reg(u8 Reg_Number,u8 Data)
{
	u8 Temp[10];
	
	Temp[0] = Reg_Number;
	Temp[1] = Data;
	FP_Send_Command(FP,0x0E,Temp,2);	//写寄存器
	
	if(Wait_FP_Ack(FP))					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码

	return 0;
}

/*******************************************************************************
* 函 数 名         : Read_Valid_Templete_Num
* 输    入         : 空
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 读取有效模板个数
* 详细描述			 :	空
*******************************************************************************/
u8 Read_Valid_Templete_Num(void)
{
	FP_Send_Command(FP,0x1D,NULL,0);	//读取有效模板个数
	
	if(Wait_FP_Ack(FP))					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码
	
	return 0;
}

/*******************************************************************************
* 函 数 名         : Read_Valid_Templete_Num
* 输    入         : Page_Number：索引表页码
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 读取已录入模板的索引表
* 详细描述			 :	空
*******************************************************************************/
u8 Read_Index_Table(u8 Page_Number)
{
	u8 Temp[10];
	
	Temp[0] = Page_Number;
	FP_Send_Command(FP,0x1F,Temp,1);	//读取已录入模板的索引表
	
	if(Wait_FP_Ack(FP))					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码

	return 0;
}

/*******************************************************************************
* 函 数 名         : Set_FP_Addr
* 输    入         : Addr：需设置的模块地址
* 输    出         :	0：成功
*							其他：失败
*
* 函数功能		    : 设置模块地址
* 详细描述			 :	空
*******************************************************************************/
u8 Set_FP_Addr(u8 *Addr)
{
	FP_Send_Command(FP,0x15,Addr,4);	//搜索指纹
	
	if(Wait_FP_Ack(FP))					//等待应答
		return FP->Affirm;				//应答超时或指令执行失败返回确认码

	return 0;
}
#endif

/*******************************************************************************
* 函 数 名         : FP_Test
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 指纹模块测试函数
* 详细描述			 :	空
*******************************************************************************/
void FP_Test(void)
{
	u8 err = 0,Step = 0,Step_Max = 4,Cnt;
	FP_Struct xdata FP;
	
#if (FP_Model == YX_F112)
	Step_Max = 7;
#endif
	
#if (FP_Model == AS608)
	Step_Max = 4;
#endif
	
	FP_Init(&FP);
	
	while(1)
	{
		/*指纹对比*/
//		err = Scan_Fingerprint(0,100);
//		if(err == 0)
//			FP_LED_Test = 0;
//		else
//			FP_LED_Test = 1;
//		
		/*录入指纹到地址0中*/
		err = Add_Fingerprint(Step,0);
		if(err == 0x00)
		{
			FP_LED_Test = 0;
			Step ++;
		}
		else
			FP_LED_Test = 1;

		if(Step == Step_Max)
		{
			for(Cnt = 0;Cnt < 10;Cnt ++)
			{
				FP_LED_Test = !FP_LED_Test;
				Delay_Xms(300);
			}
			
			FP_LED_Test = 1;
			Step = 0;
		}
		
		FP_LED_Test = TOUCH;
		Delay_Xms(1000);
	}
}
