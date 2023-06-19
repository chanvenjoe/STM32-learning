/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.0
* @date     09/09/2019
* @brief
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/
#include "HAL_device.h"
#include "HAL_conf.h"

extern u32 SystemCoreClock;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

static __IO uint32_t TimingDelay;

#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	// PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//��ȡ����4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 
void setalarmtime_ms(uint32_t nTime);
void setLEDtoggle(u32 freq);

uint32_t SysTickCnt = 0;
uint32_t systickLEDfreq;

void Sys_Standby(void);
/*
slow fast blink then slow 1s blink,
and wait key3(wkup key) to enter standby
if get Key3 enter standby
if get Other key , slow other freq led toggle
enter standby, MCU current is about 0.5uA
if get wkup key , then wake up from Standby as reset;
*/
/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u8 t = 0;

    setLEDtoggle(1000);
    delay_init();
    LED_Init();                                                                 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();                                                                 //��ʼ���밴�����ӵ�Ӳ���ӿ�
    //�Ѵ�standbyģʽ���ѻ���ϵ�

    //����6s 100msһ�µ�LED��
    setLEDtoggle(100);
    delay_ms(6000);
    //����1000msһ�µ�LED��˸
    setLEDtoggle(1000);
    //
    //�밴��KEY3��������Standby�������������л�����Ƶ����˸LED

    while(1)
    {
        t = KEY_Scan(0);                                                        //�õ���ֵ
        if(KEY1_PRES == t)
        {
            setLEDtoggle(300);
        }
        else if(WKUP_PRES == t)
        {
            setLEDtoggle(600);
        }
        else if(KEY3_PRES == t)
        {
            //�밴��KEY3��������Standby
            Sys_Standby();
            //����Standby��ֻ�а���WKUP��PA0����������MCU����λ����MCU
        }
        else if(KEY4_PRES == t)
        {
            setLEDtoggle(2000);
        }
        else
        {
            delay_ms(10);
        }
    }
}

/********************************************************************************************************
**������Ϣ ��Sys_Standby(void)
**�������� ���������ģʽ
**������� ����
**������� ����
********************************************************************************************************/
void Sys_Standby(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);                         //ʹ��PWR����ʱ��
    //	RCC->APB2RSTR|=0X01FC;//��λ����IO��
    PWR_WakeUpPinCmd(ENABLE);                                                   //ʹ�ܻ��ѹܽŹ���
    PWR_EnterSTANDBYMode();                                                     //���������STANDBY��ģʽ
}
void setLEDtoggle(u32 freq)
{
    if((freq > 10000) || (freq == 0))
        freq = 10000;
    systickLEDfreq = freq;
}
static u32 abc = 0;
void ledtogglefreq(void)
{


    if(abc < systickLEDfreq)
        abc++;
    else
    {
        abc = 0;
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
    }
}


void setalarmtime_ms(uint32_t nTime)
{
    TimingDelay = nTime;
}
void checkalarmtime_ms(uint32_t *pnTime)
{
    *pnTime = TimingDelay;
}
static u8 key_up = 1;                                                           //�������ɿ���־

u8 Systick_KEY_Scan(u8 mode)
{
    if(mode)key_up = 1;                                                         //֧������
    if(key_up && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0))
    {
        delay_ms(10);                                                           //ȥ����
        key_up = 0;
        if(KEY1 == 0)return KEY1_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
        else if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
    }
    else if(KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;                                                                   //�ް�������
}

/********************************************************************************************************
**������Ϣ ��delay_init(void)
**�������� ��systick��ʱ������ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);                                        //SysTick�ж����ȼ�����
}

/********************************************************************************************************
**������Ϣ ��SysTick_Handler(void)
**�������� ��������жϺ�����Systick���еݼ�
**������� ����
**������� ����
********************************************************************************************************/
void SysTick_Handler(void)
{
    SysTickCnt++;
    ledtogglefreq();
    TimingDelay_Decrement();
}

/********************************************************************************************************
**������Ϣ ��TimingDelay_Decrement(void)
**�������� ����1ms���ٶȵݼ�
**������� ��pclk2������ϵͳʱ��Ϊ8MHz������Ӧ����8
**������� ����
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

/********************************************************************************************************
**������Ϣ ��delay_ms(__IO uint32_t nTime)
**�������� ������Ӧ�õ�����ʱ��ʹ��systick
**������� ��nTime����ʱ
**������� ����
********************************************************************************************************/
void delay_ms(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/*******************************************************************************
* @name   : GPIO_Clock_Set
* @brief  : RCC clock set
* @param  : Portx , State
* @retval : void
*******************************************************************************/
void GPIO_Clock_Set(GPIO_TypeDef* GPIOx, FunctionalState NewState)
{

    if(GPIOx == GPIOA)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);                   //GPIO clock starts
    }
    if(GPIOx == GPIOB)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);                   //GPIO clock starts
    }
    if(GPIOx == GPIOC)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);                   //GPIO clock starts
    }
    if(GPIOx == GPIOD)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, NewState);                   //GPIO clock starts
    }
}
/********************************************************************************************************
**������Ϣ ��LED_Init(void)
**�������� ��LED��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_Clock_Set(GPIOA, ENABLE);  //����GPIOAʱ��
    GPIO_Clock_Set(GPIOB, ENABLE);  //����GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}

/********************************************************************************************************
**������Ϣ ��void KEY_Init(void)
**�������� ��������ʼ������
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_Clock_Set(GPIOA, ENABLE);  //����GPIOAʱ��
    GPIO_Clock_Set(GPIOB, ENABLE);  //����GPIOBʱ��
    GPIO_Clock_Set(GPIOC, ENABLE);  //����GPIOCʱ��
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //PC13��K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    GPIO_Init(GPIOC, &GPIO_InitStructure);                                      //��ʼ��GPIOC13

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2��WK_UP��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA0

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;                   //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //��ʼ��GPIOB.10,11
}

/********************************************************************************************************
**������Ϣ ��u8 KEY_Scan(u8 mode)
**�������� ������������
**������� ��mode  mode:0,��֧��������;1,֧��������;
**������� ��0,û�а�������;KEY1_PRES,KEY1����;WKUP_PRES,WK_UP����;KEY3_PRES,KEY3����;KEY4_PRES��KEY4����
**    ��ע ��
********************************************************************************************************/
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;                                                       //�������ɿ���־
    if(mode)key_up = 1;                                                         //֧������
    if(key_up && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0))
    {
        delay_ms(10);                                                           //ȥ����
        key_up = 0;
        if(KEY1 == 0)return KEY1_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
        else if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
    }
    else if(KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;                                                                   //�ް�������
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

