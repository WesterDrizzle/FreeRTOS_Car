#include "tracking_module.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

void tracking_module_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t tracking_module_get_data()
{
    uint16_t bitVal = 0;
    tracking_module_data data;
    data.value = 0;
    data.bits.left = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
    data.bits.middle = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
    data.bits.right = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
    return data.value;
}
