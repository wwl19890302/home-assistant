#ifndef _HAL_RELAY_H
#define _HAL_RELAY_H

#include <stdio.h>
#include <stm32f10x.h>
#include "Hal_led/sys.h"

#define relay0	PBout(5)

void relayGpioInit(void);
void relay_on(uint8_t index);
void relay_off(uint8_t index);


#endif /*_HAL_RELAY_H*/

