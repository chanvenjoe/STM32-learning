#ifndef _RNG__H
#define _RNG__H
#include "sys.h"
#include "delay.h"
u8 RNG_Init(void);
u32 RNG_Get_RandomNum(void);
int RNG_Get_RandomRange(int min, int max);

#endif

