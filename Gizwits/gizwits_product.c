/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
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

#include <stdio.h>
#include <string.h>
#include "Hal_Usart/hal_uart.h"
#include "gizwits_product.h"
#include "common.h"
#include "Hal_led/Hal_led.h"
#include "Hal_relay/Hal_relay.h"
#include "Hal_nrf24l01/Hal_nrf24l01.h"

static uint32_t timerMsCount;

/** User area the current device state structure*/
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
	  extern uint8_t tmp_buf[30];
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_LED_OnOff:
        currentDataPoint.valueLED_OnOff = dataPointPtr->valueLED_OnOff;
        GIZWITS_LOG("Evt: EVENT_LED_OnOff %d \n", currentDataPoint.valueLED_OnOff);
        if(0x01 == currentDataPoint.valueLED_OnOff)
        {
          //user handle  开灯
// 			led0 = 0;
// 			led1 = 0;
			relay_on(0);//ledon(1);
			tmp_buf[5] = 1;
			tmp_buf[6] = 0;
        }
        else
        {
          //user handle    关灯
// 			led0 = 1;
// 			led1 = 1;
			relay_off(0);//ledoff(1);
			tmp_buf[5] = 0;
			tmp_buf[6] = 1;
        }
		  NRF24L01_TX_Mode();
		NRF24L01_TxPacket(tmp_buf);
		NRF24L01_RX_Mode();
		
        break;

      case EVENT_LED_Color:
        currentDataPoint.valueLED_Color = dataPointPtr->valueLED_Color;
        GIZWITS_LOG("Evt: EVENT_LED_Color %d\n", currentDataPoint.valueLED_Color);
        switch(currentDataPoint.valueLED_Color)
        {
          case LED_Color_VALUE0:
            //user handle
//			ledon(0);ledon(1);
		  GIZWITS_LOG("\r\ncustom\r\n");
            break;
          case LED_Color_VALUE1:
            //user handle
//			ledon(0);ledoff(1);
			GIZWITS_LOG("\r\nyellow\r\n");
            break;
          case LED_Color_VALUE2:
            //user handle
//			ledoff(0);ledon(1);
		  GIZWITS_LOG("\r\nred\r\n");
            break;
          case LED_Color_VALUE3:
            //user handle
//			ledoff(0);ledoff(1);
		  GIZWITS_LOG("\r\nblue\r\n");
            break;
          default:
            break;
        }
        break;

      case EVENT_LED_R:
        currentDataPoint.valueLED_R = dataPointPtr->valueLED_R;
        GIZWITS_LOG("Evt:EVENT_LED_R %d\n",currentDataPoint.valueLED_R);
        //user handle
        break;
      case EVENT_LED_G:
        currentDataPoint.valueLED_G = dataPointPtr->valueLED_G;
        GIZWITS_LOG("Evt:EVENT_LED_G %d\n",currentDataPoint.valueLED_G);
        //user handle
        break;
      case EVENT_LED_B:
        currentDataPoint.valueLED_B = dataPointPtr->valueLED_B;
        GIZWITS_LOG("Evt:EVENT_LED_B %d\n",currentDataPoint.valueLED_B);
        //user handle
        break;
      case EVENT_Motor_Speed:
        currentDataPoint.valueMotor_Speed = dataPointPtr->valueMotor_Speed;
        GIZWITS_LOG("Evt:EVENT_Motor_Speed %d\n",currentDataPoint.valueMotor_Speed);
        //user handle
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }
  //此处添加发往从机控制代码，凡从机数据一律等待从机反应后更新到云服务器



		

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valueInfrared = ;//Add Sensor Data Collection
    currentDataPoint.valueTemperature = ;//Add Sensor Data Collection
    currentDataPoint.valueHumidity = ;//Add Sensor Data Collection
    currentDataPoint.valueAlert_1 = ;//Add Sensor Data Collection
    currentDataPoint.valueAlert_2 = ;//Add Sensor Data Collection
    currentDataPoint.valueFault_LED = ;//Add Sensor Data Collection
    currentDataPoint.valueFault_Motor = ;//Add Sensor Data Collection
    currentDataPoint.valueFault_TemHum = ;//Add Sensor Data Collection
    currentDataPoint.valueFault_IR = ;//Add Sensor Data Collection

    */
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueLED_OnOff = ;
      currentDataPoint.valueLED_Color = ;
      currentDataPoint.valueLED_R = ;
      currentDataPoint.valueLED_G = ;
      currentDataPoint.valueLED_B = ;
      currentDataPoint.valueMotor_Speed = ;
      currentDataPoint.valueInfrared = ;
      currentDataPoint.valueTemperature = ;
      currentDataPoint.valueHumidity = ;
      currentDataPoint.valueAlert_1 = ;
      currentDataPoint.valueAlert_2 = ;
      currentDataPoint.valueFault_LED = ;
      currentDataPoint.valueFault_Motor = ;
      currentDataPoint.valueFault_TemHum = ;
      currentDataPoint.valueFault_IR = ;
    */

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}


/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

/**@} */

/**
* @brief Timer TIM3 interrupt handler

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
    if (TIM_GetITStatus(TIMER, TIM_IT_Update) != RESET)  
    {
        TIM_ClearITPendingBit(TIMER, TIM_IT_Update  );
        gizTimerMs();
    }
}

/**
* @brief Timer TIM3 init function

* @param none
* @return none
*/
void timerInit(void)
{
    u16 arr = 7199;
    u16 psc = 9;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(TIMER_RCC, ENABLE); //Clock enable

    //Timer TIM3 init
    
    TIM_TimeBaseStructure.TIM_Period = arr; //Set the value of the auto-reload register cycle in the next update event
    
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //Set the prescaler value used as the TIMx clock frequency
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //Set the clock split:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM count up mode
    TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure); //Initializes the time base according to the specified parameter

    //Enable the specified TIM3 interrupt, allowing update interrupts
    TIM_ITConfig(TIMER, TIM_IT_Update,ENABLE );

    //Set NVIC interrupt priority
    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ;  //TIM3 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //Excellent priority level 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //Slave priority level 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ channel is enabled
    NVIC_Init(&NVIC_InitStructure);  //Initialize the NVIC register
    TIM_Cmd(TIMER, ENABLE);  //TIMx enable
}

/**
* @brief USART2 interrupt handler

* Receive the serial protocol data from the WiFi module
* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
    uint8_t value = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        value = USART_ReceiveData(USART2);

        gizPutData(&value, 1);
    }
}


/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }

    GIZWITS_LOG("\n");
#endif
    
    for(i=0; i<len; i++)
    {
        USART_SendData(UART, buf[i]);
        while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);//Loop until the end of transmission

        if(i >=2 && buf[i] == 0xFF)
        {
          USART_SendData(UART,0x55);
          while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);//Loop until the end of transmission
        }
    }
        
    return len;
}

/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
void uartInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    UART_GPIO_Cmd(UART_GPIO_CLK, ENABLE);
    UART_CLK_Cmd(UART_CLK, ENABLE);
    UART_AFIO_Cmd(UART_AFIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = UART_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = UART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART, &USART_InitStructure);

    USART_ITConfig(UART,USART_IT_RXNE,ENABLE);
    USART_Cmd(UART, ENABLE);
    USART_ClearFlag(UART, USART_FLAG_TC); //Clear transmission complete flag 

    /*Enable serial interrupt, and set the priority*/
    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

