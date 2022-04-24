/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "gizwits_product.h"
#include "led.h"
#include "dht11.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_adc.h"
#include "servo.h"
static uint32_t timerMsCount;
extern u8 temperature,humidity;
extern  float ADC_ConvertedValueLocal ;
/** Current datapoint */
dataPoint_t currentDataPoint;
u8 auto_rain=0,auto_hum_tem=0,handle_humidity_Temp=0,pre_rain=0,cur_rain=0;
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
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)//事件处理，添加逻辑控制
{
  uint8_t i = 0;
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
      case EVENT_All_Led:
        currentDataPoint.valueAll_Led = dataPointPtr->valueAll_Led;
        GIZWITS_LOG("Evt: EVENT_All_Led %d \n", currentDataPoint.valueAll_Led);
        if(0x01 == currentDataPoint.valueAll_Led)
        {
            currentDataPoint.valueBlue_Led=100;//LED1_ON;
            currentDataPoint.valueRed_Led=100;//LED2_ON;
            currentDataPoint.valueGreen_Led=100;//LED3_ON;
            TIM3->CCR4=currentDataPoint.valueBlue_Led;	//WIFI控制PWM输出
            TIM3->CCR2=currentDataPoint.valueRed_Led ;
            TIM3->CCR3=currentDataPoint.valueGreen_Led;
        }
        else
        {
           currentDataPoint.valueBlue_Led=0;//LED1_OFF;
            currentDataPoint.valueRed_Led=0;//LED2_OFF;
            currentDataPoint.valueGreen_Led=0;//LED3_OFF;
            TIM3->CCR4=currentDataPoint.valueBlue_Led;	//WIFI控制PWM输出
            TIM3->CCR2=currentDataPoint.valueRed_Led ;
            TIM3->CCR3=currentDataPoint.valueGreen_Led;
        }
        break;
      case EVENT_auto_rain:
        currentDataPoint.valueauto_rain = dataPointPtr->valueauto_rain;
        GIZWITS_LOG("Evt: EVENT_auto_rain %d \n", currentDataPoint.valueauto_rain);
        if(0x01 == currentDataPoint.valueauto_rain)
        {
          auto_rain=1;
            switch(currentDataPoint.valueRain)
          {
              case Rain_VALUE0:
                    Servo_Control(0);
                    currentDataPoint.valuewindow_open_rangle=0;
                    break;
              case Rain_VALUE1:
                    Servo_Control(36);
                    currentDataPoint.valuewindow_open_rangle=36;
                    break;
              case Rain_VALUE2:
                    Servo_Control(72);
                    currentDataPoint.valuewindow_open_rangle=72;
                    break;
              case Rain_VALUE3:
                   Servo_Control(108);
                    currentDataPoint.valuewindow_open_rangle=108;
                    break;
              default:
                    break;
          }
        }
        else
        { 
          auto_rain=0;            
        }
        break;
      case EVENT_auto_hum_tem:
        currentDataPoint.valueauto_hum_tem = dataPointPtr->valueauto_hum_tem;
        GIZWITS_LOG("Evt: EVENT_auto_hum_tem %d \n", currentDataPoint.valueauto_hum_tem);
        if(0x01 == currentDataPoint.valueauto_hum_tem)
        {
          auto_hum_tem=1;
        }
        else
        {
          auto_hum_tem=0;   
        }
        break;

      case EVENT_handle_humidity_Temp:
        currentDataPoint.valuehandle_humidity_Temp = dataPointPtr->valuehandle_humidity_Temp;
        GIZWITS_LOG("Evt: EVENT_handle_humidity_Temp %d\n", currentDataPoint.valuehandle_humidity_Temp);
        break;

      case EVENT_Red_Led:
        currentDataPoint.valueRed_Led = dataPointPtr->valueRed_Led;
        GIZWITS_LOG("Evt:EVENT_Red_Led %d\n",currentDataPoint.valueRed_Led);
         TIM3->CCR2=currentDataPoint.valueRed_Led  ;
        break;
      case EVENT_Green_Led:
        currentDataPoint.valueGreen_Led = dataPointPtr->valueGreen_Led;
        GIZWITS_LOG("Evt:EVENT_Green_Led %d\n",currentDataPoint.valueGreen_Led);
         TIM3->CCR3=currentDataPoint.valueGreen_Led ;
        break;
      case EVENT_Blue_Led:
        currentDataPoint.valueBlue_Led = dataPointPtr->valueBlue_Led;
        GIZWITS_LOG("Evt:EVENT_Blue_Led %d\n",currentDataPoint.valueBlue_Led);
         TIM3->CCR4=currentDataPoint.valueBlue_Led;
        break;
      case EVENT_window_open_rangle:
        currentDataPoint.valuewindow_open_rangle = dataPointPtr->valuewindow_open_rangle;
        GIZWITS_LOG("Evt:EVENT_window_open_rangle %d\n",currentDataPoint.valuewindow_open_rangle);
        if(auto_rain==0)
        Servo_Control(currentDataPoint.valuewindow_open_rangle);
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

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
extern u8 bug;
void userHandle(void)       //用户端数据采集上传
{
    if((TIM3->CCR2==100)&&(TIM3->CCR3==100)&&(TIM3->CCR4==100))//灯总开关
        currentDataPoint.valueAll_Led= 1;
    else 
        currentDataPoint.valueAll_Led= 0;
    
    currentDataPoint.valueRed_Led = TIM3->CCR2;             //灯光亮度
    currentDataPoint.valueGreen_Led = TIM3->CCR3  ;
    currentDataPoint.valueBlue_Led = TIM3->CCR4 ;
    
    if(ADC_ConvertedValueLocal >=0&&ADC_ConvertedValueLocal <0.8)//雨滴
        currentDataPoint.valueRain = Rain_VALUE0 ;//Add Sensor Data Collection
    else if(ADC_ConvertedValueLocal >=0.8&&ADC_ConvertedValueLocal <1.6)
        currentDataPoint.valueRain = Rain_VALUE1 ;//Add Sensor Data Collection
    else if(ADC_ConvertedValueLocal >=1.6&&ADC_ConvertedValueLocal <2.4)
        currentDataPoint.valueRain = Rain_VALUE2 ;//Add Sensor Data Collection
    else
        currentDataPoint.valueRain = Rain_VALUE3 ;//Add Sensor Data Collection
    currentDataPoint.valuewindow_open_rangle=((TIM4->CCR3)-5)*9;      //
//    temp = (1.0 / 9.0) * angle + 5.0;//寄存器值 = 1/9 * 角度 + 5
    
    currentDataPoint.valueTemperature =temperature;//Add Sensor Data Collection//温湿度
    
    currentDataPoint.valuehumidity =humidity;//Add Sensor Data Collection
    
    if(auto_hum_tem==1)
    {
        if(humidity>75||temperature>35)
            currentDataPoint.valuehandle_humidity_Temp=2;
        else if(humidity<50||temperature<20)
           currentDataPoint.valuehandle_humidity_Temp=1;
        else 
            currentDataPoint.valuehandle_humidity_Temp=0;
    }
   else currentDataPoint.valuehandle_humidity_Temp=currentDataPoint.valuehandle_humidity_Temp;
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
      currentDataPoint.valueAll_Led = ;
      currentDataPoint.valueauto_rain = ;
      currentDataPoint.valueauto_hum_tem = ;
      currentDataPoint.valuehandle_humidity_Temp = ;
      currentDataPoint.valueRed_Led = ;
      currentDataPoint.valueGreen_Led = ;
      currentDataPoint.valueBlue_Led = ;
      currentDataPoint.valuewindow_open_rangle = ;
      currentDataPoint.valueRain = ;
      currentDataPoint.valueTemperature = ;
      currentDataPoint.valuehumidity = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{

}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
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
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        USART_SendData(USART3, buf[i]);//STM32 test demo
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
        //Serial port to achieve the function, the buf[i] sent to the module
        if(i >=2 && buf[i] == 0xFF)
        {
			//Serial port to achieve the function, the 0x55 sent to the module
			USART_SendData(USART3, 0x55);//STM32 test demo
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
        }
    }


    
    return len;
}


