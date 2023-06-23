#include "sys.h"

/* 私有类型定义 --------------------------------------------------------------*/
typedef enum {
    CW      = 0,    // 顺时钟方向
    CCW     = 1     // 逆时针方向
} motor_dir_t;

typedef enum {
    START   = 0,    // 启动
    STOP    = 1,    // 停机
    RUN     = 2     // 运行
} motor_state_t;

typedef struct{
    motor_dir_t tMotorDirection;        // 电机旋转方向
    motor_state_t tMotorState;          // 电机状态
    uint16_t tDuty;                     // 速度占空比：0~100  为100是占空比为100%
    uint16_t tSpeed;                    // 电机转速
    uint8_t chHallValue;                // 霍尔状态
    struct{
        uint8_t chStartCount;           // 启动计数
        uint16_t hwTimeCount;           // 堵转超时溢出计数
    }tCount;
}User_TypeDef_t;

/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
volatile User_TypeDef_t g_tMotor = {
    CW,
    START,
    40,
    0,
    1,
    0,
    0
};
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
extern void Bldc_Phase_Chaneg(uint8_t step);
extern void Hall_Exti_Callback(void);
extern void Systick_Callback(void);
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{
    Systick_Init();
    
    Led_Init();
    Hall_Init();
    Bldc_Pwm_Init();
    Gate_Driver_Init();

    /* 无限循环 */
    while(1) {
        switch(g_tMotor.tMotorState) {
            case START:         // 电机启动
                if(++g_tMotor.tCount.chStartCount > 20){    //电机启动失败
                    g_tMotor.tMotorState = STOP;
                    g_tMotor.tCount.chStartCount = 0;
                }
                SET_DUTY_U(BLDC_TIM_PERIOD * g_tMotor.tDuty / 100);
                SET_DUTY_V(BLDC_TIM_PERIOD * g_tMotor.tDuty / 100);
                SET_DUTY_W(BLDC_TIM_PERIOD * g_tMotor.tDuty / 100);
                GATE_DRIVER_ENABLE();
                Hall_Exti_Callback();
                Systick_Delay(5);
                break;
            case RUN:         // 电机运行
                if (0){//some err
                    g_tMotor.tMotorState = STOP;
                }
                Hall_Exti_Callback();
                break;
            case STOP:         // 停机
                GATE_DRIVER_DISABLE();
                BLDC_UH_DISABLE();
                BLDC_UL_DISABLE();
                BLDC_VH_DISABLE();
                BLDC_VL_DISABLE();
                BLDC_WH_DISABLE();
                BLDC_WL_DISABLE();
                break;
        }
    }
}

/**
  * 函数功能: 无刷驱动换相
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void Bldc_Phase_Chaneg(uint8_t step)
{
    switch(step) {
        case 1: //U+ W-
            BLDC_UL_DISABLE();
            BLDC_UH_ENABLE();
            BLDC_VL_DISABLE();;
            BLDC_VH_DISABLE();
            BLDC_WH_DISABLE();
            BLDC_WL_ENABLE();
            break;
        case 3: //V+ W-
            BLDC_UL_DISABLE();
            BLDC_UH_DISABLE();
            BLDC_VL_DISABLE();
            BLDC_VH_ENABLE();
            BLDC_WH_DISABLE();
            BLDC_WL_ENABLE();
            break;
        case 2: //V+ U-
            BLDC_UH_DISABLE();
            BLDC_UL_ENABLE();
            BLDC_VL_DISABLE();
            BLDC_VH_ENABLE();
            BLDC_WL_DISABLE();
            BLDC_WH_DISABLE();
            break;
        case 6: //W+ U-
            BLDC_UH_DISABLE();
            BLDC_UL_ENABLE();
            BLDC_VL_DISABLE();
            BLDC_VH_DISABLE();
            BLDC_WL_DISABLE();
            BLDC_WH_ENABLE();
            break;
        case 4: //W+ V-
            BLDC_UH_DISABLE();
            BLDC_UL_DISABLE();
            BLDC_VH_DISABLE();
            BLDC_VL_ENABLE();
            BLDC_WL_DISABLE();
            BLDC_WH_ENABLE();
            break;
        case 5: //U+ V-
            BLDC_UL_DISABLE();
            BLDC_UH_ENABLE();
            BLDC_VH_DISABLE();
            BLDC_VL_ENABLE();
            BLDC_WH_DISABLE();
            BLDC_WL_DISABLE();
            break;
        default:
            BLDC_UH_DISABLE();
            BLDC_UL_DISABLE();
            BLDC_VH_DISABLE();
            BLDC_VL_DISABLE();
            BLDC_WH_DISABLE();
            BLDC_WL_DISABLE();
            break;
    }
}

void Systick_Callback(void)
{
    if(RUN == g_tMotor.tMotorState) {
        g_tMotor.tCount.hwTimeCount ++;
        if(g_tMotor.tCount.hwTimeCount > 2000) { // 2s超时，电机卡住不运转超过2s时间
            GATE_DRIVER_DISABLE();
            BLDC_UH_DISABLE();
            BLDC_UL_DISABLE();
            BLDC_VH_DISABLE();
            BLDC_VL_DISABLE();
            BLDC_WH_DISABLE();
            BLDC_WL_DISABLE();
            g_tMotor.tMotorState = STOP;
            g_tMotor.tCount.hwTimeCount = 0;
        }
    }
}

void Hall_Exti_Callback(void)
{
    volatile uint8_t chStep = 0;
//    chStep = (uint8_t)((HALL_PORT_U->IDR >> HALL_PINSOURCE_U) |
//                       (HALL_PORT_V->IDR >> (HALL_PINSOURCE_V - 1)) |
//                       (HALL_PORT_W->IDR >> (HALL_PINSOURCE_W - 2))) & 0x07;
    if(g_tMotor.tMotorState == STOP) {
        return;
    }
    if((HALL_PORT_U ->IDR & HALL_PIN_U) != (uint32_t)Bit_RESET) {
        chStep |= 0x01;
    }
    if((HALL_PORT_V ->IDR & HALL_PIN_V) != (uint32_t)Bit_RESET) {
        chStep |= 0x02;
    }
    if((HALL_PORT_W ->IDR & HALL_PIN_W) != (uint32_t)Bit_RESET) {
        chStep |= 0x04;
    }
    if (g_tMotor.chHallValue != chStep) {   //判断是否换向
        g_tMotor.tMotorState = RUN;
        g_tMotor.chHallValue = chStep;
    }

    if(g_tMotor.tMotorDirection == CCW) {   // 方向判断
        chStep =  7 - chStep;
    }
    Bldc_Phase_Chaneg(chStep);    //驱动换相
    g_tMotor.tCount.hwTimeCount = 0;
}
