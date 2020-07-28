/***************************************************************************************
*		              STC89C51工程模板											  																
描述	：通过配置Config.h中的宏定义，即可轻松实现51资源的配置																	
作者	：本架构由老顽童团队共同制定维护																																
声明	：该程序在老玩童Demo板测试通过，可以自由扩展编写，后续会逐步完善51库，整理并
		  修订全网模块库，文件中出现的名字均为QQ昵称。																						
淘宝店：老玩童电子
QQ群	：667039215   QQ号：1354696096
CSDN	：https://blog.csdn.net/qq_34390075
***************************************************************************************/

/*************温馨提示：若注释没有对齐，请使用工程目录下提供的KEIL主题文件*************/
/***************************************************************************************
*							库编程使用步骤
	  1：打开ic_config.h文件进行芯片裁剪
	  2：打开hardware_config.h进行内核资源裁剪
	  3：打开想要使用的模块库头文件，修改I/O接口（如果使用了SPI或IIC   因为是模拟
		  的总线所以要打开相应的头文件修改总线的I/O接口）
	  4：为了防止main.c文件内容因程序过大导致臃肿，破坏库结构。建议分文件编写，新建
		  XXX.c  XXX.h两个文件，存放到Func_Lib下，该文件夹用于存放功能实现，详细可参考
		  该文件夹下的Test_LED测试程序。
	  5：参考模块库中对应的Test_XXXX函数进行编程，也可直接调用测试例程
	  6：在#include "include_main.h"	中添加想要调用的函数声明的头文件
	  7：在main中调用，如下图Test_Led();函数
***************************************************************************************/

#include "include_main.h"			 //该头文件中包含了本该main包含的所有头文件

/*******************************************************************************
* 函 数 名         : main
* 输    入         : 无
* 输    出         :	无
* 函数功能		    : 主函数
* 详细描述			 :	无
*******************************************************************************/
void main()
{
	/*删除*/
	while(1);	
}
