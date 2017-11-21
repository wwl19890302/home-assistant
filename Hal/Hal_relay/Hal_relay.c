/**
********************************************************
*
* @file      Hal_rgb_led.c
* @author    Gizwtis
* @version   V03010100
* @date      2016-07-05
*
* @brief     机智云.只为智能硬件而生
*            Gizwits Smart Cloud  for Smart Products
*            链接|增值ֵ|开放|中立|安全|自有|自由|生态
*            www.gizwits.com
*
*********************************************************/
#include "Hal_relay/Hal_relay.h"

void relayGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	relay0 = 0;

}

void relay_on(uint8_t index)
{
	if(index == 0)
	{
		relay0 = 1;
	}
//	else {led1 = 0;}
}
void relay_off(uint8_t index)
{
	if(index == 0)
	{
		relay0 = 0;
	}
//	else {led1 = 1;}
}


