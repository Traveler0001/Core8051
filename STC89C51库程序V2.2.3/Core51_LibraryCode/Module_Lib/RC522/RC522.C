#include "RC522.H"

#define MAXRLEN 18 		//最大接收数据长度 

/*******************************************************************************
* 函 数 名			 : ReadRawRC
* 输    入			 : Address[IN]:寄存器地址
* 输    出			 : 读出的值
* 函数功能			 : 读RC522寄存器
* 详细描述			 : 无
*******************************************************************************/
static unsigned char ReadRawRC(unsigned char Address)
{
	unsigned char ucAddr;
	unsigned char ucResult = 0;

	CS = 0;
	ucAddr = ((Address << 1) & 0x7E) | 0x80;
	SPI_Write(ucAddr);
	ucResult = SPI_Read();
	CS = 1;

	return ucResult;
}

/*******************************************************************************
* 函 数 名			 : WriteRawRC
* 输    入			 : Address[IN]:寄存器地址
*					 		value[IN]:写入的值
*
* 输    出			 : 无
* 函数功能			 : 写RC632寄存器
* 详细描述			 : 无
*******************************************************************************/
static void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char ucAddr;

	CS = 0;
    ucAddr = ((Address << 1) & 0x7E);
    SPI_Write(ucAddr);
	SPI_Write(value);
	SPI_Start();
    CS = 1;
}

/*******************************************************************************
* 函 数 名			 : RC522_Init
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 无
* 详细描述			 : 无
*******************************************************************************/
void RC522_Init(void)
{
	PcdReset();
	PcdAntennaOff(); 
	PcdAntennaOn();  
}

/*******************************************************************************
* 函 数 名			 : PcdRequest
* 输    入			 : req_code[IN]:寻卡方式
*					 		0x52 = 寻感应区内所有符合14443A标准的卡
*					 		0x26 = 寻未进入休眠状态的卡
*
*					 		pTagType[OUT]：卡片类型代码
*					 		0x4400 = Mifare_UltraLight
*					 		0x0400 = Mifare_One(S50)
*					 		0x0200 = Mifare_One(S70)
*					 		0x0800 = Mifare_Pro(X)
*					 		0x4403 = Mifare_DESFire
*
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 寻卡
* 详细描述			 : 无
*******************************************************************************/                   
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	
	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
	
	ucComMF522Buf[0] = req_code;
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	
	if((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = MI_ERR;   
	}
	
	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdAnticoll
* 输    入			 : pSnr[OUT]:卡片序列号，4字节
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 防冲撞
* 详细描述			 : 无
*******************************************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
	char status;
	unsigned char i,snr_check=0;
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x00);
	ClearBitMask(CollReg,0x80);
	
	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;
	
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

	if(status == MI_OK)
	{
		for(i = 0;i < 4;i ++)
		{   
			*(pSnr+i)  = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}

		if(snr_check != ucComMF522Buf[i])
		{   
			status = MI_ERR;    
		}
	}
	
	SetBitMask(CollReg,0x80);

	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdSelect
* 输    入			 : pSnr[IN]:卡片序列号，4字节
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 选定卡片
* 详细描述			 : 无
*******************************************************************************/
char PcdSelect(unsigned char *pSnr)
{
	char status;
	unsigned char i;
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	
	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x70;
	ucComMF522Buf[6] = 0;

	for(i = 0;i < 4; i ++)
	{
		ucComMF522Buf[i+2] = *(pSnr+i);
		ucComMF522Buf[6]  ^= *(pSnr+i);
	}

	CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);	
	ClearBitMask(Status2Reg,0x08);	
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
	
	if((status == MI_OK) && (unLen == 0x18))
	{   
		status = MI_OK;  
	}
	else
	{   
		status = MI_ERR;   
	}
	
	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdAuthState
* 输    入			 : auth_mode[IN]: 密码验证模式
*					 		0x60 = 验证A密钥
*					 		0x61 = 验证B密钥
*
*					 		addr[IN]：块地址
*					 		pKey[IN]：密码
*					 		pSnr[IN]：卡片序列号，4字节
*
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 验证卡片密码
* 详细描述			 : 无
*******************************************************************************/         
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
	char status;
	unsigned int  unLen;
	unsigned char i,ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = auth_mode;
	ucComMF522Buf[1] = addr;
	for (i=0; i<6; i++)
	{    ucComMF522Buf[i+2] = *(pKey+i);   }
	for (i=0; i<6; i++)
	{    ucComMF522Buf[i+8] = *(pSnr+i);   }

	status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
	if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
	{   status = MI_ERR;   }

	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdRead
* 输    入			 : addr[IN]：块地址
*					 		pData[OUT]：读出的数据，16字节
*
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 读取M1卡一块数据
* 详细描述			 : 无
*******************************************************************************/
char PcdRead(unsigned char addr,unsigned char *pData)
{
	char status;
	unsigned int  unLen;
	unsigned char i,ucComMF522Buf[MAXRLEN]; 
	
	ucComMF522Buf[0] = PICC_READ;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x90))
	{
		for(i=0; i<16; i++)
			*(pData+i) = ucComMF522Buf[i];   
	}
	else
	{   
		status = MI_ERR;   
	}
	
	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdWrite
* 输    入			 : addr[IN]：块地址
*					 		pData[IN]：写入的数据，16字节
*
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 写数据到M1卡一块
* 详细描述			 : 无
*******************************************************************************/          
char PcdWrite(unsigned char addr,unsigned char *pData)
{
	char status;
	unsigned int  unLen;
	unsigned char i,ucComMF522Buf[MAXRLEN]; 
	
	ucComMF522Buf[0] = PICC_WRITE;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
	
	if((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
	{   
		status = MI_ERR;   
	}
	
	if(status == MI_OK)
	{
		for(i = 0;i < 16;i ++)   
			ucComMF522Buf[i] = *(pData+i); 
			  
		CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);
		status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);

		if((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
		{   
			status = MI_ERR;   
		}
	}

	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdHalt
* 输    入			 : 无
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 命令卡片进入休眠状态
* 详细描述			 : 无
*******************************************************************************/
char PcdHalt(void)
{
	char status;
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	
	ucComMF522Buf[0] = PICC_HALT;
	ucComMF522Buf[1] = 0;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
	
	return MI_OK;
}

/*******************************************************************************
* 函 数 名			 : CalulateCRC
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 用RC522计算CRC16函数
* 详细描述			 : 无
*******************************************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
	unsigned char i,n;

	ClearBitMask(DivIrqReg,0x04);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);

	for(i = 0;i < len;i ++)   
		WriteRawRC(FIFODataReg, *(pIndata+i));  
		 
	WriteRawRC(CommandReg, PCD_CALCCRC);

	i = 0xFF;
	do 
	{
		n = ReadRawRC(DivIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x04));

	pOutData[0] = ReadRawRC(CRCResultRegL);
	pOutData[1] = ReadRawRC(CRCResultRegM);
}

/*******************************************************************************
* 函 数 名			 : PcdReset
* 输    入			 : 无
* 输    出			 : 成功返回MI_OK
* 函数功能			 : 复位RC522
* 详细描述			 : 无
*******************************************************************************/
char PcdReset(void)
{
	RC522_RST = 1;
	_nop_();
	RC522_RST = 0;
	_nop_();
	RC522_RST = 1;
	_nop_();
	WriteRawRC(CommandReg,PCD_RESETPHASE);
	_nop_();
	
	WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);
	WriteRawRC(TxAutoReg,0x40);     

	return MI_OK;
}

/*******************************************************************************
* 函 数 名			 : M500PcdConfigISOType
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 设置RC522的工作方式 
* 详细描述			 : 无
*******************************************************************************/
char M500PcdConfigISOType(unsigned char type)
{
	if(type == 'A')                    
	{ 
		ClearBitMask(Status2Reg,0x08);
		WriteRawRC(ModeReg,0x3D);
		WriteRawRC(RxSelReg,0x86);
		WriteRawRC(RFCfgReg,0x7F);  
		WriteRawRC(TReloadRegL,30);
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);
		Delay_Xms(10);
		PcdAntennaOn();
	}
	else
		return -1;
	
	return MI_OK;
}

/*******************************************************************************
* 函 数 名			 : SetBitMask
* 输    入			 : reg[IN]:寄存器地址
*					 		mask[IN]:置位值
*
* 输    出			 : 无
* 函数功能			 : 置RC522寄存器位
* 详细描述			 : 无
*******************************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
	char tmp = 0x00;

	tmp = ReadRawRC(reg);
	WriteRawRC(reg,tmp | mask);  // set bit mask
}

/*******************************************************************************
* 函 数 名			 : ClearBitMask
* 输    入			 : reg[IN]:寄存器地址
*					 		mask[IN]:清位值
*
* 输    出			 : 无
* 函数功能			 : 清RC522寄存器位
* 详细描述			 : 无
*******************************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
	char tmp = 0x00;

	tmp = ReadRawRC(reg);
	WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/*******************************************************************************
* 函 数 名			 : PcdComMF522
* 输    入			 : Command[IN]:RC522命令字
*							pInData[IN]:通过RC522发送到卡片的数据
*					 		InLenByte[IN]:发送数据的字节长度
*					 		pOutData[OUT]:接收到的卡片返回数据
*					 		pOutLenBit[OUT]:返回数据的位长度
*
* 输    出			 : 输出状态值
* 函数功能			 : 通过RC522和ISO14443卡通讯
* 详细描述			 : 无
*******************************************************************************/
char PcdComMF522(unsigned char Command, 
unsigned char *pInData, 
unsigned char InLenByte,
unsigned char *pOutData, 
unsigned int  *pOutLenBit)
{
	char status = MI_ERR;
	unsigned char irqEn   = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;

	switch (Command)
	{
		case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
		break;

		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
		break;

		default:
		break;				  
	}

	WriteRawRC(ComIEnReg,irqEn | 0x80);
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);
	
	for(i = 0;i < InLenByte;i ++)
		WriteRawRC(FIFODataReg, pInData[i]);

	WriteRawRC(CommandReg, Command);

	if(Command == PCD_TRANSCEIVE)
		SetBitMask(BitFramingReg,0x80);  
	
	//    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
	i = 2000;
	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));

	ClearBitMask(BitFramingReg,0x80);
	
	if(i != 0)
	{    
		if(!(ReadRawRC(ErrorReg) & 0x1B))
		{
			status = MI_OK;
	
			if (n & irqEn & 0x01)
				status = MI_NOTAGERR;   
		
			if(Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
		
				if(lastBits)
					*pOutLenBit = (n-1)*8 + lastBits;   
				else
					*pOutLenBit = n*8;   
		
				if(n == 0)
					n = 1;    
		
				if(n > MAXRLEN)
					n = MAXRLEN;   
		
				for(i= 0;i < n;i ++)
					pOutData[i] = ReadRawRC(FIFODataReg);    
			}
		}
		else
		{   
			status = MI_ERR;   
		}
	}

	SetBitMask(ControlReg,0x80);           // stop timer now
	WriteRawRC(CommandReg,PCD_IDLE); 

	return status;
}

/*******************************************************************************
* 函 数 名			 : PcdAntennaOn
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 开启天线
* 详细描述			 : 每次启动或关闭天险发射之间应至少有1ms的间隔
*******************************************************************************/
void PcdAntennaOn(void)
{
	unsigned char i;

	i = ReadRawRC(TxControlReg);

	if (!(i & 0x03))
		SetBitMask(TxControlReg, 0x03);
}

/*******************************************************************************
* 函 数 名			 : PcdAntennaOff
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 关闭天线
* 详细描述			 : 无
*******************************************************************************/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

/*******************************************************************************
* 函 数 名			 : WaitCardOff
* 输    入			 : 无
* 输    出			 : 无
* 函数功能			 : 等待卡离开
* 详细描述			 : 无
*******************************************************************************/
void WaitCardOff(void)
{
	char status, TagType[2];
	
	while(1)
	{
		status = PcdRequest(REQ_ALL, TagType);

		if(status)
		{
			status = PcdRequest(REQ_ALL, TagType);
			if(status)
			{
				status = PcdRequest(REQ_ALL, TagType);

				if(status)
					return;
			}
		}

		Delay_Xms(10);
	}
}

/*******************************************************************************
* 函 数 名         : Test_RC522_UART
* 输    入         : 空
* 输    出         :	空
* 函数功能		    : 测试RC522
* 详细描述			 :	读取卡类型和卡序列号在串口上显示，前两个字节为卡类型，后4字节为卡序列号
*******************************************************************************/
void Test_RC522_UART(void)
{
	unsigned char Data[4];

	RC522_Init();
	
	while(1)
	{
		if(PcdRequest(REQ_ALL,Data) == MI_OK)
		{
			#if Uart
			UART1_Send_Data(Data,2);
			#endif	
			
			if(PcdAnticoll(Data) == MI_OK)
			{
				#if Uart
				UART1_Send_Data(Data,4);
				#endif	
			}
		}
		
		Delay_Xms(100);
	}
}
