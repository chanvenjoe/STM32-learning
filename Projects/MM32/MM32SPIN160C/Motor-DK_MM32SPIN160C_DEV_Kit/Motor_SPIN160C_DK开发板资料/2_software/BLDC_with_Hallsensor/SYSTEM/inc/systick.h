#ifndef __SYSTICK_H
#define __SYSTICK_H 			   
#include "sys.h"
	 
extern void Systick_Init(void);

extern void Systick_Delay(__IO uint32_t Delay);
#endif

