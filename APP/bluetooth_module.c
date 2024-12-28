#include "bluetooth_module.h"
#include "usart.h"
#include "string.h"
#include "motor_module.h"
#include <stdarg.h>
#include <stdint.h>

uint8_t tracing_task_lock = 1;
uint8_t k210_task_lock = 1;
uint8_t wave_task_lock = 1;
uint8_t BLEProcessCmd(uint8_t *buf);
void BLEControl(void);
void BLESpeed(void);
void bluetooth_module_init(void)
{
    tracing_task_lock = 1;
    k210_task_lock = 1;
    wave_task_lock = 1;
}
uint8_t BLE_SPEED = 0;
void bluetooth_module_receive_command(uint8_t *buf)
{
    BLE_ACTION action;
    action = BLEProcessCmd(buf);
    switch (action)
    {

    case TRACKING:
        tracing_task_lock = 0;
        k210_task_lock = 1;
        wave_task_lock = 1;
        break;
    case BLE:
        k210_task_lock = 1;
        tracing_task_lock = 1;
        wave_task_lock = 1;
        BLE_SPEED = 0;
        break;
    case K210:
        k210_task_lock = 0;
        tracing_task_lock = 1;
        wave_task_lock = 1;
        break;
    case WAVE:
        wave_task_lock = 0;
        tracing_task_lock = 1;
        k210_task_lock = 1;
        break;
    case SPEED:
        wave_task_lock = 1;
        tracing_task_lock = 1;
        k210_task_lock = 1;
        BLE_SPEED = 1;
        break;
    case Data:
        wave_task_lock = 1;
        tracing_task_lock = 1;
        k210_task_lock = 1;
        if (BLE_SPEED == 1)
        {
            BLESpeed();
        }
        else
        {
            BLEControl();
        }
        break;
    default:
        break;
    }
}
/*
发送格式:
CMD:xxx;
other
接收格式:
OK
ERROR
*/

const char *ACTION[5] = {"TRACKING", "BLE", "K210", "WAVE", "SPEED"};
uint8_t BLEProcessCmd(uint8_t *buf)
{
    char *token = strtok(buf, ":");
    if (token != NULL && strcmp(token, "CMD") == 0)
    {
        token = strtok(NULL, ":");
        if (token != NULL)
        {
            for (int i = 0; i < 5; i++)
            {

                if (strcmp(token, ACTION[i]) == 0)
                {
                    return i;
                }
            }
            assert_param(0);
            return 8;
        }
    }
    else
    {
        // DATA
        return 5;
    }
    assert_param(0);
    return 7;
}
void BLESpeed(void)
{

    if (*Car_USART_Manager.BlueToothSerial.rx_buf[0] < 100)
    {
        uint8_t speed = *Car_USART_Manager.BlueToothSerial.rx_buf[0];
        motor_module_set_speed(BOTH_MOTOR, speed);
    }
}
void BLEControl(void)
{
    switch (*Car_USART_Manager.BlueToothSerial.rx_buf[0])
    {
    case 'F':
        motor_module_set_direction(FORWARD);
        break;
    case 'B':
        motor_module_set_direction(BACKWARD);

        break;
    case 'L':
        motor_module_set_direction(LEFT);
        break;
    case 'R':
        motor_module_set_direction(RIGHT);
        break;
    default:
        motor_module_set_direction(STOP);
        break;
    }
}

void ble_printf(const char *format, ...)
{
    uint16_t i = 0;
    va_list args;
    char buffer[100];
    const char *buffer_ptr = buffer;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    while ((*buffer_ptr) != '\0')
    {
        USART_SendData(USART2, *buffer_ptr++);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
            ;
    }
    va_end(args);
}