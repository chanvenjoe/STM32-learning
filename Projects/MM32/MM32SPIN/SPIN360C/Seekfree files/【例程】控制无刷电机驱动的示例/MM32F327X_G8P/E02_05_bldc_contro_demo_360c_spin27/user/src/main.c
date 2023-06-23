/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             main
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F327X_G8P
* @Taobao           https://seekfree.taobao.com/
* @date             2022-04-11
********************************************************************************************************************/

#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完


// *************************** 例程硬件连接说明 ***************************
// 如果使用的并不是逐飞科技 MM32F3277 主板 请按照下述方式进行接线
//      模块引脚    单片机引脚
//      PWM         查看 main.c 中 PWM_CH1  宏定义 默认 A0
//      DIR         查看 main.c 中 DIR_CH1  宏定义 默认 A1
//      SPDOUT      查看 main.c 中 ENCODER1_PLUS 宏定义 默认 B4
//      DIROUT      查看 main.c 中 ENCODER1_DIR  宏定义 默认 B5
//      GND         电源地


// *************************** 例程测试说明 ***************************
// 1.这个例程需要连接电磁电容检测部分 并且要接好运放模块
// 2.接好后下载这个模块还需要一个赛道信号源接上电磁线测试
// 3.接好硬件信号源接好后 本例程会通过串口输出各个电容电感对的检测值
// 4.根据实际值调整对应的运放的滑变电阻调整放大倍数
// 5.输出信息应当如下：
//      Lx data is xxx.
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define MAX_DUTY            (30 )                                               // 最大占空比输出限制
#define PWM_CH1             (TIM2_PWM_CH1_A0)                                   // PWM输出端口
#define DIR_CH1             (A1 )                                               // 电机方向输出端口

#define ENCODER1_TIM        (TIM3_ENCOEDER)                                     // 编码器定时器
#define ENCODER1_PLUS       (TIM3_ENCOEDER_CH1_B4)                              // 编码器计数端口
#define ENCODER1_DIR        (TIM3_ENCOEDER_CH2_B5)                              // 编码器方向采值端口

int8 duty = 0;                                                                  // 当前占空比
bool dir = true;                                                                // 计数方向
int16 encoder=0;                                                                // 编码器值

int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

    // 此处编写用户代码 例如外设初始化代码等
    pwm_init(PWM_CH1, 1000, 0);                                                 // PWM 通道1 初始化频率1KHz 占空比初始为0
    gpio_init(DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          //初始化电机方向输出引脚           
    encoder_init_dir(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);                //初始化编码器采值引脚及定时器

    pit_init_ms(TIM8_PIT, 100);
    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码
        printf("bldc encoder counter: %d \r\n", encoder);                       // 串口输出采集的数据
        system_delay_ms(1000);
        // 此处编写需要循环执行的代码
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TIM8_PIT 的中断处理函数 这个函数将在 TIM8_PIT 对应的定时器中断调用 详见 isr.c
// @param       void
// @return      void
// Sample usage:                pit_hanlder();
//-------------------------------------------------------------------------------------------------------------------
void pit_hanlder (void)
{
    if(duty >= 0)                                                               // 正转
    {
        pwm_set_duty(PWM_CH1, duty * (PWM_DUTY_MAX / 100));                     // 计算占空比
        gpio_set(DIR_CH1, 1);
    }
    else                                                                        // 反转
    {
        pwm_set_duty(PWM_CH1, -duty * (PWM_DUTY_MAX / 100));     
        gpio_set(DIR_CH1, 0);
    }
    if(dir)                                                                     // 根据方向判断计数方向 本例程仅作参考
    {
        duty ++;                                                                // 正向计数
        if(duty >= MAX_DUTY)                                                    // 达到最大值
        dir = false;                                                            // 变更计数方向
    }
    else
    {
        duty --;                                                                // 反向计数
        if(duty <= -MAX_DUTY)                                                   // 达到最小值
        dir = true;                                                             // 变更计数方向
    }

    encoder = encoder_get_count(ENCODER1_TIM);                                  // 采集对应编码器数据
    encoder_clear_count(ENCODER1_TIM);                                          // 清除对应计数
}
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口
//      如果是使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
// 问题2：串口数据乱码
//      查看串口助手设置的波特率是否与程序设置一致，程序中 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义为 debug uart 使用的串口波特率
// 问题3：无刷电机无反应
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量实际PWM输出引脚是否有输出或者示波器查看波形
// 问题4：无刷电机转动但转速显示无速度
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量对应的无刷电机速度输出是否有信号
