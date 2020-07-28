#ifndef __INCLUDE_MAIN_H
#define __INCLUDE_MAIN_H

//内核级别头文件 不允许用户私自修改添加
#include "include_drives.h"

#if STC89C52RC
#include "reg52.h"			 			//此文件中定义了单片机的一些特殊功能寄存器
#include "core51_exti.h"				//此文件中定义了单片机的外部中断配置函数，不允许修改
#include "core51_tim.h"					//此文件中定义了单片机的定时器的配置函数，不允许修改
#include "core51_uart.h"				//此文件中定义了单片机的串口配置函数，不允许修改

//用户级别头文件，可由用户自行增删修改
#include "core51_it.h"					//此文件中定义了单片机的中断实现函数，可由用户自行编写实现功能
#include "test_led.h"


#endif
#endif