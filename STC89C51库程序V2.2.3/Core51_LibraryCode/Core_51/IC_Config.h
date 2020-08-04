#ifndef __IC_CONFIG_H
#define __IC_CONFIG_H

//STC系列单片机
#define			STC							1
#define 			STC89C52RC  				1
#define			STC12C5A60S2				0	

//STC系列单片机宏编译错误提示
#if STC
	#if ((STC89C52RC + STC12C5A60S2) > 1)
		#error Multiple chips are configured
	#elif	((STC89C52RC + STC12C5A60S2) == 0)
		#error No chip configuration
	#endif
#endif

#endif
