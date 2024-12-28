#ifndef _usart_H
#define _usart_H
#include "system.h"
#include "stdio.h"

/********************************define***************************************/
#define USART1_REC_LEN 200
#define USART2_REC_LEN 200
#define USART3_REC_LEN 200

enum USART_Type
{
    ComputerSerial,
    BlueToothSerial,
    K210Serial,
};

typedef union rx_status
{
    struct _rx_status
    {
        uint16_t rx_len : 14;
        uint16_t rx_wait_to_finish : 1;
        uint16_t rx_already_finish : 1;
        uint16_t reserved;
    } status;
    uint32_t value;
} usart_rx_status;

struct USART_Manager
{
    struct _ComputerSerial
    {
        usart_rx_status ComputerSerialRxStatus;
        uint8_t (*rx_buf)[USART1_REC_LEN];
        void (*init)(u32);
    } ComputerSerial;
    struct _BlueToothSerial
    {
        usart_rx_status BlueToothSerialRxStatus;
        uint8_t (*rx_buf)[USART2_REC_LEN];
        void (*init)(u32);
    } BlueToothSerial;
    struct _K210Serial
    {
        usart_rx_status K210SerialRxStatus;
        uint8_t (*rx_buf)[USART3_REC_LEN];
        void (*init)(u32);
    } K210Serial;
};

extern struct USART_Manager Car_USART_Manager;

/*********************************function****************************************/

void USART_Manager_Init(void);
void USART_Manager_Start(enum USART_Type port, uint32_t baud);

#endif
