#include "system.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mytask.h"
#include "board.h"

int main()
{
    SysTick_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    board_init();
    StartCarTask();
    // 创建开始任务
}
