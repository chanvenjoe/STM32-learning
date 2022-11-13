#include "i2c.h"
#include "delay.h"
#include "tcs34725.h"

#define TCS34725_ADDRESS          (0x29)

#define TCS34725_COMMAND_BIT      (0x80)

#define TCS34725_ENABLE           (0x00)
#define TCS34725_ENABLE_AIEN      (0x10)    /* RGBC Interrupt Enable */
#define TCS34725_ENABLE_WEN       (0x08)    /* Wait enable - Writing 1 activates the wait timer */
#define TCS34725_ENABLE_AEN       (0x02)    /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS34725_ENABLE_PON       (0x01)    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */
#define TCS34725_ATIME            (0x01)    /* Integration time */
#define TCS34725_WTIME            (0x03)    /* Wait time (if TCS34725_ENABLE_WEN is asserted) */
#define TCS34725_WTIME_2_4MS      (0xFF)    /* WLONG0 = 2.4ms   WLONG1 = 0.029s */
#define TCS34725_WTIME_204MS      (0xAB)    /* WLONG0 = 204ms   WLONG1 = 2.45s  */
#define TCS34725_WTIME_614MS      (0x00)    /* WLONG0 = 614ms   WLONG1 = 7.4s   */
#define TCS34725_AILTL            (0x04)    /* Clear channel lower interrupt threshold */
#define TCS34725_AILTH            (0x05)
#define TCS34725_AIHTL            (0x06)    /* Clear channel upper interrupt threshold */
#define TCS34725_AIHTH            (0x07)
#define TCS34725_PERS             (0x0C)    /* Persistence register - basic SW filtering mechanism for interrupts */
#define TCS34725_PERS_NONE        (0b0000)  /* Every RGBC cycle generates an interrupt                                */
#define TCS34725_PERS_1_CYCLE     (0b0001)  /* 1 clean channel value outside threshold range generates an interrupt   */
#define TCS34725_PERS_2_CYCLE     (0b0010)  /* 2 clean channel values outside threshold range generates an interrupt  */
#define TCS34725_PERS_3_CYCLE     (0b0011)  /* 3 clean channel values outside threshold range generates an interrupt  */
#define TCS34725_PERS_5_CYCLE     (0b0100)  /* 5 clean channel values outside threshold range generates an interrupt  */
#define TCS34725_PERS_10_CYCLE    (0b0101)  /* 10 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_15_CYCLE    (0b0110)  /* 15 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_20_CYCLE    (0b0111)  /* 20 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_25_CYCLE    (0b1000)  /* 25 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_30_CYCLE    (0b1001)  /* 30 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_35_CYCLE    (0b1010)  /* 35 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_40_CYCLE    (0b1011)  /* 40 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_45_CYCLE    (0b1100)  /* 45 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_50_CYCLE    (0b1101)  /* 50 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_55_CYCLE    (0b1110)  /* 55 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_60_CYCLE    (0b1111)  /* 60 clean channel values outside threshold range generates an interrupt */
#define TCS34725_CONFIG           (0x0D)
#define TCS34725_CONFIG_WLONG     (0x02)    /* Choose between short and long (12x) wait times via TCS34725_WTIME */
#define TCS34725_CONTROL          (0x0F)    /* Set the gain level for the sensor */
#define TCS34725_ID               (0x12)    /* 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS34725_STATUS           (0x13)
#define TCS34725_STATUS_AINT      (0x10)    /* RGBC Clean channel interrupt */
#define TCS34725_STATUS_AVALID    (0x01)    /* Indicates that the RGBC channels have completed an integration cycle */
#define TCS34725_CDATAL           (0x14)    /* Clear channel data */
#define TCS34725_CDATAH           (0x15)
#define TCS34725_RDATAL           (0x16)    /* Red channel data */
#define TCS34725_RDATAH           (0x17)
#define TCS34725_GDATAL           (0x18)    /* Green channel data */
#define TCS34725_GDATAH           (0x19)
#define TCS34725_BDATAL           (0x1A)    /* Blue channel data */
#define TCS34725_BDATAH           (0x1B)

#define TCS34725_INTEGRATIONTIME_2_4MS   0xFF   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
#define TCS34725_INTEGRATIONTIME_24MS    0xF6   /**<  24ms  - 10 cycles  - Max Count: 10240 */
#define TCS34725_INTEGRATIONTIME_50MS    0xEB   /**<  50ms  - 20 cycles  - Max Count: 20480 */
#define TCS34725_INTEGRATIONTIME_101MS   0xD5   /**<  101ms - 42 cycles  - Max Count: 43008 */
#define TCS34725_INTEGRATIONTIME_154MS   0xC0   /**<  154ms - 64 cycles  - Max Count: 65535 */
#define TCS34725_INTEGRATIONTIME_240MS   0x9C   /**<  240ms - 100 cycles - Max Count: 65535 */
#define TCS34725_INTEGRATIONTIME_700MS   0x00   /**<  700ms - 256 cycles - Max Count: 65535 */

#define TCS34725_GAIN_1X                 0x00   /**<  No gain  */
#define TCS34725_GAIN_4X                 0x01   /**<  4x gain  */
#define TCS34725_GAIN_16X                0x02   /**<  16x gain */
#define TCS34725_GAIN_60X                0x03   /**<  60x gain */

u8 regarr[10] = { 0x00,0};
void MyI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_SCLH;
	I2C_SDAH;
	
	I2C_Read(TCS34725_ADDRESS, TCS34725_ID,1,1);
	I2C_Read(TCS34725_ADDRESS, TCS34725_ENABLE_PON,1,1);
	I2C_Write(TCS34725_ADDRESS,regarr,1,1);
	I2C_Read(TCS34725_ADDRESS, TCS34725_ENABLE_PON,1,1);
//	while(TCS34725_Ready()==0x44);
}

void I2C_Write(u8 slaveaddr, u8 *arrdata, u8 bytes, u8 stopbit)
{
	u8 i;
	I2C_Start();
	I2C_Send_Byte(slaveaddr<<1|0X00);
	I2C_Wait_Ack();
	
	I2C_Send_Byte(TCS34725_ENABLE_PON|TCS34725_COMMAND_BIT);
	I2C_Wait_Ack();
	
	for(i=0;i<bytes;i++)
	{
		I2C_Send_Byte(* arrdata+i);
		I2C_Wait_Ack();
	}
	if(1==stopbit)
		I2C_Stop();
}

void I2C_Read(u8 slaveaddr, u8 regaddr, u8 bytes, u8 stopbit)
{
	I2C_Start();
	I2C_Send_Byte(slaveaddr<<1|0x00);//Write
	while(I2C_Wait_Ack());
	I2C_Send_Byte(regaddr|TCS34725_COMMAND_BIT);
	I2C_Wait_Ack();
	
	I2C_Start();
	I2C_Send_Byte(slaveaddr<<1|0x01);//Read
	I2C_Wait_Ack();
	
	u8 rxd = I2C_Read_Byte(0);
}
	

u8 TCS34725_Ready()
{
	u8 id;
	I2C_Start();
	I2C_Send_Byte(TCS34725_ADDRESS<<1|0x00);//Write
	while(I2C_Wait_Ack());
//	TCS34725_I2C_Wait_ACK();// the wait function cause the NACK

	I2C_Send_Byte(TCS34725_ID|TCS34725_COMMAND_BIT);
	while(I2C_Wait_Ack());

	I2C_Start();
	I2C_Send_Byte(TCS34725_ADDRESS<<1|0x01);//Read
	I2C_Wait_Ack();

	id = I2C_Read_Byte(0);
	I2C_Stop();
	return id;
}
void I2C_Start()
{
	SDA_OUT();
	I2C_SCLH;
	I2C_SDAH;
	delay_us(4);
	I2C_SDAL;
	delay_us(4);
	I2C_SCLL;
}

void I2C_Stop()	//    SDA ___/---
{				//	  SCL _|-----
	SDA_OUT();
	I2C_SCLL;
	I2C_SDAL;
	delay_us(4);
	I2C_SCLH;
	delay_us(4);
	I2C_SCLH;
	I2C_SDAH;
	delay_us(4);
}

u8 I2C_Wait_Ack()
{
	u8 Ack_ErrTim;

	SDA_IN();
	I2C_SDAH;
	delay_us(1);
	I2C_SCLH;
	delay_us(1);
	while(SDA_READ)
	{
		Ack_ErrTim++;
		if(Ack_ErrTim>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCLL;
//	I2C_SDA = 0;
	return 0;
}

void I2C_ACK()
{
	I2C_SCLL;
	SDA_OUT();
	I2C_SDAL;
	delay_us(2);
	I2C_SCLH;
	delay_us(2);
//	I2C_SDAH;
	I2C_SCLL;
}

void I2C_NACK()
{
	I2C_SCLL;
	SDA_OUT();
	I2C_SDAH;
	delay_us(2);
	I2C_SCLH;
	delay_us(2);
	I2C_SCLL;
}

void I2C_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	I2C_SCLL;
	for(t=0;t<8;t++)
	{
		if(((txd&0x80)>>7)==1)			//Send the highest bit first
			I2C_SDAH;
		else
			I2C_SDAL;
		txd<<=1;
		delay_s(20);
		I2C_SCLH;
		delay_s(20);
		I2C_SCLL;
		delay_s(20);
	}
}
/* ack=1; send ack*/
u8 I2C_Read_Byte(u8 ack)
{
	u8 i, receive=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		I2C_SCLL;delay_us(2);
		I2C_SCLH;
		receive<<=1;
		if(SDA_READ)
			receive++;
		delay_us(1);
	}
	if(!ack)
		I2C_NACK();
	else
		I2C_ACK();
	return receive;
}
		
		
	

