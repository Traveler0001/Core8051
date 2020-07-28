#include "core51_e2prom.h"

/*定义IAP寄存器*/
sfr IAP_DATA    =   0xE2;           //Flash 寄存器
sfr IAP_ADDRH   =   0xE3;           //Flash 地址寄存器高字节
sfr IAP_ADDRL   =   0xE4;           //Flash 地址寄存器低字节
sfr IAP_CMD     =   0xE5;           //Flash 命令寄存器
sfr IAP_TRIG    =   0xE6;           //Flash 触发寄存器
sfr IAP_CONTR   =   0xE7;           //Flash 控制寄存器

/*宏定义IAP读写命令常量*/
#define CMD_IDLE    0               //常规命令
#define CMD_READ    1               //读
#define CMD_WRITE   2               //写
#define CMD_ERASE   3               //扇区擦错

/*宏定义IAP操作速度常量*/
#if (XTAL_24MHz == 1)
	#define ENABLE_IAP 0x80    
#endif

#if (XTAL_12MHz == 1)
	#define ENABLE_IAP 0x81        
#endif

#if (XTAL_110592MHz == 1)
	#define ENABLE_IAP 0x82     
#endif

/*******************************************************************************
* 函 数 名			:IAP_Disable
* 输    入			:无
* 输    出			:无
* 函数功能			:关闭IAP
* 详细描述			:无
*******************************************************************************/
void IAP_Disable(void)
{
    IAP_CONTR = 0;                  //关闭IAP控制
    IAP_CMD   = 0;                  //清除命令
    IAP_TRIG  = 0;                  //清空触发寄存器
    IAP_ADDRH = 0x80;               //清空地址寄存器高字节（地址指向程序起始地址）
    IAP_ADDRL = 0;                  //清空地址寄存器低字节
}

/*******************************************************************************
* 函 数 名			:E2PROM_ReadByte
* 输    入			:Addr：地址
* 输    出			:该地址的数据
* 函数功能			:从E2PROM中读取一个字节
* 详细描述			:无
*******************************************************************************/
unsigned char E2PROM_ReadByte(unsigned int Addr)
{
    unsigned char Data;              

    IAP_CONTR = ENABLE_IAP;         //打开IAP控制
    IAP_CMD   = CMD_READ;           //设置命令
    IAP_ADDRL = Addr;               //设置地址低字节
    IAP_ADDRH = Addr >> 8;          //设置地址高字节
    IAP_TRIG  = 0x46;               //发送0x46触发命令
    IAP_TRIG  = 0xB9;               //发送0xB9触发命令
    _nop_();                        //延时一个时钟
    Data = IAP_DATA;                //从存储区读一个字节
    IAP_Disable();                  //关闭IAP控制

    return Data;                    //返回读取到的值
}

/*******************************************************************************
* 函 数 名			:E2PROM_WriteByte
* 输    入			:Addr：地址
* 输    出			:无
* 函数功能			:写一个数据到E2PROM中
* 详细描述			:无
*******************************************************************************/
void E2PROM_WriteByte(unsigned int Addr, unsigned char Data)
{
    IAP_CONTR = ENABLE_IAP;         //打开IAP控制
    IAP_CMD   = CMD_WRITE;          //设置命令
    IAP_ADDRL = Addr;               //设置地址低字节
    IAP_ADDRH = Addr >> 8;          //设置地址高字节
    IAP_DATA  = Data;               //写入数据
    IAP_TRIG  = 0x46;               //发送0x46触发命令
    IAP_TRIG  = 0xB9;               //发送0xB9触发命令
    _nop_();                        //延时一个时钟
    IAP_Disable();					//关闭IAP控制
}

/*******************************************************************************
* 函 数 名			:Erase_Sector
* 输    入			:Addr：地址
* 输    出			:无
* 函数功能			:擦除扇区
* 详细描述			:注意填写需要擦除扇区的首地址
*******************************************************************************/
void Erase_Sector(unsigned int Addr)
{
    IAP_CONTR = ENABLE_IAP;         //打开IAP控制
    IAP_CMD   = CMD_ERASE;          //设置命令
    IAP_ADDRL = Addr;               //设置地址低字节
    IAP_ADDRH = Addr >> 8;          //设置地址高字节
    IAP_TRIG  = 0x46;               //发送0x46触发命令
    IAP_TRIG  = 0xB9;               //发送0xB9触发命令
    _nop_();                        //延时一个时钟
    IAP_Disable();					//关闭IAP控制
}

/*******************************************************************************
* 函 数 名         : Test_EEPROM
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : EEPROM测试程序
* 详细描述			 :	按下复位键或者断电重启单片机打印在串口助手上的计数值会自加1
*******************************************************************************/
void Test_EEPROM(void)
{
	unsigned char Count = 0;
	
	Count = E2PROM_ReadByte(0x2000);	//读取保存好的值
	
	#if Uart
	UART1_Send_Data(&Count,1);
	#endif
	
	Count ++;								//修改计数值
	Erase_Sector(0x2000);				//擦除扇区
	E2PROM_WriteByte(0x2000,Count);	//保存计数值
	
	while(1);
}
