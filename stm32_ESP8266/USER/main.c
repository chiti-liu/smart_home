#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "timer.h"
#include "common.h"
#include "gizwits_product.h"
#include "bsp_ili9341_lcd.h"
#include "fonts.h"	
#include "bsp_spi_flash.h"
#include "bsp_adc.h"
#include "dht11.h"
#include <stdio.h>
#include "servo.h"

extern dataPoint_t currentDataPoint;//led灯的状态读取（pwm）
extern u8 count;//倒计时(秒)辅助
extern u8 auto_hum_tem,handle_humidity_Temp,temperature,humidity;	 //温湿度处理另外用到了屏幕，屏幕flash与外设flash冲突不放在product.c中处理
//void    HANDLE_HUM_TEM(void);//温湿度处理
u8 bug;
//协议相关初始
void Gizwits_Init(void)
{

    TIM2_Int_Init(9,7199);	//1MS 系统定时
    usart3_init(9600);		//WIFI 初始化 波特率必须为 9600,pb10,pb11
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
    userInit();				//数据点初始化
	gizwitsInit();			//协议初始化
}
//系统初始化函数
void System_Init(void)
{

	delay_init(); 		//延时函数初始化 
	ILI9341_Init ();    //初始化屏幕pd0\1\4\5\8\9\10\14\15,pe2\7\8\9\10\11\12\13\15\14,pg6\11\12 
    DHT11_Init();   //温湿度初始化pD6    
    ADCx_Init();    //ADC2初始化pc1，ADC_Channel_11目前接在开发板测滑动变阻器
    TIM4_Init();        //舵机控制pb8
    //相关spi——flash：pa4\5\6\7
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //设置 NVIC 中断分组 2:2 位抢占优先级，2 位响应优先级
	
    uart_init(115200); 	//串口初始化为 115200,pa9,pa10
    TIMx_PWM_Init(); 		//LED 端口初始化并且用作TIM3PWM输出pb0,pb1,pb5(TIM3)
	
    KEY_Init(); 		//按键初始化 pa0,pc13
	Gizwits_Init(); 	//协议相关初始
}
void ILI9341_Showcommon()
{
    printf("--------机智云 IOT-LED 控制实验----------\r\n");
    printf("KEY1:AirLink 连接模式\t KEY2:复位\r\n\r\n");
    
    
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);/* 清屏，显示全黑 */
    
    ILI9341_DispStringLine_EN_CH(LINE(0),"------机智云 IOT-智能家居-----");
    ILI9341_DispStringLine_EN_CH(LINE(1),"KEY1:连接WIFI KEY2:复位");
    ILI9341_DispStringLine_EN_CH(LINE(10),"正常范围：20-35℃  50-75%RH");
    ILI9341_DispStringLine_EN_CH(LINE(11),"decrease(减),increase(加)");
    ILI9341_DispStringLine_EN_CH(LINE(12),"处理温湿度状况:");
    ILI9341_DisplayStringEx(32,216, 32,32,"No work!",0);
}
void Cloud_KEY()
{
     	u8 key;
        userHandle();		//用户数据采集
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理,上报当前数据
		LCD_SetTextColor(RED);
		key = KEY_Scan(0);
        if(key==KEY1_PRES)	//KEY1 按键
        {
            printf("WIFI 进入 AirLink 连接模式\r\n");
            delay_ms(20);
            ILI9341_DispStringLine_EN_CH(LINE(2),"WIFI 进入 AirLink 连接模式");
            gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link 模式接入
        }
        if(key==KEY2_PRES)	//KEY2 按键
        { 
            printf("WIFI 复位，请重新配置连接\r\n");
            LCD_ClearLine(LINE(2));
            LCD_ClearLine(LINE(3));/* 清除单行文字 */
            ILI9341_DispStringLine_EN_CH(LINE(2),"WIFI 复位，请重新配置连接");
            gizwitsSetMode(WIFI_RESET_MODE);//WIFI 复位
        }
}
int main(void)
{  
    System_Init();
	ILI9341_Showcommon();////TFT不刷新恒定显示内容
	while(1)
    {
        Cloud_KEY();//机智云按键配网及协议处理
    }
}

 


