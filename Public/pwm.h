#ifndef _PWM_H_
#define _PWM_H_

/********************************include****************************************/
#include "system.h"
/********************************define***************************************/

enum pwm_port
{
    PWM_PORT_1 = 0,
    PWM_PORT_2,
    PWM_PORT_BOTH,
};
/*********************************function****************************************/
void tim4_pwm_init(uint16_t freq);
void pwm_set_duty(enum pwm_port port, uint8_t duty);

#endif
