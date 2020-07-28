//本文件为规范示例头文件，无任何功能实现
//本文件主要提供头文件注释规范，宏定义及声明等规范
#ifndef __SPECIFICATION_H			//						<------头文件定义时结构为__XXXX_XX_XX_XX_H,中间有下划线加下划线，无下划线时不加
#define __SPECIFICATION_H



//-----------重定义关键词-----------//  <------不再改动的注释用这种注释  一般为宏定义注释
#ifndef 		u8
#define 		u8 			unsigned char			//<------宏定义时  一定要注意对齐事项
#endif
#ifndef 		u16 
#define 		u16 			unsigned int
#endif
#ifndef 		uchar
#define 		uchar 		unsigned char
#endif
#ifndef 		uint 
#define 		uint 			unsigned int
#endif



//设置矩阵按键使用的GPIO组							<------经常改动的注释用这种

//后续逐渐完善
#endif