#include "WS2811.h"
#include "sys.h"


void WS_Send24bits(unsigned long color)//Care about the direction of rope: Din->Dout
{
	u8 i;
	for(i=0;i<24;i++)
	{
		if(color & 0x00800000)
		{
			CODE_1
		}
		else CODE_0
		color<<=1;
	}
}
