#ifndef __PWM_H_
#define __PWM_H_
#include "HAL_device.h"
#include "HAL_conf.h"
extern uint32_t SystemCoreClock;
//gate driver使能脚对应IO时钟
#define GATE_DRIVER_GPIO_CLK       RCC_AHBPeriph_GPIOB
//gate driver使能脚对应PWM引脚
#define GATE_DRIVER_PORT            GPIOB
#define GATE_DRIVER_PIN             GPIO_Pin_12
#define GATE_DRIVER_ENABLE()        GPIO_SetBits(GATE_DRIVER_PORT,GATE_DRIVER_PIN)
#define GATE_DRIVER_DISABLE()       GPIO_ResetBits(GATE_DRIVER_PORT,GATE_DRIVER_PIN)

//BLDC对应PWM输出IO时钟
#define BLDC_GPIO_CLK               (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB)
//BLDC对应PWM引脚
#define BLDC_UH_PORT                GPIOA
#define BLDC_UH_PIN                 GPIO_Pin_10
#define BLDC_VH_PORT                GPIOA
#define BLDC_VH_PIN                 GPIO_Pin_8
#define BLDC_WH_PORT                GPIOB
#define BLDC_WH_PIN                 GPIO_Pin_14

#define BLDC_UL_PORT                GPIOA
#define BLDC_UL_PIN                 GPIO_Pin_9
#define BLDC_VL_PORT                GPIOB
#define BLDC_VL_PIN                 GPIO_Pin_15
#define BLDC_WL_PORT                GPIOB
#define BLDC_WL_PIN                 GPIO_Pin_13
//BLDC刹车IO
#define BLDC_BKP_PORT               GPIOA
#define BLDC_BKP_PIN                GPIO_Pin_6

#define BLDC_UH_PIN_SRC             GPIO_PinSource10
#define BLDC_VH_PIN_SRC             GPIO_PinSource8
#define BLDC_WH_PIN_SRC             GPIO_PinSource14
#define BLDC_UL_PIN_SRC             GPIO_PinSource9
#define BLDC_VL_PIN_SRC             GPIO_PinSource15
#define BLDC_WL_PIN_SRC             GPIO_PinSource13
#define BLDC_BKP_PIN_SRC            GPIO_PinSource6
//BLDC对应PWM与BKP引脚复用选择
#define BLDC_UH_PIN_AF              GPIO_AF_6
#define BLDC_VH_PIN_AF              GPIO_AF_6
#define BLDC_WH_PIN_AF              GPIO_AF_6
#define BLDC_UL_PIN_AF              GPIO_AF_6
#define BLDC_VL_PIN_AF              GPIO_AF_6
#define BLDC_WL_PIN_AF              GPIO_AF_6
#define BLDC_BKP_PIN_AF             GPIO_AF_6
//BLDC使用TIM相关配置
#define BLDC_PWM_TIM                TIM1
#define BLDC_TIM_CLOCK_CMD          RCC_APB2PeriphClockCmd
#define BLDC_TIM_CLK                RCC_APB2Periph_TIM1
#define BLDC_TIM_PWM_FREQ           22000
#define BLDC_TIM_PRESCALER          0
#define BLDC_TIM_PERIOD             (uint16_t)(SystemCoreClock/(BLDC_TIM_PRESCALER+1)/BLDC_TIM_PWM_FREQ)

#define BLDC_TIM_REPETITIONCOUNTER  0
#define BLDC_TIM_DEADTIME           5


#define BLDC_UH_ENABLE()            TIM_CCxCmd(BLDC_PWM_TIM, TIM_Channel_1, TIM_CCx_Enable)     //CC1NE=1: enable OC1 output
#define BLDC_UH_DISABLE()           TIM_CCxCmd(BLDC_PWM_TIM, TIM_Channel_1, TIM_CCx_Disable)    //CC1NE=0: disable OC1 output
#define BLDC_VH_ENABLE()            TIM_CCxCmd(BLDC_PWM_TIM, TIM_Channel_2, TIM_CCx_Enable)     //CC2NE=1: enable OC2 output
#define BLDC_VH_DISABLE()           TIM_CCxCmd(BLDC_PWM_TIM, TIM_Channel_2, TIM_CCx_Disable)    //CC2NE=0: disable OC2 outpu
#define BLDC_WH_ENABLE()            TIM_CCxCmd(BLDC_PWM_TIM, TIM_Channel_3, TIM_CCx_Enable)     //CC3NE=1: enable OC3 output
#define BLDC_WH_DISABLE()           TIM_CCxCmd(BLDC_PWM_TIM, TIM_Channel_3, TIM_CCx_Disable)    //CC3NE=0: disable OC3 output

#define BLDC_UL_ENABLE()            TIM_OC1NPolarityConfig(BLDC_PWM_TIM,TIM_OCNPolarity_Low)     //CC1NE=1: enable OC1 output
#define BLDC_UL_DISABLE()           TIM_OC1NPolarityConfig(BLDC_PWM_TIM,TIM_OCNPolarity_High)    //CC1NE=0: disable OC1 output
#define BLDC_VL_ENABLE()            TIM_OC2NPolarityConfig(BLDC_PWM_TIM,TIM_OCNPolarity_Low)     //CC2NE=1: enable OC2 output
#define BLDC_VL_DISABLE()           TIM_OC2NPolarityConfig(BLDC_PWM_TIM,TIM_OCNPolarity_High)    //CC2NE=0: disable OC2 outpu
#define BLDC_WL_ENABLE()            TIM_OC3NPolarityConfig(BLDC_PWM_TIM,TIM_OCNPolarity_Low)     //CC3NE=1: enable OC3 output
#define BLDC_WL_DISABLE()           TIM_OC3NPolarityConfig(BLDC_PWM_TIM,TIM_OCNPolarity_High)    //CC3NE=0: disable OC3 output

#define SET_DUTY_U(duty)            TIM_SetCompare1(BLDC_PWM_TIM, duty)
#define SET_DUTY_V(duty)            TIM_SetCompare2(BLDC_PWM_TIM, duty)
#define SET_DUTY_W(duty)            TIM_SetCompare3(BLDC_PWM_TIM, duty)

extern void Bldc_Pwm_Init(void);
extern void Gate_Driver_Init(void);

#endif
