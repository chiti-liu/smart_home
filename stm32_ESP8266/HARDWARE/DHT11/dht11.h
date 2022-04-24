#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//typedef struct
//{
//	u8  humi_int;		//ʪ�ȵ���������
//	u8  humi_deci;	 	//ʪ�ȵ�С������
//	u8  temp_int;	 	//�¶ȵ���������
//	u8  temp_deci;	 	//�¶ȵ�С������		                 
//} DHT11_Data_TypeDef;
////IO��������											   
#define	DHT11_DQ_OUT PDout(6) //���ݶ˿�	PD6
#define	DHT11_DQ_IN  PDin(6)  //���ݶ˿�	PD6

#define DHT11_GPIO_PORT    	GPIOD			              //GPIO�˿�
#define DHT11_GPIO_CLK 	    RCC_APB2Periph_GPIOD		//GPIO�˿�ʱ��
#define DHT11_GPIO_PIN		GPIO_Pin_6			        //���ӵ�SCLʱ���ߵ�GPIO

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif















