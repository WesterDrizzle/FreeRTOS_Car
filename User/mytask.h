#ifndef MY_TASK_H
#define MY_TASK_H
#include "system.h"

#define start_task_priority 1
#define computer_serial_task_priority 5
#define mpu6050_task_priority 13
#define ultrasonic_wave_task_priority 12
#define k210_task_priority 8
#define wifi_task_priority 10
#define bluetooth_task_priority 6
#define tracking_task_priority 7
#define motor_task_priority 8

#define start_task_stack_size 512
#define computer_serial_task_stack_size 512
#define mpu6050_task_stack_size 0
#define ultrasonic_wave_task_stack_size 512
#define k210_task_stack_size 512
#define wifi_task_stack_size 0
#define bluetooth_task_stack_size 512
#define tracking_task_stack_size 512
#define motor_task_stack_size 512
//
#define total_task_stack_size (mpu6050_task_stack_size + ultrasonic_wave_task_stack_size + k210_task_stack_size + wifi_task_stack_size + bluetooth_task_stack_size + tracking_task_stack_size + motor_task_stack_size)

void start_task(void *Parameters);
void mpu6050_task(void *parameter);
void ultrasonic_wave_task(void *parameter);
void k210_task(void *parameter);
void wifi_task(void *parameter);
void bluetooth_task(void *parameter);
void tracking_task(void *parameter);
void motor_task(void *parameter);
void StartCarTask(void);

#endif
