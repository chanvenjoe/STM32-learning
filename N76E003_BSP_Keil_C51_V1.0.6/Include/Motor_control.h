#define u8 unsigned char
#define u16 unsigned int
void ADC_Init(void);
u16 Get_HallValue(void);
u16 Get_CurrentValue(void);
u8  Get_Speedvalue(void);
void PWM_Init(void);
void PWM_Setting(u16 n);
void GPIO_Init();
int Incremental_P(u8 Cbat, u8 CC_value);
void Relay_On(u8 On_FB);
void Relay_Off(u8 Off_FB);
