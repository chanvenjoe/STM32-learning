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
  *
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BLDC.h"
#include <stdio.h>
#include <string.h>
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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t adc_buf[CH_NUM]={0};
MADC_Structure adc_val = {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0};
FlagsStructure MFlag = {0,0,0,1};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Delay_ms(uint32_t delay);
/* USER CODE BEGIN PFP */

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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM14_Init();
  MX_TIM15_Init();
  MX_USART1_UART_Init();
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim14);
  HAL_UART_Receive_IT(&huart1, &rxdata, sizeof(rxdata));
	if(HAL_ADC_Start_DMA(&hadc, (uint32_t*)adc_buf, sizeof(adc_buf)/2)!=HAL_OK)//Remember that the length of DMA is half world and size of return bytes:that is double of the data transmited so the array overfllow!
	{
	 Error_Handler(); //This function also enable the interruption
	}

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  HAL_TIM_OC_Start(&htim1,TIM_CHANNEL_4);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

  /*******initial position reset**************/
//   BLDC_Start_Up();

  /****************************************/
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(1)//adc_val.commutation_delay!=0)
	  {
//		printf("\r\n threshole:%d", adc_val.cross_zero_threshole);
		Delay_ms(100);
//		printf("TIM14 ARR:%d\r\n", (int)TIM14->ARR);
//		printf("\r\n BEMF now:%d", adc_val.bemf_now);
//		printf("\r\n BEMF next:%d", adc_val.bemf_next);
//		printf("\r\n STATUS:%d", adc_val.zero_across_flag);
//		printf("\r\n zero_ac_COUNT:%d", adc_val.zero_across_count);
//		printf("\r\n zero_across_thr:%d",adc_val.zero_across_threshole);
//		printf("\r\n Speed:%dms/round", adc_val.speed*POLOAR_PARIRS/1000);
		Delay_ms(100);
		printf("%0.2fV\n",	adc_val.vbat*(Vrefint*4095/adc_val.vref_data)/4095/VBAT_FACTOR);
		Delay_ms(100);
		printf("%dRPM\n", (int)(3600/(adc_val.commutation_delay*6/1000)));//3 pair
//		printf("\r\n BEMF A:%0.2f B:%0.2f C:%0.2f M:%0.2f",
//				adc_buf[0]*(Vrefint*4095/adc_val.vref_data)/819, adc_buf[1]*(Vrefint*4095/adc_val.vref_data)/819,
//				adc_buf[2]*(Vrefint*4095/adc_val.vref_data)/819, adc_buf[8]*(Vrefint*4095/adc_val.vref_data)/819);
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
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
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
	static char bldc_duty = 0;
	static int period1;
	if(htim == &htim6)//1s base
	{
	/*	if(period1==20&&bldc_duty<50)
		{
			bldc_duty+=1;
			BLDC_PWM_Handle(bldc_duty);
		}*/
		if(period1>45)//20)					//200ms initial and 4.5ms stable are for the large motor
		{								//100ms initial and 2ms		stable are for the Drone motor
			period1-=5;					//-5
			TIM14->ARR=period1;
		}
		MFlag.IsLEDOn = true;
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);

	}
	else if(htim == &htim14)//0.1ms base
	{
		if(adc_val.commutation_timeout >1000)//if 100ms no phase switching, 100us++
		{
			adc_val.commutation_timeout = 0;
			adc_val.commutation_delay 	= 0;
			adc_val.zero_across_flag	= TIMEOUT;
			MFlag.IsPWMOutput			= false;
			CLOSE_ALL;
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)==0||MFlag.IsAPPMotorOn==false)//button reset, stop outputing
		{
			bldc_duty = 50;
			CLOSE_ALL;
			BLDC_PWM_Handle(bldc_duty);
			period1=200;
			MFlag.IsPWMOutput			= false;
			MFlag.IsSwitchOn			= false;
			adc_val.zero_across_flag	= START_UP;
			adc_val.commutation_timeout = 0;
			adc_val.commutation_delay 	= 0;
			adc_val.zero_across_count 	= 0;
			adc_val.speed 				= 0;
		}
		else if((MFlag.IsAPPMotorOn&&HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)==1) && adc_val.zero_across_flag==START_UP)
		{
			MFlag.IsPWMOutput			= true;
			MFlag.IsSwitchOn			= true;
			BLDC_Driving_test(&adc_val);
/*			if(period1>20)					//200ms initial and 4.5ms stable are for the large motor
			{
				period1-=1;
				TIM14->ARR=period1;
			}*/
		}


	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//every byte transmit complete, enter this function
{
	if(huart == &huart1)
	{
		extern uint8_t cnt;
		rxbuf[cnt] = rxdata;
		const char cat[] = "0X13\r\n"; //APP(A:1) to Controller(C:3)
		cnt=cnt==RX_BUF_NUM?1:cnt+1;

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
				adc_val.zero_across_flag = BEMF_DETECTION;
			}
			else if(0 == strcmp((char*) rxbuf, MotorOn))
			{
				MFlag.IsAPPMotorOn = true;
				printf("Start command correct\n");
			}
			else if(0 == strcmp((char*) rxbuf, MotorOff)) // 0X13MOff
			{
				printf("Stop command correct\n");
				MFlag.IsAPPMotorOn = false;
				MFlag.IsPWMOutput	= false;
			}

		}
		HAL_UART_Receive_IT(&huart1, &rxdata, sizeof(rxdata));
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)// Using tim15 to get a 88us between each trigger 50us as TIM1 cycle, 14MHz ADC(12.5+55.5 cycles) consume 4.37us to complete conversion
{											  // The ADC sample time is for all channel, the DMA
	adc_val = My_ADC_getvalue(adc_buf, &adc_val);
	adc_val.commutation_delay = (adc_val.speed/10);// 10: since the speed is 5 phase switching time
	if(adc_val.zero_across_count>30000)//adc_val.zero_across_flag == BEMF_DETECTION)//make sure the speed is stable
	{
//		adc_val.zero_across_flag = BEMF_DETECTION;
//		BLDC_Phase_switching(&adc_val);//The real speed should be read from this function
	}

}

void Delay_ms(uint32_t delay)
{
	uint32_t tick_start;
	tick_start = HAL_GetTick();
	while((HAL_GetTick()-tick_start)<delay)
		__NOP();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)//Needs external circuit to pull up/down
{

}

/* Count the interval
  	static char temp = 0;
	temp = ~temp;
	if(temp == 0)
	{
	__HAL_TIM_SET_COUNTER(&htim15, 0);//the auto reload is set to 65535 1us time base
	HAL_TIM_Base_Start(&htim15);
	}
	else
	{
		adc_val.speed = __HAL_TIM_GET_COUNTER(&htim15);
		HAL_TIM_Base_Stop(&htim15);
	}
 */

