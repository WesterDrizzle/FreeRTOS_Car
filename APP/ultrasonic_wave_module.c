#include "ultrasonic_wave_module.h"
#include "SysTick.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#define TRIG(bit) GPIO_WriteBit(GPIOA, GPIO_Pin_11, bit)
#define ECHO PAin(12)
void timer2_start(void);
void timer2_stop(void);
void timer2_init(void);
void ultrasonic_wave_module_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    timer2_init();
}

static uint32_t timer_cnt = 0;
static uint16_t timer_overflow = 0;
// s=340m/s× t / 2
// return timer_cnt /us
uint32_t ultrasonic_wave_module_get_val(void)
{
    taskENTER_CRITICAL();
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, 1);
	delay_us(20);
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, 0);

    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
    {
    };
    timer2_start();
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1)
    {
       
    };
    timer2_stop();
    timer_cnt = timer_cnt + timer_overflow * 1000;
    taskEXIT_CRITICAL();

    // 最大100ms
    // 则100000
    return timer_cnt;
}
// s=340m/s× t / 2
// return  mm
uint16_t ultrasonic_wave_module_get_distance(void)
{
    uint32_t cnt = ultrasonic_wave_module_get_val();
    uint16_t distance = 340000 * cnt / 1000000/2; // mm/s* us/cnt
    return distance;
}
// 1ms一次中断
void timer2_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//启用TIM3时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseStructure.TIM_Period = 999;  // T = 1us * 100 = 0.1ms = 10Khz
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // f =  72MHZ/(71+1) = 1Mhz //1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        timer_overflow++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void timer2_start(void)
{
    timer_cnt = 0;
    timer_overflow = 0;
    TIM_Cmd(TIM2, ENABLE);
}

void timer2_stop(void)
{
    timer_cnt = TIM_GetCounter(TIM2);
    TIM_Cmd(TIM2, DISABLE);
    TIM_SetCounter(TIM2, 0);
}
