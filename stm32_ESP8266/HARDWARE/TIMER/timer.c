#include "timer.h"
#include "gizwits_product.h"
#include "dht11.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}


float ADC_ConvertedValueLocal=0;//����ADC��ѹֵ
u8 temperature=0,humidity=0;//��ʪ��
// ȫ�ֱ��������ڱ���ת�������ĵ�ѹֵ 

void LCD_Show_Situation(void)
{
    char dispBuff1[100],dispBuff2[100];
    int Result = 0;
    LCD_SetFont(&Font8x16);
    LCD_SetTextColor(RED);
    
    Result= ADC_GetConversionValue(ADC_x);
    ADC_ConvertedValueLocal =(float) Result/4096*3.3; 
	sprintf(dispBuff1,"��ǰ����ADת����ѹ��%.2f ",ADC_ConvertedValueLocal);
    
    if(ADC_ConvertedValueLocal >=0&&ADC_ConvertedValueLocal <0.8)//���
    {
        LCD_SetTextColor(RED);
        ILI9341_DisplayStringEx(160,84, 32,32,"����",0);
    }
    else if(ADC_ConvertedValueLocal >=0.8&&ADC_ConvertedValueLocal <1.6)
    {
        LCD_SetTextColor(YELLOW);
        ILI9341_DisplayStringEx(160,84, 32,32,"С��",0);
    }
    else if(ADC_ConvertedValueLocal >=1.6&&ADC_ConvertedValueLocal <2.4)
    {
        LCD_SetTextColor(BLUE);
       ILI9341_DisplayStringEx(160,84, 32,32,"����",0);
    }
    else
    {
        LCD_SetTextColor(WHITE);
        ILI9341_DisplayStringEx(160,84, 32,32,"����",0);
        
    }
    
//    LCD_ClearLine(LINE(4));
    ILI9341_DispStringLine_EN_CH(LINE(4),dispBuff1);

    LCD_SetTextColor(RED);
    ILI9341_DispStringLine_EN_CH(LINE(7),"����һ���ֽ���--------");
    
    DHT11_Read_Data(&temperature,&humidity); 	//��ȡ��ʪ��ֵ
    if(humidity>75||temperature>35)
        {
            LCD_SetTextColor(WHITE);
        }
        else if(humidity<50||temperature<20)
        {
            LCD_SetTextColor(BLUE);
        }
        else 
        {
            LCD_SetTextColor(RED);
        }
    sprintf(dispBuff2,"�¶�:%d��  ʪ��:%d %%RH",temperature,humidity);
//    LCD_ClearLine(LINE(9));
    ILI9341_DispStringLine_EN_CH(LINE(9),dispBuff2);
//	printf("�¶�:%d.%d��\r\nʪ��:%d.%d%%RH\r\n",DHT11_Data.temp_int, DHT11_Data.temp_deci,DHT11_Data.humi_int, DHT11_Data.humi_deci);

	
}

u8 count=0;//��ʱ
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
        static u16 cnt=0;
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		}
		gizTimerMs();//ϵͳ���붨ʱ
        cnt++;
        if(cnt>1000)//1S
        {
            count++;
            LCD_Show_Situation();//��ʪ�Ȳɼ�������״̬Ҳ����������
            cnt=0;  
        }
//        if(count==6)//6�����һ��
//        {
//             LCD_Show_Situation();
//        }
}









