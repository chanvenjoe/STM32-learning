#include "Speed_display.h"
#include "sys.h"
#include "delay.h"

#define Bright 2

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


struct
{
	u8 units;
	u8 tens;
	u8 hundreds;
	u8 thousands;
}speed;

	
u8 digit_arr[] =
{
	0x01,  //1 thous
	0x02, //2 hund
	0x04, //3 ten
	0x08 //4 unit	
};

void digit_selc_show(u8 bits, u8 num)
{
	GPIO_SetBits(GPIOC,0xff);
	GPIO_ResetBits(GPIOF,0xff);
	GPIO_ResetBits(GPIOC,digit_arr[bits]);
	GPIO_SetBits(GPIOF,num_arr[num]);
	delay_ms(10);
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
	
//	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
//	delay_ms(Bright);
//	GPIO_SetBits(GPIOC,GPIO_Pin_0);
//	delay_ms(Bright);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//	delay_ms(Bright);
//	GPIO_SetBits(GPIOC,GPIO_Pin_1);
//	delay_ms(Bright);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
//	delay_ms(Bright);
//	GPIO_SetBits(GPIOC,GPIO_Pin_2);
//	delay_ms(Bright);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
//	delay_ms(Bright);
//	GPIO_SetBits(GPIOC,GPIO_Pin_3);
//	delay_ms(Bright);
}
