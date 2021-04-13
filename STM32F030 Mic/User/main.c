 #include "stm32f0xx.h"
 #include "ADC.h"
 
 int main()
 {
	// LED_init();
	 uint16_t led_pwmval = 0;
	 uint8_t dir = 1;
	// NVIC_priorityGroupConfig(NVIC_PriorityGroup_2);
	 Timer(500-1,84-1);
	 
	 while(1)
	 {
		 if(dir) led_pwmval++;
		 else led_pwmval--;
		 if(led_pwmval>300) dir = 0;
		 if(led_pwmval==0)dir = 1;

		 TIM_SetCompare1(TIM14, led_pwmval);
	 }
 }
