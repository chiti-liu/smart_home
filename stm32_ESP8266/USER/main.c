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

extern dataPoint_t currentDataPoint;//led�Ƶ�״̬��ȡ��pwm��
extern u8 count;//����ʱ(��)����
extern u8 auto_hum_tem,handle_humidity_Temp,temperature,humidity;	 //��ʪ�ȴ��������õ�����Ļ����Ļflash������flash��ͻ������product.c�д���
//void    HANDLE_HUM_TEM(void);//��ʪ�ȴ���
u8 bug;
//Э����س�ʼ
void Gizwits_Init(void)
{

    TIM2_Int_Init(9,7199);	//1MS ϵͳ��ʱ
    usart3_init(9600);		//WIFI ��ʼ�� �����ʱ���Ϊ 9600,pb10,pb11
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
    userInit();				//���ݵ��ʼ��
	gizwitsInit();			//Э���ʼ��
}
//ϵͳ��ʼ������
void System_Init(void)
{

	delay_init(); 		//��ʱ������ʼ�� 
	ILI9341_Init ();    //��ʼ����Ļpd0\1\4\5\8\9\10\14\15,pe2\7\8\9\10\11\12\13\15\14,pg6\11\12 
    DHT11_Init();   //��ʪ�ȳ�ʼ��pD6    
    ADCx_Init();    //ADC2��ʼ��pc1��ADC_Channel_11Ŀǰ���ڿ�����⻬��������
    TIM4_Init();        //�������pb8
    //���spi����flash��pa4\5\6\7
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //���� NVIC �жϷ��� 2:2 λ��ռ���ȼ���2 λ��Ӧ���ȼ�
	
    uart_init(115200); 	//���ڳ�ʼ��Ϊ 115200,pa9,pa10
    TIMx_PWM_Init(); 		//LED �˿ڳ�ʼ����������TIM3PWM���pb0,pb1,pb5(TIM3)
	
    KEY_Init(); 		//������ʼ�� pa0,pc13
	Gizwits_Init(); 	//Э����س�ʼ
}
void ILI9341_Showcommon()
{
    printf("--------������ IOT-LED ����ʵ��----------\r\n");
    printf("KEY1:AirLink ����ģʽ\t KEY2:��λ\r\n\r\n");
    
    
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);/* ��������ʾȫ�� */
    
    ILI9341_DispStringLine_EN_CH(LINE(0),"------������ IOT-���ܼҾ�-----");
    ILI9341_DispStringLine_EN_CH(LINE(1),"KEY1:����WIFI KEY2:��λ");
    ILI9341_DispStringLine_EN_CH(LINE(10),"������Χ��20-35��  50-75%RH");
    ILI9341_DispStringLine_EN_CH(LINE(11),"decrease(��),increase(��)");
    ILI9341_DispStringLine_EN_CH(LINE(12),"������ʪ��״��:");
    ILI9341_DisplayStringEx(32,216, 32,32,"No work!",0);
}
void Cloud_KEY()
{
     	u8 key;
        userHandle();		//�û����ݲɼ�
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��,�ϱ���ǰ����
		LCD_SetTextColor(RED);
		key = KEY_Scan(0);
        if(key==KEY1_PRES)	//KEY1 ����
        {
            printf("WIFI ���� AirLink ����ģʽ\r\n");
            delay_ms(20);
            ILI9341_DispStringLine_EN_CH(LINE(2),"WIFI ���� AirLink ����ģʽ");
            gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link ģʽ����
        }
        if(key==KEY2_PRES)	//KEY2 ����
        { 
            printf("WIFI ��λ����������������\r\n");
            LCD_ClearLine(LINE(2));
            LCD_ClearLine(LINE(3));/* ����������� */
            ILI9341_DispStringLine_EN_CH(LINE(2),"WIFI ��λ����������������");
            gizwitsSetMode(WIFI_RESET_MODE);//WIFI ��λ
        }
}
int main(void)
{  
    System_Init();
	ILI9341_Showcommon();////TFT��ˢ�º㶨��ʾ����
	while(1)
    {
        Cloud_KEY();//�����ư���������Э�鴦��
    }
}

 


