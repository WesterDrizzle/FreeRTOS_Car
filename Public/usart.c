#include "usart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

u8 USART1_RX_BUF[USART1_REC_LEN];
#define USART1_RX_STA Car_USART_Manager.ComputerSerial.ComputerSerialRxStatus.value
u8 USART2_RX_BUF[USART2_REC_LEN];
#define USART2_RX_STA Car_USART_Manager.BlueToothSerial.BlueToothSerialRxStatus.value
u8 USART3_RX_BUF[USART3_REC_LEN];
#define USART3_RX_STA Car_USART_Manager.K210Serial.K210SerialRxStatus.value

struct USART_Manager Car_USART_Manager = {0};
void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);

void USART_Manager_Init()
{
	memset(&Car_USART_Manager, 0, sizeof(struct USART_Manager));
	Car_USART_Manager.ComputerSerial.init = USART1_Init;
	Car_USART_Manager.ComputerSerial.rx_buf = &USART1_RX_BUF;
	Car_USART_Manager.ComputerSerial.ComputerSerialRxStatus.value = 0;

	Car_USART_Manager.BlueToothSerial.init = USART2_Init;
	Car_USART_Manager.BlueToothSerial.rx_buf = &USART2_RX_BUF;
	Car_USART_Manager.BlueToothSerial.BlueToothSerialRxStatus.value = 0;

	Car_USART_Manager.K210Serial.init = USART3_Init;
	Car_USART_Manager.K210Serial.rx_buf = &USART3_RX_BUF;
	Car_USART_Manager.K210Serial.K210SerialRxStatus.value = 0;
}
/**
 * @param USART_Type ComputerSerial,BlueToothSerial,K210Serial
 * @param baud 9600 115200
 */
void USART_Manager_Start(enum USART_Type port, uint32_t baud)
{
	switch (port)
	{
	case ComputerSerial:
		Car_USART_Manager.ComputerSerial.init(baud);
		break;
	case BlueToothSerial:
		Car_USART_Manager.BlueToothSerial.init(baud);
		break;
	case K210Serial:
		Car_USART_Manager.K210Serial.init(baud);
		break;
	}
}

/**
 * @brief 电脑串口
 */
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief 蓝牙串口
 */
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief k210串口
 */
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
extern TaskHandle_t computer_serial_task_handler;
;
void USART1_IRQHandler(void)
{
	u8 r;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		r = USART_ReceiveData(USART1);

		if ((USART1_RX_STA & 0x8000) == 0)
		{
			if (USART1_RX_STA & 0x4000)
			{
				if (r != 0x0a)
					USART1_RX_STA = 0;
				else
				{
					USART1_RX_STA |= 0x8000;
					xTaskNotifyFromISR(computer_serial_task_handler, USART1_RX_STA, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				}
			}
			else
			{
				if (r == 0x0d)
					USART1_RX_STA |= 0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = r;
					USART1_RX_STA++;
					if (USART1_RX_STA > (USART1_REC_LEN - 1))
						USART1_RX_STA = 0;
				}
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
extern TaskHandle_t bluetooth_task_handler;

void USART2_IRQHandler(void)
{
	// u32 r;
	// BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	// if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	// {
	// 	r = USART_ReceiveData(USART2);
	// 	xTaskNotifyFromISR(bluetooth_task_handler, r, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
	// 	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	// 	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	// }
	u8 r;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		r = USART_ReceiveData(USART2);

		if ((USART2_RX_STA & 0x8000) == 0)
		{
			if (USART2_RX_STA & 0x4000)
			{
				if (r != 0x0a)
					USART2_RX_STA = 0;
				else
				{
					USART2_RX_STA |= 0x8000;
					xTaskNotifyFromISR(bluetooth_task_handler, USART2_RX_STA, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				}
			}
			else
			{
				if (r == 0x0d)
					USART2_RX_STA |= 0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = r;
					USART2_RX_STA++;
					if (USART2_RX_STA > (USART2_REC_LEN - 1))
						USART2_RX_STA = 0;
				}
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
extern TaskHandle_t k210_task_handler;

void USART3_IRQHandler(void)
{
	u8 r;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		r = USART_ReceiveData(USART3);
		if ((USART3_RX_STA & 0x8000) == 0)
		{
			if (USART3_RX_STA & 0x4000)
			{
				if (r != 0x0a)
					USART3_RX_STA = 0;
				else
				{
					USART3_RX_STA |= 0x8000;
					xTaskNotifyFromISR(k210_task_handler, USART3_RX_STA, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				}
			}
			else
			{
				if (r == 0x0d)
					USART3_RX_STA |= 0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = r;
					USART3_RX_STA++;
					if (USART3_RX_STA > (USART3_REC_LEN - 1))
						USART3_RX_STA = 0;
				}
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

int fputc(int ch, FILE *p)
{
	USART_SendData(USART1, (u8)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	return ch;
}

void assert_failed(uint8_t *file, uint32_t line)
{
	printf("error in file %s ,in line %d", file, line);
}
