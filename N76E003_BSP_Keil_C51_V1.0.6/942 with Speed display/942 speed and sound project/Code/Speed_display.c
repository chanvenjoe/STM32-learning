#include "Speed_display.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#define Bright 2
#define u8 unsigned char
#define u16 unsigned int

u8 num_arr[] = 
{	
	0x3f,//0
	0x06,//1
	0x5b,//2
	0x4f,//3
	0x66,//4
	0x6d,//5
	0x7d,//6
	0x07,//7
	0x7f,//8
	0x6f //9
}; //uchar == u8 uint == u16

u8 digit_arr[] =
{
	0xfe,  //1 thous
	0xfd, //2 hund
	0xfb, //3 ten
	0xf7 //4 unit	
};

	
struct
{
	u8 units;
	u8 tens;
	u8 hundreds;
	u8 thousands;
}speed;

void GPIO_SetBits(u8 GPIOx,u8 GPIO)
{
	switch(GPIOx)
	{
		case 0:
			P0 = GPIO;
			break;
		case 1:
			P1 = GPIO;
			break;
	}
	
}

void digit_selc_show(u8 bits, u8 num)
{
	GPIO_SetBits(0,0x00);// num
	GPIO_SetBits(1,0xff); //bits
	GPIO_SetBits(1,digit_arr[bits]);
	GPIO_SetBits(0,num_arr[num]);
}

void numb_show(u16 n)
{
	u8 i;
	n = n*71;//m/h
	speed.thousands = n/1000%10;
	speed.hundreds 	= n/100%10;
	speed.tens		= n/10%10;
	speed.units	    = n%10;

	for(i=0;i<4;i++)
	{
		switch(i)
		{
			case 0:
				digit_selc_show(i,speed.thousands);
				break;
			case 1:
				digit_selc_show(i,speed.hundreds);
				break;
			case 2:
				digit_selc_show(i,speed.tens);
				break;
			case 3:
				digit_selc_show(i,speed.units);
				break;
			default:
			break;
		}
	}
}
