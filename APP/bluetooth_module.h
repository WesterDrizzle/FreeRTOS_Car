#ifndef _bluetooth_module_h_
#define _bluetooth_module_h_
#include "system.h"
/*define*/
typedef enum BLE_ACTION
{
    TRACKING = 0,
    BLE,
    K210,
    WAVE,
    SPEED,
    Data
} BLE_ACTION;

/*function*/
void bluetooth_module_init(void);
void bluetooth_module_receive_command(uint8_t *buf);
void ble_printf(const char *format, ...);

#endif
