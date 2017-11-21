/**
************************************************************
* @file         main.c
* @brief        MCU entry file
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <string.h>
#include "Hal_key/hal_key.h"
#include "Hal_Usart/hal_uart.h"
#include "Hal_Watchdog/hal_watchdog.h"
#include "gizwits_product.h"
#include "common.h"
#include "Hal_Led/hal_led.h"


/**@name Key-related macro definition 
* @{
*/
#define GPIO_KEY_NUM 4                                  ///< Defines the total number of key member
keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< Defines a single key member array pointer
keysTypedef_t keys;                                     ///< Defines the overall key module structure pointer
/**@} */

/**
* key1 short press handle
* @param none
* @return none
*/
void key1ShortPress(void)
{
    GIZWITS_LOG("KEY1 PRESS ,Production Mode\n");
    gizwitsSetMode(WIFI_PRODUCTION_TEST);
}

/**
* key1 long press handle
* @param none
* @return none
*/
void key1LongPress(void)
{
    GIZWITS_LOG("KEY1 PRESS LONG ,Wifi Reset\n");
    gizwitsSetMode(WIFI_RESET_MODE);

}

/**
* key2 short press handle
* @param none
* @return none
*/
void key2ShortPress(void)
{
    GIZWITS_LOG("KEY2 PRESS ,Soft AP mode\n");
	ledoff(1);
    #if !MODULE_TYPE
    gizwitsSetMode(WIFI_SOFTAP_MODE);
    #endif
}

/**
* key2 long press handle
* @param none
* @return none
*/
void key2LongPress(void)
{
    //AirLink mode
    GIZWITS_LOG("KEY2 PRESS LONG ,AirLink mode\n");
	ledon(1);
    #if !MODULE_TYPE
    gizwitsSetMode(WIFI_AIRLINK_MODE);
    #endif
}

/**
* key3 short press handle
* @param none
* @return none
*/
void key3ShortPress(void)
{
	static uint8_t i=0;
	i++;
    GIZWITS_LOG("KEY3 PRESS ,led_color custom\n");
	if(i%2 == 0)
		{ledon(0);currentDataPoint.valueLED_OnOff = LED_Color_VALUE1;}
	else	{ledoff(0);currentDataPoint.valueLED_OnOff = LED_Color_VALUE0;}
//    ledon(0);ledon(1);
	currentDataPoint.valueLED_Color = LED_Color_VALUE0;
}

/**
* key3 long press handle
* @param none
* @return none
*/
void key3LongPress(void)
{
    GIZWITS_LOG("KEY3 PRESS LONG ,led_color yellow\n");
    ledon(0);ledoff(1);
	currentDataPoint.valueLED_Color = LED_Color_VALUE1;
}

/**
* key4 short press handle
* @param none
* @return none
*/
void key4ShortPress(void)
{
    GIZWITS_LOG("KEY4 PRESS ,led_color red\n");
    ledoff(0);ledon(1);
	currentDataPoint.valueLED_Color = LED_Color_VALUE2;
}

/**
* key4 long press handle
* @param none
* @return none
*/
void key4LongPress(void)
{
    GIZWITS_LOG("KEY4 PRESS LONG ,led_color blue\n");
    ledoff(0);ledoff(1);
	currentDataPoint.valueLED_Color = LED_Color_VALUE3;
}

/**
* Key init function
* @param none
* @return none
*/
void keyInit(void)
{
    singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_0, key1ShortPress, key1LongPress);
    singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_10, key2ShortPress, key2LongPress);
	singleKey[2] = keyInitOne(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_11, key3ShortPress, key3LongPress);
//    singleKey[3] = keyInitOne(RCC_APB2Periph_GPIOE, GPIOE, GPIO_Pin_4, key4ShortPress, key4LongPress);
    keys.singleKey = (keyTypedef_t *)&singleKey;
    keyParaInit(&keys); 
}

/**
* @brief Program entry function

* 
* @param none
* @return none
*/
int main(void)
{
    SystemInit();
    
	uartxInit();        //print serial port init
	 
    userInit();
    gizwitsInit();

	ledGpioInit();
    keyInit();
    timerInit();
    uartInit();         //communication serial port init
   
    watchdogInit(2);    //watchdog reset time :2s

    GIZWITS_LOG("MCU Init Success \n");
    while(1)
    {
        watchdogFeed();
        
        userHandle();
        
        gizwitsHandle((dataPoint_t *)&currentDataPoint);
    }
}

