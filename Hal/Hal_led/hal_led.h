#ifndef _HAL_LED_H
#define _HAL_LED_H

#include <stdio.h>
#include <stm32f10x.h>
#include "Hal_led/sys.h"

#define relay0	PBout(5)
#define led1	PCout(13)

void ledGpioInit(void);
void ledon(uint8_t index);
void ledoff(uint8_t index);


#endif /*_HAL_RGB_LED_H*/

