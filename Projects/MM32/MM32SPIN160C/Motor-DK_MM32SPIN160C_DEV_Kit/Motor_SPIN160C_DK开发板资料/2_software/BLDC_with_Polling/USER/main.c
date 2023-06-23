#include "sys.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
typedef enum {
    CW      = 0,    // ˳ʱ�ӷ���
    CCW     = 1     // ��ʱ�뷽��
} motor_dir_t;

typedef enum {
    START   = 0,    // ����
    STOP    = 1,    // ͣ��
    RUN     = 2     // ����
} motor_state_t;

typedef struct{
    motor_dir_t tMotorDirection;        // �����ת����
    motor_state_t tMotorState;          // ���״̬
    uint16_t tDuty;                     // �ٶ�ռ�ձȣ�0~100  Ϊ100��ռ�ձ�Ϊ100%
    uint16_t tSpeed;                    // ���ת��
    uint8_t chHallValue;                // ����״̬
    struct{
        uint8_t chStartCount;           // ��������
        uint16_t hwTimeCount;           // ��ת��ʱ�������
    }tCount;
}User_TypeDef_t;

/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
volatile User_TypeDef_t g_tMotor = {
    CW,
    START,
    40,
    0,
    1,
    0,
    0
};
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
extern void Bldc_Phase_Chaneg(uint8_t step);
extern void Hall_Exti_Callback(void);
extern void Systick_Callback(void);
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{
    Systick_Init();
    
    Led_Init();
    Hall_Init();
    Bldc_Pwm_Init();
    Gate_Driver_Init();

    /* ����ѭ�� */
    while(1) {
        switch(g_tMotor.tMotorState) {
            case START:         // �������
                if(++g_tMotor.tCount.chStartCount > 20){    //�������ʧ��
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
            case RUN:         // �������
                if (0){//some err
                    g_tMotor.tMotorState = STOP;
                }
                Hall_Exti_Callback();
                break;
            case STOP:         // ͣ��
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
  * ��������: ��ˢ��������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
        if(g_tMotor.tCount.hwTimeCount > 2000) { // 2s��ʱ�������ס����ת����2sʱ��
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
    if (g_tMotor.chHallValue != chStep) {   //�ж��Ƿ���
        g_tMotor.tMotorState = RUN;
        g_tMotor.chHallValue = chStep;
    }

    if(g_tMotor.tMotorDirection == CCW) {   // �����ж�
        chStep =  7 - chStep;
    }
    Bldc_Phase_Chaneg(chStep);    //��������
    g_tMotor.tCount.hwTimeCount = 0;
}
