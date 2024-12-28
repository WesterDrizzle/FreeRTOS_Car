#ifndef _motor_module_h_
#define _motor_module_h_
/********************************include****************************************/

#include "system.h"

/********************************define***************************************/

enum MOTOR_DIRECTION
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP,
    NotMatter,
};
enum MOTOR
{
    LEFT_MOTOR,
    RIGHT_MOTOR,
    BOTH_MOTOR
};
/*********************************function****************************************/

void motor_module_init(void);
void motor_module_set_direction(enum MOTOR_DIRECTION direction);
void motor_module_set_speed(enum MOTOR motor, uint8_t speed);

#endif
