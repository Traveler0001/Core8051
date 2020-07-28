#include "core51_e2prom.h"

/*����IAP�Ĵ���*/
sfr IAP_DATA    =   0xE2;           //Flash �Ĵ���
sfr IAP_ADDRH   =   0xE3;           //Flash ��ַ�Ĵ������ֽ�
sfr IAP_ADDRL   =   0xE4;           //Flash ��ַ�Ĵ������ֽ�
sfr IAP_CMD     =   0xE5;           //Flash ����Ĵ���
sfr IAP_TRIG    =   0xE6;           //Flash �����Ĵ���
sfr IAP_CONTR   =   0xE7;           //Flash ���ƼĴ���

/*�궨��IAP��д�����*/
#define CMD_IDLE    0               //��������
#define CMD_READ    1               //��
#define CMD_WRITE   2               //д
#define CMD_ERASE   3               //��������

/*�궨��IAP�����ٶȳ���*/
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
* �� �� ��			:IAP_Disable
* ��    ��			:��
* ��    ��			:��
* ��������			:�ر�IAP
* ��ϸ����			:��
*******************************************************************************/
void IAP_Disable(void)
{
    IAP_CONTR = 0;                  //�ر�IAP����
    IAP_CMD   = 0;                  //�������
    IAP_TRIG  = 0;                  //��մ����Ĵ���
    IAP_ADDRH = 0x80;               //��յ�ַ�Ĵ������ֽڣ���ַָ�������ʼ��ַ��
    IAP_ADDRL = 0;                  //��յ�ַ�Ĵ������ֽ�
}

/*******************************************************************************
* �� �� ��			:E2PROM_ReadByte
* ��    ��			:Addr����ַ
* ��    ��			:�õ�ַ������
* ��������			:��E2PROM�ж�ȡһ���ֽ�
* ��ϸ����			:��
*******************************************************************************/
unsigned char E2PROM_ReadByte(unsigned int Addr)
{
    unsigned char Data;              

    IAP_CONTR = ENABLE_IAP;         //��IAP����
    IAP_CMD   = CMD_READ;           //��������
    IAP_ADDRL = Addr;               //���õ�ַ���ֽ�
    IAP_ADDRH = Addr >> 8;          //���õ�ַ���ֽ�
    IAP_TRIG  = 0x46;               //����0x46��������
    IAP_TRIG  = 0xB9;               //����0xB9��������
    _nop_();                        //��ʱһ��ʱ��
    Data = IAP_DATA;                //�Ӵ洢����һ���ֽ�
    IAP_Disable();                  //�ر�IAP����

    return Data;                    //���ض�ȡ����ֵ
}

/*******************************************************************************
* �� �� ��			:E2PROM_WriteByte
* ��    ��			:Addr����ַ
* ��    ��			:��
* ��������			:дһ�����ݵ�E2PROM��
* ��ϸ����			:��
*******************************************************************************/
void E2PROM_WriteByte(unsigned int Addr, unsigned char Data)
{
    IAP_CONTR = ENABLE_IAP;         //��IAP����
    IAP_CMD   = CMD_WRITE;          //��������
    IAP_ADDRL = Addr;               //���õ�ַ���ֽ�
    IAP_ADDRH = Addr >> 8;          //���õ�ַ���ֽ�
    IAP_DATA  = Data;               //д������
    IAP_TRIG  = 0x46;               //����0x46��������
    IAP_TRIG  = 0xB9;               //����0xB9��������
    _nop_();                        //��ʱһ��ʱ��
    IAP_Disable();					//�ر�IAP����
}

/*******************************************************************************
* �� �� ��			:Erase_Sector
* ��    ��			:Addr����ַ
* ��    ��			:��
* ��������			:��������
* ��ϸ����			:ע����д��Ҫ�����������׵�ַ
*******************************************************************************/
void Erase_Sector(unsigned int Addr)
{
    IAP_CONTR = ENABLE_IAP;         //��IAP����
    IAP_CMD   = CMD_ERASE;          //��������
    IAP_ADDRL = Addr;               //���õ�ַ���ֽ�
    IAP_ADDRH = Addr >> 8;          //���õ�ַ���ֽ�
    IAP_TRIG  = 0x46;               //����0x46��������
    IAP_TRIG  = 0xB9;               //����0xB9��������
    _nop_();                        //��ʱһ��ʱ��
    IAP_Disable();					//�ر�IAP����
}

/*******************************************************************************
* �� �� ��         : Test_EEPROM
* ��    ��         : ��
* ��    ��         :	��
* ��������		    : EEPROM���Գ���
* ��ϸ����			 :	���¸�λ�����߶ϵ�������Ƭ����ӡ�ڴ��������ϵļ���ֵ���Լ�1
*******************************************************************************/
void Test_EEPROM(void)
{
	unsigned char Count = 0;
	
	Count = E2PROM_ReadByte(0x2000);	//��ȡ����õ�ֵ
	
	#if Uart
	UART1_Send_Data(&Count,1);
	#endif
	
	Count ++;								//�޸ļ���ֵ
	Erase_Sector(0x2000);				//��������
	E2PROM_WriteByte(0x2000,Count);	//�������ֵ
	
	while(1);
}
