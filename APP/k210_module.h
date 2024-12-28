#ifndef _k210_module_h_
#define _k210_module_h_
#include "system.h"

struct _k210_data
{
    uint16_t x;
    uint16_t y;
};

void k210_module_init(void);
void k210_process_data(char* rxbuf);
#endif
