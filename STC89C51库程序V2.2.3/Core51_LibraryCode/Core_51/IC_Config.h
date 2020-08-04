#ifndef __IC_CONFIG_H
#define __IC_CONFIG_H

//STCϵ�е�Ƭ��
#define			STC							1
#define 			STC89C52RC  				1
#define			STC12C5A60S2				0	

//STCϵ�е�Ƭ������������ʾ
#if STC
	#if ((STC89C52RC + STC12C5A60S2) > 1)
		#error Multiple chips are configured
	#elif	((STC89C52RC + STC12C5A60S2) == 0)
		#error No chip configuration
	#endif
#endif

#endif
