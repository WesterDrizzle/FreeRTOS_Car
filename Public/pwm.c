#include "pwm.h"
#include "system.h"
#include "FreeRTOS.h"
#include "task.h"

// 1kz
uint16_t pwm_period = 0;
uint16_t pwm_freqency = 0;
uint16_t pwm_ccr = 0;
bool timer_inited = 0;
void tim4_pwm_init(uint16_t freq)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    if (freq == 0)
    {
        return;
    }
    else if (1000000 / freq > 65536)
    {
        return;
    }
    timer_inited = true;
    pwm_period = 1000000 / freq;
    pwm_freqency = freq;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // 32Mhz
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // 36Mhz/72 = 500Khz => 1/500kHz = 2us T

    TIM_TimeBaseStructure.TIM_Period = pwm_period; // T = 1us * 100 = 0.1ms = 10Khz
    TIM_TimeBaseStructure.TIM_Prescaler = 71;      // f =  72MHZ/(71+1) = 1Mhz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM4, ENABLE);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse = pwm_ccr; // CCR val<CCR = 0
    TIM_OCInitStructure.TIM_OutputState = ENABLE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
}
/**
 * @param duty: (0-100)%
 * @param duty: (0-100)
 * @brief duty = ccr/period;
 */
void pwm_set_duty(enum pwm_port port, uint8_t duty)
{
    if (!timer_inited)
        return;
    if (duty > pwm_period)
    {
        duty = pwm_period;
    }
    pwm_ccr = pwm_period * duty / 100;
    switch (port)
    {
    case PWM_PORT_1:
        TIM_SetCompare3(TIM4, pwm_ccr);
        break;
    case PWM_PORT_2:
        TIM_SetCompare4(TIM4, pwm_ccr);
        break;
    case PWM_PORT_BOTH:
        TIM_SetCompare3(TIM4, pwm_ccr);
        TIM_SetCompare4(TIM4, pwm_ccr);
        break;
    default:
        break;
    }
}
