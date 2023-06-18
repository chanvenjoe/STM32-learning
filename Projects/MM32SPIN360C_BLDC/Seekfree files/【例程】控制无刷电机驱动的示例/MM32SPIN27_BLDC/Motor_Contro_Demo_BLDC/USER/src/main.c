/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				main
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "headfile.h"

// *************************** 例程硬件连接说明 ***************************
//
// 如果使用的并不是逐飞科技 MM32SPIN27PS 主板 请按照下述方式进行接线
//      模块引脚    单片机引脚
//      PWM         查看 main.c 中 PWM_CH1  宏定义 默认 A11
//      DIR         查看 main.c 中 DIR_CH1  宏定义 默认 A12
//      SPDOUT      查看 main.c 中 ENCODER1_PLUS 宏定义 默认 A1
//      DIROUT      查看 main.c 中 ENCODER1_DIR  宏定义 默认 A8
//      GND         电源地
//
// 如果需要修改使用的引脚 在main.c中修改对应引脚
// 确认连接无误即可上电 请务必使用电池供电 并确保电池电量充足
//
// *************************** 例程硬件连接说明 ***************************

// *************************** 例程说明 ***************************
// 
// 测试需要准备逐飞科技MM32SPIIN27PS核心板一块
// 
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project  clean  等待下方进度条走完
//
// *************************** 例程说明 ***************************


// **************************** 宏定义 ****************************
#define MAX_DUTY			10				//最大占空比输出限制
#define PWM_TIM				TIM_2			//PWM定时器
#define PWM_CH1				TIM_2_CH1_A11	//PWM输出端口
#define DIR_CH1				A12				//电机方向输出端口

#define ENCODER1_TIM		TIM_1			//编码器定时器
#define ENCODER1_PLUS		TIM_1_ENC2_A01	//编码器计数端口
#define ENCODER1_DIR		A8				//编码器方向采值端口
// **************************** 宏定义 ****************************



// **************************** 变量定义 ****************************
int8 duty = 0;		//当前占空比
bool dir = true;	//计数方向
int16 encoder=0;	//编码器值
// **************************** 变量定义 ****************************



// **************************** 代码区域 ****************************
int main(void)
{
	board_init(true);																// 初始化 debug 输出串口

	//此处编写用户代码(例如：外设初始化代码等)
	
	pwm_init(TIM_2, TIM_2_CH1_A11, 1000, 0);											// PWM 通道1 初始化频率1KHz 占空比初始为0
//	pwm_init(TIM_3, TIM_3_CH1_B04, 1000, PWM_DUTY_MAX/2);
//	pwm_init(TIM_2, TIM_2_CH1_A11, 1000, 0);
	gpio_init(DIR_CH1, GPO, 0, GPO_PUSH_PUL);										//初始化电机方向输出引脚			

	tim_counter_init(TIM_1, TIM_1_ENC2_A01);										//初始化编码器采值引脚及定时器
	gpio_init(ENCODER1_DIR, GPI, 0, GPI_PULL_UP);									//初始化编码器方向端口
	//此处编写用户代码(例如：外设初始化代码等)

	while(1)
	{
		//此处编写需要循环执行的代码
		if(duty >= 0)														// 正转
		{
			pwm_duty_updata(TIM_2, TIM_2_CH1_A11, duty*(PWM_DUTY_MAX/100));		// 计算占空比
//			pwm_duty_updata(TIM_3, TIM_3_CH1_B04, duty*(PWM_DUTY_MAX/2));
			gpio_set(DIR_CH1,1);
		}
		else																// 反转
		{
			pwm_duty_updata(PWM_TIM, PWM_CH1, -duty*(PWM_DUTY_MAX/100));									
			gpio_set(DIR_CH1,0);
		}
		if(dir)																// 根据方向判断计数方向 本例程仅作参考
		{
			duty++;															// 正向计数
			if(duty >= MAX_DUTY)											// 达到最大值
				dir = false;												// 变更计数方向
		}
		else
		{
			duty--;															// 反向计数
			if(duty <= -MAX_DUTY)											// 达到最小值
				dir = true;													// 变更计数方向
		}
		
		encoder = tim_counter_get_count(TIM_1);								// 采集对应编码器数据
		tim_counter_rst(TIM_1);												// 清除对应计数
		if(gpio_get(ENCODER1_DIR))encoder=-encoder;
		
		printf("encoder counter: %d \r\n", encoder);				// 串口输出采集的数据
		
		systick_delay_ms(100);
		//此处编写需要循环执行的代码
	}
}
// **************************** 代码区域 ****************************
