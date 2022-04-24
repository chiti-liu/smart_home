#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//typedef struct
//{
//	u8  humi_int;		//湿度的整数部分
//	u8  humi_deci;	 	//湿度的小数部分
//	u8  temp_int;	 	//温度的整数部分
//	u8  temp_deci;	 	//温度的小数部分		                 
//} DHT11_Data_TypeDef;
////IO操作函数											   
#define	DHT11_DQ_OUT PDout(6) //数据端口	PD6
#define	DHT11_DQ_IN  PDin(6)  //数据端口	PD6

#define DHT11_GPIO_PORT    	GPIOD			              //GPIO端口
#define DHT11_GPIO_CLK 	    RCC_APB2Periph_GPIOD		//GPIO端口时钟
#define DHT11_GPIO_PIN		GPIO_Pin_6			        //连接到SCL时钟线的GPIO

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif















