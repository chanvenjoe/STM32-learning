/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.

  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "dma.h"
#include "sw_spi.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BLDC.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//MADC_Structure adc_buffer;
#define Phase_delay 5000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//uint8_t tx_buff[];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t adc_buf[CH_NUM]={0};
MADC_Structure adc_val = {1,1,1,1,1,1,1,1,1,1};
HX711_Structure weight_par = {0,0,0,0,0};
TimeFlagStruct printflag = {0};
PID_ParameterStruct PID_Parameters = {0.1, 0.0002, 0.2}; //PID adjustment, ID = 0, increase P till the curve shaking

/* USER CODE END PV */

uint8_t txbuff[] = "hellow worlds \r\n";

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void printf_DMA(const char *format,...);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
//  MX_SPI1_Init();
  MX_USART1_UART_Init();
  HAL_UART_Receive_IT(&huart1, &rxdata, sizeof(rxdata));

  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM14_Init();
  MX_TIM15_Init();
  MX_TIM16_Init();

  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim14);
  HAL_TIM_Base_Start_IT(&htim15);
  HAL_TIM_Base_Start_IT(&htim16);

	if(HAL_ADC_Start_DMA(&hadc, (uint32_t*)adc_buf, sizeof(adc_buf)/2)!=HAL_OK)//Remember that the length of DMA is half world and size of return bytes:that is double of the data transmited so the array overfllow!
	{
		Error_Handler(); //This function also enable the interruption
	}

  /* USER CODE BEGIN 2 */
	HAL_TIM_OC_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1, AH);
	HAL_TIM_PWM_Start(&htim1, BH);
	HAL_TIM_PWM_Start(&htim1, CH);
	HAL_TIMEx_PWMN_Start(&htim1, AH);
	HAL_TIMEx_PWMN_Start(&htim1, BH);
	HAL_TIMEx_PWMN_Start(&htim1, CH);
	HX711_Calibration(&weight_par);
	printflag.PID_Set = FALSE;



  /* USER CODE END 2 */

//  HAL_TIM_OC_Start(&htim1,TIM_CHANNEL_4);


  /*******initial position reset**************/
//   BLDC_Start_Up();

  /****************************************/



  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//Kalman  filter
	if(weight_par.calibration_flag)
	{
		printf_DMA("%d, %d, %0.1f\r\n",weight_par.gramAvgval, (int)htim1.Instance->CCR2, (float)(adc_val.ia-1640)*0.015);//Vofa+ chart   3.3/4095/35/0.001 = 0.023
//		printf_DMA("0x31 0x14%d\n\r", weight_par.gramAvgval);
//		printf_DMA("PWM%d\r\n", (int)htim1.Instance->CCR2);

//		printf_DMA("gram: %d\r\n", weight_par.gramAvgval );
		//		printf_DMA("%d\r\n", adc_val.commutation_delay);
//		HAL_UART_Transmit_DMA(&huart1, (uint8_t *) txbuff, sizeof(txbuff)+1); // if it is (uint8_t) * txbuff, then it will be force the txbuff to char, not change the addr to char
	}
	if(1100 <= printflag.TimeCNT)
	{
		printf_DMA("VBat%0.2fV\n",	adc_val.vbat*(Vrefint*4095/adc_val.vref_data)/4095/VBAT_FACTOR);
	}
	if(printflag.OCP_flag)//||weight_par.sens_err_flag)
	{
		printf_DMA("OCP\n");
		error_code();
	}
	if(printflag.sensor_err)
	{
		printf_DMA("Data un-ready\n");
	}
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__                                    //串口重定向
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
//	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ch, sizeof(ch));

    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

void printf_DMA(const char *format, ...)
{
	uint32_t length;
	va_list args;
//	uint8_t temp = 0;

	va_start(args, format);
	length = vsnprintf((char*)txbuf, sizeof(txbuf)+1, (char*)format, args);
	va_end(args);

	HAL_UART_Transmit_DMA(&huart1, txbuf, length);
	while(!__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC));


}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static signed int dc_pwm, pid_pwm;
	signed int temp;
	if(htim == &htim3) // Get pull force value
	{
		if(1 == weight_par.calibration_flag)
		{
			Get_weight(&weight_par);
			weight_par.cnt= weight_par.cnt >= 5? 0:weight_par.cnt+1;//Get average value of 5
			weight_par.gramAvg[weight_par.cnt] = weight_par.gram;
			weight_par.gramAvgval = (weight_par.gramAvg[0] + weight_par.gramAvg[1] + weight_par.gramAvg[2] + weight_par.gramAvg[3] + weight_par.gramAvg[4])/5;
//			FORCESAPTIME;
		}
	}
	else if(htim == &htim6)// PWM step 1 for accurate acceleration, change the timer to modify the ramp time
	{
		if((weight_par.gramAvgval<LOWER_LIMMIT&&weight_par.gramAvgval>=0)||weight_par.sens_err_flag==TRUE)//LOWER_LIMMIT)// when release the handle, turn off all
		{
/*			CLOSE_PWM;
			pid_pwm = 0;//if not, the PID_PWM will always be the same value and dc_pwm never be 0
			dc_pwm = pid_pwm; //to set the PWM to 0 immediately
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dc_pwm);*/
			if(printflag.OCP_flag == FALSE)
				IND_LED_OFF;//Turn off LED
			weight_par.eps_flag = FALSE;
			printflag.Motor_short_flag = TRUE;
		}
		else
		{
			printflag.Motor_short_flag = FALSE;// if keep pulling, stop shorting motor, if = true, H/L MOS will open together
			temp = Incremental_PID(&weight_par, PULL_FORCE_THR, &PID_Parameters);//Get PID PWM
			if((pid_pwm+temp) > MAX_PWM)
				pid_pwm = MAX_PWM;
			else
				pid_pwm = (signed int)(temp + pid_pwm)<=0	? 0		: 	temp + pid_pwm;

			dc_pwm = (0<(pid_pwm-dc_pwm)) ? dc_pwm + 1 : dc_pwm - 1;

			if(dc_pwm>10&&printflag.OCP_flag!=TRUE) //When PWM>10%, start to drive
			{
				//BHALONBLOFF; //Drive MOTOR, open lower bridge first
				/*__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dc_pwm);
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dc_pwm);
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dc_pwm);*/
				if(0 == weight_par.eps_flag)
				{
					IND_LED_ON;// Turn on LED indicator
					weight_par.eps_flag = TRUE;
				}
			}
		}
	}
	else if(htim == &htim14)//10ms enter for E-wagon protection counting
	{
		printflag.TimeCNT = printflag.TimeCNT>1105? 0:printflag.TimeCNT+1;
		if(adc_val.commutation_timeout >1000)//if 100ms no phase switching, 100us++
		{
			adc_val.commutation_timeout = 0;
			adc_val.commutation_delay 	= 0;
//			CLOSE_ALL;
		}
	/*	if(1==printflag.Motor_short_flag || 1==HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) //Brake counting
		{
			static char count; static char short_rls_cnt = 0;
			count+=1;
			if(count>150)//count 500ms to short the motor
			{
				CLOSE_PWM;
				pid_pwm = 0;//if not, the PID_PWM will always be the same value and dc_pwm never be 0
				dc_pwm = pid_pwm; //to set the PWM to 0 immediately
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dc_pwm);
				BRAKE;
				count = 0;
				printflag.Motor_short_flag = 0;
			}
		}*/
		if(adc_val.ia>=2240)// OCP 2973:20A 2240:9A 3640:30A Based on 0.015(53.7 times)
		{
			static char ocp_cnt = 0;
			ocp_cnt++;
			if(ocp_cnt>50)
			{
				CLOSE_PWM;
				BRAKE;
				BRKRLS;
				ocp_cnt = 0;
				printflag.OCP_flag = TRUE;

			}
		}
		if(printflag.OCP_flag == TRUE)
		{
			static int ocp_clr = 1000;
			ocp_clr--;
			if(ocp_clr==0)
				printflag.OCP_flag = FALSE;
		}

	}
	else if(htim == &htim15)//1us
	{

	}
	else if(htim == &htim16)//1us interval for time counting
	{

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//every byte transmit complete, enter this function
{
	if(huart == &huart1)
	{
		extern uint8_t cnt;
		rxbuf[cnt] = rxdata;
		const char cat[] = "0X13\r\n"; //APP(A:1) to Controller(C:3)
		cnt=cnt==RX_BUF_NUM-1?1:cnt+1;
		float PIDV;

		char a[6];

		if(rxdata == 'e')//'\n')
		{
//			printf("sting:%s\r\n", rxbuf);

			for(int t=cnt; t<RX_BUF_NUM; t++)
				rxbuf[t]=0;
			cnt = 0;

			if(0 == strcmp((char*) rxbuf,"0x00"))
			{
				HAL_TIM_Base_Stop_IT(&htim6);
			}

			else if(0 == strcmp((char*) rxbuf, "0x01"))
			{
				HAL_TIM_Base_Start_IT(&htim6);
			}
			else if(0 == strcmp((char*)rxbuf, "0x02"))
			{
				BT_PWM_handle(TURE);
			}
			else if(0 == strcmp(cat, "0x03"))
			{
				BT_PWM_handle(FALSE);
			}
			else if(0 == strcmp(cat, "0x04"))
			{
				htim1.Instance->CCR1++;
				htim1.Instance->CCR2++;
				htim1.Instance->CCR3++;
			}
			else if(0 == strcmp(cat, "0x05"))
			{
				TIM14->ARR--;
			}
			else if(0 == strcmp(cat, "0x06"))
			{

			}
			else if(0 == strcmp((char*) rxbuf, MotorOn))
			{
				printflag.Motor_On = TRUE;
			}
			else if(0 == strcmp((char*) rxbuf, MotorOff)) // 0X13MOff
			{
				printflag.Motor_Off = TRUE;

			}
		}
		if(rxdata == 'P') // This code comsumes 3KB?
		{
			for(int t=cnt; t<RX_BUF_NUM; t++)
				rxbuf[t]=0;
			cnt = 0;
			sscanf(rxbuf, "%[^P]", a);
			PIDV = atof(a);
			PID_Parameters.Kp = PIDV;
//			printflag.PID_Set = TRUE;
		}
		else if(rxdata == 'I')
		{
			for(int t=cnt; t<RX_BUF_NUM; t++)
				rxbuf[t]=0;
			cnt = 0;
			sscanf(rxbuf, "%[^I]", a);
			PIDV = atof(a);
			PID_Parameters.Ki = PIDV;
//			printflag.PID_Set = TRUE;
		}
		else if(rxdata == 'D'|| rxdata == 'E')
		{
			for(int t=cnt; t<RX_BUF_NUM; t++)
				rxbuf[t]=0;
			cnt = 0;
			if(rxdata == 'D')
			{
				sscanf(rxbuf, "%[^D]", a);
				PIDV = atof(a);
				PID_Parameters.Kd = PIDV;
//				printf_DMA("0X31 Set ok\n");
			}
			else
			{
				printflag.PID_Set = TRUE;
				printf_DMA("0X31 PID align ok\n");

			}
		}
		HAL_UART_Receive_IT(&huart1, &rxdata, sizeof(rxdata));
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)// Using tim15 to get a 88us between each trigger 50us as TIM1 cycle, 14MHz ADC(12.5+55.5 cycles) consume 4.37ms to complete conversion
{									  // The ADC sample time is for all channel, the DMA
	My_ADC_getvalue(adc_buf, &adc_val);
//	BLDC_Phase_switching(&adc_val);
}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	if(GPIO_PIN == GPIO_PIN_2)
	{
	//		weight_par.pw_flag = ~ weight_par.pw_flag;

	}
	else if(GPIO_PIN == GPIO_PIN_4)
	{
		HAL_TIM_Base_Stop_IT(&htim6);
	}
	else if(GPIO_PIN == GPIO_PIN_5)
	{
		HAL_TIM_Base_Start_IT(&htim6);
	}
}


