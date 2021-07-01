#include "WS2811.h"
#include "sys.h"

Color_Typedef LED_Buf[LEDNUM];
u32 Color_circle[] = {White, Black, Green, Red, Blue};

void WS_Send24bits(u32 color)//Be Careful about the direction of rope: Din->Dout
{
	color<<8;
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

void WS_Color_copy(u8 No, u32 color)
{
	LED_Buf[No].RGB = color;
}

void WS_ColorSet_LED(u8 from, u8 to, u32 color)
{
	static int temp;
	if(from>to)
	{
		temp=from; from=to; to=temp;
	}
	for(temp = from;temp<to; temp++)
	{
		WS_Color_copy(temp, color);
	}
}

void WS_Refresh()
{
	u8 i;
	for( i=0;i<LEDNUM;i++)
	{
		WS_Send24bits(LED_Buf[i].RGB);
	}
	//RES
}
