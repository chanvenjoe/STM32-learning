#include "WS2811.h"
#include "sys.h"

/***********TYPE DEFINE****************/

/***********VARIABLE DEFINE************/

/***********Constant variable**********/

#define RGB_HSV 0		//1 to enable transformation  code
/**************************************/

Color_Typedef LED_Buf[LEDNUM];
u32 Color_circle[] = {White, Black, Green, Red, Blue};

void WS_Send24bits(u32 color)//Be Careful about the direction of rope: Din->Dout
{
	u8 i;
//	u32 sftcolor = color;
//	sftcolor<<8;
	for(i=0;i<24;i++)
	{
		if(color & 0x00800000)// 0000 0000 1000 0000... the highest bit & color, then the MCU can send 24bits data to WS2812
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
	for(temp = from;temp<=to; temp++)
	{
		WS_Color_copy(temp, color);
	}
}

void WS_Refresh()
{
	u8 i;
	WS_Send24bits(dummy);
	for( i=0;i<=LEDNUM+1;i++)
	{
		WS_Send24bits(LED_Buf[i].RGB);
	}
	RES
}

void WS_Key_RGB(void)
{
	static u8 temp = 0;
	switch (temp)
	{
		case 0:
			WS_ColorSet_LED(0, LEDNUM, Black);
			WS_Refresh();
			WS_ColorSet_LED(0, 3, Red);
			WS_Refresh();
			break;
		case 1:
			WS_ColorSet_LED(0, LEDNUM, Black);
			WS_Refresh();
			WS_ColorSet_LED(4, 6, Blue);
			WS_Refresh();
			break;
		case 2:
			WS_ColorSet_LED(0, LEDNUM, Black);
			WS_Refresh();
			WS_ColorSet_LED(7,10, Green);
			WS_Refresh();
			break;
	}
	temp++;
	if(temp>2)
		temp = 0;
			
		
//		WS_Refresh();
		delay_ms(50);
}


void WS_Hue_change()
{
//	u8 cylon=160;
	u8 cycle;
//	u32 temp;
	static int h=160;
	if(h>=0&&h<360)
	{
		for(cycle=0;cycle<=LEDNUM;cycle++)
		{
			WS_ColorSet_LED(0, cycle, HSV_RGB(h, 1,1,0,0,0));
			WS_Refresh();
			delay_ms(50);
		}
		h+=15;
	}
		else
			h=0;
}


#if(RGB_HSV)
u8	Get_RGB_Max(u8 R, u8 G, u8 B)
{
	u8 Max=R;
	if(Max<G)
	{
		Max=G;
	}
	if(Max<B)
	{
		Max=B;
	}
	return Max;
}
u8	Get_RGB_Main(u8 R, u8 G, u8 B)
{
	u8 Min=R;
	if(Min>G)
	{
		Min=G;
	}
	if(Min>B)
	{
		Min=B;
	}
	return Min;
}

u8	Get_Hue_Val(u8 R, u8 G, u8 B)
{
	u8 Max, Min,Num,Hue;
	Max = Get_RGB_Max(R,G,B);
	Min = Get_RGB_Min(R,G,B);
	if(Max==Min)
	{
		Num=First;
	}
	else if(Max==R&&(G>=B))
	{
		Num=Second;
	}
	else if(Max==R&&(G<B))
	{
		Num=Third;
	}
	else if(Max==G)
	{
		Num=Fourth;
	}
	else if(Max==B)
	{
		Num=Fifth;
	}
	else
	{
		Num=Non;
	}
	switch(Num)
	{
		case 0:
		break;
		case 1:
			Hue=0;
		break;
		case 2:
			Hue=60*((R-G)/(Max-Min));
		break;
		case 3:
			Hue=60*((R-G)/(Max-Min))+360;
		break;
		case 4:
			Hue=60*((R-G)/(Max-Min))+120;
		break;
		case 5:
			Hue=60*((R-G)/(Max-Min))+240;
		break;
	}
	return Hue;
}
#else
u32 HSV_RGB(int h, char s, char v, float R, float G, float B)
{
	u32 RGB;
	u8 R8,G8,B8;
	char  i;
	float X,Y,Z,C;
	i = h/60;		//char i
	C = h;		//float C
	C = C/60-i;
//	char X = C*(1- (abs((h/60)%2-1)));
	X = v*(1-s);
	Y = v*(1-(s*C));
	Z = v*(1-s*(1-C));
	
	switch(i)
	{
		case 0:
			R=v; G=Z; B=X;
			break;
		case 1:
			R=Y; G=v; B=X;
			break;
		case 2:
			R=X; G=v; B=Z;
			break;
		case 3:
			R=X; G=Y; B=v;
			break;
		case 4:
			R=Z; G=X; B=v;
			break;
		case 5:
			R=v; G=X; B=Y;
			break;
	}
	R8=R*255;
	G8=G*255;
	B8=B*255;
	RGB|=R8;
	RGB<<=8;
	RGB|=G8;
	RGB<<=8;
	RGB|=B8;
	return RGB;
}

#endif
