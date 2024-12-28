#include "board.h"
#include "usart.h"
#include "pwm.h"

void board_init(void)
{
    USART_Manager_Init();
    USART_Manager_Start(ComputerSerial, 9600);
}

