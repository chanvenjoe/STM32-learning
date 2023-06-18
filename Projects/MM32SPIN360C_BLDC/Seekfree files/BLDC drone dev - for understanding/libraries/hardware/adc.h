#ifndef __ADC_H_
#define __ADC_H_


#include "common.h"

#define CHANNEL_DISABLE				    ((uint32_t)0xFFFF0000)
/*
ADC channel array
			ADC1      ADC2
CH			
 0		PA0				PA10
 1		PA1				PB15
 2		PA2				PB14
 3		PA3				PB13
 4		PA4				PA4
 5		PA5				PA5
 6		PA6				PA6
 7		PA7				PA7
 8		PB0				PB0
 9		PB1				PB1
 10		PB2				PB2
 11		PB10			PB10
*/
typedef struct
{ 
	uint16 voltage_bus;
	uint16 current_bus;
    uint16 current_bus_filter;
    uint16 current_a;
    uint16 current_b;
}adc_struct; 	

extern adc_struct adc_information; 			

void adc_init(void);
void adc_read(void);

#endif
