#ifndef _HAL_RELAY_H
#define _HAL_RELAY_H

#include <stdio.h>
#include <stm32f10x.h>
#include "Hal_led/sys.h"

//relay define
#define relay0_rcc      RCC_APB2Periph_GPIOB
#define relay0_gpio     GPIOB
#define relay0_pin      (GPIO_Pin_5)

void relayGpioInit(void);
void relay_on(uint8_t index);
void relay_off(uint8_t index);


#endif /*_HAL_RELAY_H*/

