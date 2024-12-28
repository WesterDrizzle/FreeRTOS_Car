#ifndef _tracking_module_h_
#define _tracking_module_h_
#include "system.h"
typedef union tracking_module_data
{
    struct _tracking_module_data
    {
        uint8_t left : 1;
        uint8_t middle : 1;
        uint8_t right : 1;
        uint8_t reserved : 5;
    } bits;
    uint8_t value;
} tracking_module_data;

uint8_t tracking_module_get_data(void);
void tracking_module_init(void);
#endif
