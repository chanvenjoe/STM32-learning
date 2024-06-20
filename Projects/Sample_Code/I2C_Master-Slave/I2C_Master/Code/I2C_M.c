/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/29/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 I2C master mode demo code, the Slave address = 0xA4
//
//   ____________            _____________ 
//  |            |   SDA    |             |
//  |            |<-------->|             |
//  |            |          |             |
//  |N76E003(M) |          | N76E003(S) |
//  |(I2C_Master)|          | (I2C_Slave) |
//  |            |   SCL    |             |
//  |            |--------->|             |
//  |____________|          |_____________|
//
//  The protocol of I2C is same the "24LC64"
//***********************************************************************************************************


#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define I2C_CLOCK               13
#define EEPROM_SLA              0x52
#define EEPROM_WR               0
#define EEPROM_RD               1
#define ERROR_CODE              0x78
#define PAGE_SIZE               32

//========================================================================================================
void Init_I2C(void)
{
//    /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2CEN;                                   
}
//========================================================================================================
void I2C_Error(void)
{
//    P3 = I2STAT;
//    P3 = ERROR_CODE;
    while (1);    
}
//========================================================================================================
void I2C_Process(UINT8 u8DAT)
{
    UINT32 u32Count;

//--------------------------------------------------------------------------------------------
//----  Page Write----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step1 */
    set_STA;                                    /* Send Start bit to I2C EEPROM */
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x08)                         //Check status value after every step
        I2C_Error();
    
    /* Step2 */
    clr_STA;                                    //STA=0
    I2DAT = (EEPROM_SLA | EEPROM_WR);
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x18)              
        I2C_Error();

    /* Step3 */
    I2DAT = 0x00;                               //address high for I2C EEPROM
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x28)              
        I2C_Error();
                    
    /* Step4 */
    I2DAT = 0x00;                               //address low for I2C EEPROM
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x28)              
        I2C_Error();

    /* Step5 */

//--------------------------------------------------------------------------------------------
//----  Waitting the ready for I2C write------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step6 */
    do{
        set_STO;
        clr_SI;
        
        set_STA;                                //Check if no ACK is returned by EEPROM, it is under timed-write cycle
        clr_SI;
        while (!SI);                            //Check SI set or not
        if (I2STAT != 0x08)                     //Check status value after every step
            I2C_Error();

        clr_STA;
        I2DAT = (EEPROM_SLA | EEPROM_WR);
        clr_SI;
        while (!SI);                            //Check SI set or not
    }while (I2STAT != 0x18);
    
    /* Step7 */
    set_STO;
    clr_SI;
    while (STO);                                /* Check STOP signal */
//--------------------------------------------------------------------------------------------
//----  Page Read ----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step8 */
    set_STA;
    clr_SI;          
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x08)                         //Check status value after every step
        I2C_Error();

    /* Step9 */
    I2DAT = (EEPROM_SLA | EEPROM_WR);
    clr_STA;
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x18)              
        I2C_Error();


    /* Step12 */
    /* Repeated START */
    set_STA;                       
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x10)                         //Check status value after every step
        I2C_Error();
    
    /* Step13 */
    clr_STA;                                    //STA needs to be cleared after START codition is generated
    I2DAT = (EEPROM_SLA | EEPROM_RD);					//6 axel ID
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x40)              
        I2C_Error();
    
    /* Step14 */

        set_AA;
        clr_SI;        
        while (!SI);                            //Check SI set or not
	
	I2DAT = (0x12);					//6 axel ID
    clr_SI;

        if (I2STAT != 0x50)              
 //           I2C_Error();
		
	I2DAT = (0x12);					//6 axel ID
    clr_SI;

        if (I2STAT != 0x50)              
 //           I2C_Error();
        
    
    
    /* Step15 */
    clr_AA;
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x58)              
        I2C_Error();

    /* Step16 */
    set_STO;
    clr_SI;
    while (STO);                                /* Check STOP signal */ 
}
//========================================================================================================
void main(void)
{
    /* Note
       MCU power on system clock is HIRC (22.1184MHz), so Fsys = 22.1184MHz
    */
    
    Set_All_GPIO_Quasi_Mode;	
    Init_I2C();                                 //initial I2C circuit
	
	//Write
	set_STA;
    clr_SI;          
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x08)                         //Check status value after every step
        I2C_Error();

    /* Step9 */
    I2DAT = (EEPROM_SLA | EEPROM_WR);
    clr_STA;
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x18)              


    /* Step9 */
    I2DAT = 0x12|0x80;
    clr_STA;
    clr_SI;
    while (!SI);                                //Check SI set or not
//    if (I2STAT != 0x18)       

    I2DAT = 0x00|0x80;
    clr_STA;
    clr_SI;
    while (!SI);                                //Check SI set or not
//    if (I2STAT != 0x18)  	
	
	I2DAT = 0x12|0x80;
    clr_STA;
    clr_SI;
    while (!SI);                                //Check SI set or not
//    if (I2STAT != 0x18)         
		
	
	
	set_STA;                       
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x10)                         //Check status value after every step

    
    /* Step13 */
    clr_STA;                                    //STA needs to be cleared after START codition is generated
    I2DAT = (EEPROM_SLA | EEPROM_RD);					//6 axel ID
    clr_SI;
	set_AA;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x40)              

	I2DAT = 0x12|0x80;
	clr_SI;

	
	I2DAT = 0x12|0x80;					//
    clr_SI;         

		

	
	
//    I2C_Process(0x55);                          /* I2C Master will send 0x55,0xAA,.... to slave */
//    
//    P0 = 0x00;
//    P3 = 0x00;
    
    while (1);
/* =================== */
}

