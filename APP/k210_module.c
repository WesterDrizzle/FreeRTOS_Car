#include "k210_module.h"
#include <stdio.h>
#include <string.h>
void k210_module_init(void)
{
}
struct _k210_data k210_data;



void k210_process_data(char *rxbuf)
{
    char *token = strtok(rxbuf, ",");
	memset(&k210_data,0,sizeof(k210_data));
    if (token != NULL) {
        k210_data.x = (uint16_t)atoi(token); // 将字符串转换为整数
    }

    // 获取第二个值 (y)
    token = strtok(NULL, ",");
    if (token != NULL) {
        k210_data.y = (uint16_t)atoi(token); // 将字符串转换为整数
    }
}

 
