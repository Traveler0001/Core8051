#ifndef __INCLUDE_MAIN_H
#define __INCLUDE_MAIN_H

//�ں˼���ͷ�ļ� �������û�˽���޸����
#include "include_drives.h"

#if STC89C52RC
#include "reg52.h"			 			//���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "core51_exti.h"				//���ļ��ж����˵�Ƭ�����ⲿ�ж����ú������������޸�
#include "core51_tim.h"					//���ļ��ж����˵�Ƭ���Ķ�ʱ�������ú������������޸�
#include "core51_uart.h"				//���ļ��ж����˵�Ƭ���Ĵ������ú������������޸�

//�û�����ͷ�ļ��������û�������ɾ�޸�
#include "core51_it.h"					//���ļ��ж����˵�Ƭ�����ж�ʵ�ֺ����������û����б�дʵ�ֹ���
#include "test_led.h"


#endif
#endif