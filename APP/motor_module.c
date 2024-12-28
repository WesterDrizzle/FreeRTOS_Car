#include "motor_module.h"
#include "pwm.h"
void motor_module_init(void)
{
    // Direction
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
    // Speed
    tim4_pwm_init(1000);
}
/**
 * @param speed 0-100
 */

void motor_module_set_speed(enum MOTOR motor, uint8_t speed)
{
    if (speed > 100)
    {
        speed = 100;
    }
    switch (motor)
    {
    case LEFT_MOTOR:
        pwm_set_duty(motor, speed);
        break;
    case RIGHT_MOTOR:
        pwm_set_duty(motor, speed);
    case BOTH_MOTOR:
        pwm_set_duty(motor, speed);
    default:
        break;
    }
}

void motor_module_set_direction(enum MOTOR_DIRECTION direction)
{
    switch (direction)
    {
    case FORWARD:
        PBout(12) = 0; // IN1
        PBout(13) = 1;
        PBout(14) = 0;
        PBout(15) = 1; // IN4
        break;
    case BACKWARD:
        PBout(12) = 1;
        PBout(13) = 0;
        PBout(14) = 1;
        PBout(15) = 0;
        break;
    case LEFT:
        PBout(12) = 0;
        PBout(13) = 1;
        PBout(14) = 1;
        PBout(15) = 0;
        break;
    case RIGHT:
        PBout(12) = 1;
        PBout(13) = 0;
        PBout(14) = 0;
        PBout(15) = 1;
        break;
    case STOP:
        PBout(12) = 1;
        PBout(13) = 1;
        PBout(14) = 1;
        PBout(15) = 1;
        break;
    case NotMatter:
        break;
    default:
        PBout(12) = 0;
        PBout(13) = 0;
        PBout(14) = 0;
        PBout(15) = 0;
        break;
    }
}