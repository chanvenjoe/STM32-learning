#ifndef _LCD_INIT_H
#define _LCD_INIT_H
#include "sys.h"



typedef struct
{
	vu16 LCD_REG;//volatile:Each time the mcu read this variable, read if from its address 
	vu16 LCD_RAM;
}LCD_Type_Def;

#define LCD_BASE	((u32)(0x6C000000|0x0000007E))//7E is the address  of A6 as reset pin to differ data from order
#define LCD				((LCD_Type_Def*)LCD_BASE)//(LCD_Type_Def*) is a structure pointer

typedef struct
{
	u16 width;
	u16 heigh;
	u16 id;
	u8  dir;
	u16 wramcmd;
	u16 setxcmd;
	u16 setycmd;
}_lcd_dev; //"_"for system declaration
extern _lcd_dev lcddev;
	
void LCD_WR_REG(vu16 regval);
void LCD_WR_DATA(vu16 data);
u16 LCD_RD_DATA(void);
void LCD_WriteReg(vu16 LCD_Reg, vu16 LCD_RegVlue);
u16 LCD_ReadReg(vu16 LCD_Reg);
void LCD_WriteRAM(u16 RGB_CODE);
void LCD_WriteRAM_Prepare(void);
void LCD_DrawPoint(u16 x,u16 y);
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_Clear(u16 color);
#endif 
