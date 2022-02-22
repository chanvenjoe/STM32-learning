#define u8 unsigned char
#define u16 unsigned int
void ADC_Init(void);
u16 Get_HallValue(void);
u16 Get_CurrentValue(void);
u8  Get_Speedvalue(void);
u8  Get_RPx100Msvalue(u8 x);
void PWM_Init(void);
void PWM_Setting(u8 n, u8 relay);
void GPIO_Init();
int Incremental_P(u8 Cbat, u8 CC_value);
void Relay_On(u8 On_FB);
void Relay_Off(void);

