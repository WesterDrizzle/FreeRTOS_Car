// #include "FreeRTOS.h"
// #include "task.h"
#include "mytask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tracking_module.h"
#include "bluetooth_module.h"
#include "k210_module.h"
#include "motor_module.h"
#include "mpu6050_module.h"
#include "ultrasonic_wave_module.h"
#include "wifi_module.h"
#include "string.h"
TaskHandle_t start_task_handler;
TaskHandle_t computer_serial_task_handler;
TaskHandle_t mpu6050_task_handler;
TaskHandle_t ultrasonic_wave_task_handler;
TaskHandle_t k210_task_handler;
TaskHandle_t wifi_task_handler;
TaskHandle_t bluetooth_task_handler;
TaskHandle_t tracking_task_handler;
TaskHandle_t motor_task_handler;

void computer_serial_task(void *parameter);
void mpu6050_task(void *parameter);
void ultrasonic_wave_task(void *parameter);
void k210_task(void *parameter);
void wifi_task(void *parameter);
void bluetooth_task(void *parameter);
void tracking_task(void *parameter);
void motor_task(void *parameter);
extern uint8_t tracing_task_lock;
extern uint8_t k210_task_lock;
extern uint8_t wave_task_lock;
void StartCarTask(void)
{

    xTaskCreate((TaskFunction_t)start_task,           // 任务函数
                (const char *)"start_task",           // 任务名称
                (uint16_t)start_task_stack_size,      // 任务堆栈大小
                (void *)NULL,                         // 传递给任务函数的参数
                (UBaseType_t)start_task_priority,     // 任务优先级
                (TaskHandle_t *)&start_task_handler); // 任务句柄
    vTaskStartScheduler();
}
void start_task(void *Parameters)
{
    taskENTER_CRITICAL(); // 进入临界区

    xTaskCreate((TaskFunction_t)bluetooth_task,
                (const char *)"bluetooth",
                (uint16_t)bluetooth_task_stack_size,
                (void *)NULL,
                (UBaseType_t)bluetooth_task_priority,
                (TaskHandle_t *)&bluetooth_task_handler);
    xTaskCreate((TaskFunction_t)ultrasonic_wave_task,
                (const char *)"ultrasonic_wave",
                (uint16_t)ultrasonic_wave_task_stack_size,
                (void *)NULL,
                (UBaseType_t)ultrasonic_wave_task_priority,
                (TaskHandle_t *)&ultrasonic_wave_task_handler);

    // xTaskCreate((TaskFunction_t)k210_task,
    //             (const char *)"k210",
    //             (uint16_t)k210_task_stack_size,
    //             (void *)NULL,
    //             (UBaseType_t)k210_task_priority,
    //             (TaskHandle_t *)&k210_task_handler);

    // xTaskCreate((TaskFunction_t)tracking_task,
    //             (const char *)"tracking",
    //             (uint16_t)tracking_task_stack_size,
    //             (void *)NULL,
    //             (UBaseType_t)tracking_task_priority,
    //             (TaskHandle_t *)&tracking_task_handler);

    // xTaskCreate((TaskFunction_t)motor_task,
    //             (const char *)"motor",
    //             (uint16_t)motor_task_stack_size,
    //             (void *)NULL,
    //             (UBaseType_t)motor_task_priority,
    //             (TaskHandle_t *)&motor_task_handler);

    xTaskCreate((TaskFunction_t)computer_serial_task,
                (const char *)"computer_serial",
                (uint16_t)computer_serial_task_stack_size,
                (void *)NULL,
                (UBaseType_t)computer_serial_task_priority,
                (TaskHandle_t *)&computer_serial_task_handler);

    //    xTaskCreate((TaskFunction_t)wifi_task,
    //                (const char *)"wifi",
    //                (uint16_t)wifi_task_stack_size,
    //                (void *)NULL,
    //                (UBaseType_t)wifi_task_priority,
    //                (TaskHandle_t *)&wifi_task_handler);

    //    xTaskCreate((TaskFunction_t)mpu6050_task,
    //                (const char *)"mpu6050",
    //                (uint16_t)mpu6050_task_stack_size,
    //                (void *)NULL,
    //                (UBaseType_t)mpu6050_task_priority,
    //                (TaskHandle_t *)&mpu6050_task_handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL(); // 退出临界区
}
void computer_serial_task(void *parameter)
{
    usart_rx_status rx_val;
    uint8_t(*rxbuf)[] = Car_USART_Manager.ComputerSerial.rx_buf;
    USART_Manager_Start(ComputerSerial, 9600);
    while (1)
    {
        xTaskNotifyWait(0, 0xFFFFFFFF, &rx_val.value, portMAX_DELAY);
        printf("computer serial receive len :%d\r\n", rx_val.status.rx_len);
        printf("computer serial receive :%s\r\n", *rxbuf);
        Car_USART_Manager.ComputerSerial.ComputerSerialRxStatus.value = 0;
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void ultrasonic_wave_task(void *parameter)
{
    char buffer[100]; // 确保这个字符串足够大以存储格式化后的内容
    uint16_t distance = 0;
    ultrasonic_wave_module_init();
    while (1)
    {
        if (wave_task_lock)
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            continue;
        }
#if DEBUG
        printf("distance is %d mm\r\n", ultrasonic_wave_module_get_distance());
        ble_printf(buffer, sizeof(buffer));
#endif
        distance = ultrasonic_wave_module_get_distance();
        ble_printf("distance is %d\r\n", distance);

        if (distance < 20)
        {
            motor_module_set_direction(FORWARD);
        }
        else
        {
            motor_module_set_direction(BACKWARD);
        }
        // memset(buffer, 0, sizeof(buffer));
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
extern struct _k210_data k210_data;
void k210_task(void *parameter)
{
    usart_rx_status rx_val;
    uint8_t(*rxbuf)[] = Car_USART_Manager.K210Serial.rx_buf;
    USART_Manager_Start(K210Serial, 9600);
    k210_module_init();
    while (1)
    {
        if (k210_task_lock)
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            continue;
        }
        xTaskNotifyWait(0, 0xFFFFFFFF, &rx_val.value, portMAX_DELAY);
#if DEBUG
        printf("K210 serial receive len :%d\r\n", rx_val.status.rx_len);
        printf("K210 serial receive :%s\r\n", *rxbuf);
        printf("K210 serial receive x = %d , y=%d\r\n", k210_data.x, k210_data.y);
#endif
        k210_process_data(*rxbuf);
        if (k210_data.x < 160)
        {
            motor_module_set_direction(LEFT);
        }
        else if (k210_data.x > 160)
        {
            motor_module_set_direction(RIGHT);
        }
        Car_USART_Manager.K210Serial.K210SerialRxStatus.value = 0;
        memset(*rxbuf, 0, sizeof(*Car_USART_Manager.K210Serial.rx_buf));
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void bluetooth_task(void *parameter)
{
    // uint32_t rx_val;
    usart_rx_status rx_val;
    uint8_t(*rxbuf)[] = Car_USART_Manager.BlueToothSerial.rx_buf;
    USART_Manager_Start(BlueToothSerial, 9600);
    bluetooth_module_init();
    while (1)
    {
        xTaskNotifyWait(0, 0xFFFFFFFF, &rx_val.value, portMAX_DELAY);
#if DEBUG
        printf("BLE Receive: %s\r\n", *rxbuf);
        ble_printf((uint8_t *)(*rxbuf), rx_val.status.rx_len);
#endif
        bluetooth_module_receive_command((uint8_t *)(*rxbuf));
        Car_USART_Manager.BlueToothSerial.BlueToothSerialRxStatus.value = 0;
        memset(*rxbuf, 0, sizeof(*Car_USART_Manager.BlueToothSerial.rx_buf));
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
void tracking_task(void *parameter)
{
    tracking_module_data tracking_data;
    tracking_module_init();
    while (1)
    {
        if (tracing_task_lock)
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            continue;
        }
        tracking_data.value = tracking_module_get_data();
        if (tracking_data.bits.left)
        {
            motor_module_set_direction(RIGHT);
        }
        else if (tracking_data.bits.right)
        {
            motor_module_set_direction(LEFT);
        }
        else if (tracking_data.bits.middle)
        {
            motor_module_set_direction(FORWARD);
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
void motor_task(void *parameter)
{
    static uint8_t i = 0;
    motor_module_init();
    motor_module_set_speed(BOTH_MOTOR, 30);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

// void wifi_task(void *parameter)
// {
//     wifi_module_init();
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(50));
//     }
// }

// void mpu6050_task(void *parameter)
// {

//     mpu6050_module_init();
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(50));
//     }
// }