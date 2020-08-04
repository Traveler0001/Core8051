#ifndef __FINGERPRINT_H
#define __FINGERPRINT_H

#include "core51_it.h"
#include "delay.h"

/******************************
！！！！！！注意！！！！！！！！
1、必须将FP_Handler接收中断回调函数放在相应串口中断服务函数中
2、注意接收缓冲区大小设置
******************************/
#define NULL 0
sbit TOUCH = P3^7;
sbit FP_LED_Test = P0^0;

/*指纹模块型号*/
#define YX_F112	0
#define AS608 		1
#define FP_Model YX_F112

/*驱动类型*/
#define Simplification	0			//精简模式，部分功能函数无法使用，用于缩小代码体积和运行内存占用空间
#define Entirety			1			//全功能模式
#define Drive_Mode Simplification

/*接收缓冲区大小，精简模式下不小于14字节，全功能模式下不小于52字节*/
#define FP_RData_Size	14

/*判断缓冲区设置大小是否合理*/
#if ((Drive_Mode == Entirety) && (FP_RData_Size < 52))			//指纹模块结构体中的参数数组调整至40字节，所以相比精简版的缓冲区大38B
	#error Too few FP_RData_Size in entirety mode
#endif

#if ((Drive_Mode == Simplification) && (FP_RData_Size < 14))	//帧头不会存入接收缓冲区,且指纹模块结构体中的参数数组调整至4字节，16B-2B=14B
	#error Too few FP_RData_Size in simplification mode
#endif

/*指纹模块结构体*/
typedef struct
{
	u8  RData[FP_RData_Size];
	u8  RevFlag;
	u8  Addr[4];			//设备地址
	u8  Affirm;				//确认码
	
	#if (Drive_Mode == Simplification)
	u8  Param[4];			//参数
	#endif
	
	#if (Drive_Mode == Entirety)
	u8  Param[40];			//参数
	u16 System_State;		//系统状态寄存器
	u16 Discern_Number;	//识别码
	u16 Capacity;			//指纹库容量
	u16 Safety;				//安全等级
	u16 Data_Byte;			//数据包大小
	u16 Baud;				//波特率
	#endif
}FP_Struct;

static void FP_TOUCH_Init(void);			//初始化感应引脚
static u8 FP_Receive_Data(void);			//接收FP数据
static u8 FP_Verify(u8 *Data);			//数据包校验
static u8 Wait_FP_Ack(void);				//等待FP应答
void FP_Handler(void);						//串口中断回调函数
u8 FP_Init(FP_Struct *Fd);					//初始化
u8 FP_Send_Command(u8 Command, u8 *Param,u8 Len);	//发送指令
u8 Scan_Fingerprint(u16 StartPage, u16 EndPage);	//扫描指纹
u8 Add_Fingerprint(u8 Step,u16 Addr);	//添加指纹
u8 Del_Fingerprint(u16 Addr,u16 Num);	//删除指纹
u8 Cls_Del_Fingerprint(void);				//清空指纹库
void FP_Test(void);							//指纹模块测试函数

/*全功能模式下*/
#if (Drive_Mode == Entirety)
u8 Read_SysPara(void);						//读取系统参数
u8 Read_Valid_Templete_Num(void);		//读取有效模板个数
u8 Set_FP_Addr(u8 *Addr);					//设置模块地址
u8 Write_Reg(u8 Reg_Number,u8 Data);	//写寄存器
u8 Read_Index_Table(u8 Page_Number);	//读取已录入模板的索引表
						  
#endif

#endif
