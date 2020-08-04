#ifndef __DELAY_H
#define __DELAY_H

#include "include_drives.h"

#if XTAL_24MHz
	#define XTAL 24
#elif XTAL_12MHz
	#define XTAL 12
#elif XTAL_110592MHz
	#define XTAL 11.0592
#endif

#if STC89C52RC
	void Delay_Xms(unsigned int n);		//—” ± n*1ms
	void Delay_Xs(unsigned int n);		//—” ± n*1s
	void Delay_X10us(unsigned int n);	//—” ± n*10us
#endif

#endif
