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
#include "Hal_relay/Hal_relay.h"
#include "Hal_nrf24l01/Hal_nrf24l01.h"

void rx_data_pro(void);

uint8_t tx_tmp_buf[32];	//需要发送给各从机的命令  
/* 身份码    灯开关   热水器   电视  窗帘  空调  音乐  安防  插座 
**  4B         9B       1B       1B    1B   5B     3B   5B     3B
主机含有灯1操作
*/
uint8_t rx_tmp_buf[32];//从各从机接收到的状态

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
	led_off(0);
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
	led_on(0);
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
		{relay_on(0);currentDataPoint.valueLED_OnOff = 0x01;}
	else	{relay_off(0);currentDataPoint.valueLED_OnOff = 0x00;}
//    ledon(0);ledon(1);
//	currentDataPoint.valueLED_Color = LED_Color_VALUE0;
}

/**
* key3 long press handle
* @param none
* @return none
*/
void key3LongPress(void)
{
    GIZWITS_LOG("KEY3 PRESS LONG ,led_color yellow\n");
//    led_on(0);led_off(1);
//	currentDataPoint.valueLED_Color = LED_Color_VALUE1;
}

/**
* key4 short press handle
* @param none
* @return none
*/
void key4ShortPress(void)
{
    GIZWITS_LOG("KEY4 PRESS ,led_color red\n");
//    led_off(0);led_on(1);
//	currentDataPoint.valueLED_Color = LED_Color_VALUE2;
}

/**
* key4 long press handle
* @param none
* @return none
*/
void key4LongPress(void)
{
    GIZWITS_LOG("KEY4 PRESS LONG ,led_color blue\n");
//    led_off(0);led_off(1);
//	currentDataPoint.valueLED_Color = LED_Color_VALUE3;
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
	relayGpioInit();
	NRF24L01_Init();  
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
		

		if(NRF24L01_RxPacket(rx_tmp_buf)==0)//一旦接收到信息,则显示出来.
        {
            rx_data_pro();
        }
	
    }
}

/* 子设备数据变动判断 */
uint8_t subset_data_change_check(void)
{
	uint8_t state = 0;
    if(rx_tmp_buf[3] == 1) //subset 1
    {
		currentDataPoint.valueLED_R = rx_tmp_buf[5];
		currentDataPoint.valueLED_G = rx_tmp_buf[6];
        state = 1;
    }

    if(state)
    {
        state = 0;
        printf("subset1 data changed!\r\n");   //
        return 1;   //control data change
    }
    else
    {
        printf("subset1 data not changed!\r\n");   //
        return 0;   //control data not change
    }
}

/* 接收子设备发来的数据，之后判断处理 */
void rx_data_pro(void)
{
	uint8_t	state;

    printf("%s\r\n",rx_tmp_buf);   //打印接收到的字符

    state = subset_data_change_check();
    if(state)   //report change to home assistant
    {
        state = 0;
    }
}

