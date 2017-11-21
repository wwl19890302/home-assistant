#ifndef _HAL_LED_H
#define _HAL_LED_H

#include <stdio.h>
#include <stm32f10x.h>
#include "Hal_led/sys.h"

#define led0	PCout(13)

void ledGpioInit(void);
void led_on(uint8_t index);
void led_off(uint8_t index);


#endif /*_HAL_LED_H*/

