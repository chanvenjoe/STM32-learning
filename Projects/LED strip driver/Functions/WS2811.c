#include "WS2811.h"
#include "N76E003.h"
#include "Motor_control.h"

/***********TYPE DEFINE****************/



/***********VARIABLE DEFINE************/

/***********Constant variable**********/

#define RGB_HSV 0		//1 to enable transformation  code
/**************************************/

Color_Typedef LED_Buf[LEDNUM+1];

u16 sum;

void WS_Send24bits(u32 color)//Be Careful about the direction of rope: Din->Dout
{
	u8 i;
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
	if(to>LEDNUM||from<0)
	{
		to=LEDNUM;
		from=0;
	}
	color &= 0x00ffffff;// remove the effect of [32-25]
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
//	WS_Send24bits(dummy);
	for( i=0;i<=LEDNUM;i++)
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
		Timer0_Delay1ms(50);
}


void WS_Hue_change()
{
	u8 cylon=160;
	u8 cycle;
	static u8 flag=1;
//	u32 temp;
	if(h>=0&&h<360)
	{
		if(flag)
		{
			WS_ColorSet_LED(0, LEDNUM, HSV_RGB(h, 1,1,0,0,0));
			WS_Refresh();
			h+=HSV_Ch_Sp;
			Timer0_Delay1ms(HSV_Delay1ms);
			if(h>=360){
				flag=0;
			}
		}
		else
		{
			for(cycle=0;cycle<=LEDNUM;cycle++)
			{
				WS_ColorSet_LED(0, cycle, HSV_RGB(h, 1,1,0,0,0));
				Timer0_Delay1ms(HSV_Delay1ms);
			}
			WS_Refresh();
			h-=HSV_Ch_Sp;
			if(h<=0)
				flag=1;
		}
			
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
#if WS2812B
		G8=R*255;
		R8=G*255;
		B8=B*255;
#else
		R8=R*255;
		G8=G*255;
		B8=B*255;
#endif
	RGB&=0x00000000;
	RGB|=(R8>>4);
	RGB<<=8;
	RGB|=(G8>>4);
	RGB<<=8;
	RGB|=(B8>>4);
	return RGB;
}
void WS_voice_Pik(u8 mode) //Mode0: all LED together Mode1:LED light up corresponding number according to the dB
{
	u8 dB,i;
	u32 color=0;
	for(i=0;i<6;i++)
	{
		sum+=Get_HallValue();
	}
	sum=sum/6;
	dB = sum;
	sum=0;
	if(mode==0)
	{
		if(dB>=30&&dB<100)
		{
			color = Black|(dB*2);
			WS_ColorSet_LED(0,LEDNUM,color);
			WS_Refresh();
			Timer1_Delay10ms(30);
			while(color)
			{
				color = (color-FadeoutTime)>FadeoutTime? color-FadeoutTime:0;
				WS_ColorSet_LED(0,LEDNUM,color);
				WS_Refresh();
			}
		}
		else if(dB>=100)
		{
			color = Black|dB;
			color = LED_type? color<<8:color<<16;
			WS_ColorSet_LED(0,LEDNUM,color);
			WS_Refresh();
			Timer1_Delay10ms(30);
			while(dB)
			{
				dB = (dB-FadeoutTime)>FadeoutTime? dB-FadeoutTime:0;
				color = Black|dB;
				color = LED_type? color<<8:color<<16;
				WS_ColorSet_LED(0,LEDNUM,color);
				WS_Refresh();
			}
		}
		else
		{
			WS_ColorSet_LED(0,LEDNUM,0x000000);
			WS_Refresh();
			Timer1_Delay10ms(5);			
		}
	}
	else//Reactive mode
	{
		WS_ColorSet_LED(0, LEDNUM, Black);
		if(dB>=25)
		{
			u8 Light_up;
			u8 arr;
			Light_up=((dB*LEDARR)/150)>8?8:((dB*LEDARR)/150);//The power supply is not 5V so the ADC value can not reach 255
			for(arr=0;arr<LEDARR;arr++)
			{
				WS_ColorSet_LED(0+arr*LEDARR, Light_up+arr*LEDARR, Blue);
			}
			WS_Refresh();
			while(Light_up--)
			{
				WS_ColorSet_LED(0, LEDNUM, Black);
				for(arr=0;arr<LEDARR;arr++)
				{
					WS_ColorSet_LED(0+arr*LEDARR, Light_up+arr*LEDARR, Blue);
				}
				WS_Refresh();
				Timer1_Delay10ms(10);
			}
		}
		else
		{
//			WS_ColorSet_LED(0, LEDNUM, Black);
			WS_Refresh();
		}
	}
}


#endif
