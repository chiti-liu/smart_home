#include "servo.h"
void Servo_Control(uint16_t angle)
{
	int temp;
	temp = (int)((1.0 / 9.0) * angle) + 5;//�Ĵ���ֵ = 1/9 * �Ƕ� + 5
	TIM_SetCompare3(TIM4, temp);//���Ƕ�ֵд��ȽϼĴ���
}


void TIM4_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB����ʱ��ʹ��
	                                                                         	
   //���ø�����Ϊ�����������,���TIM4��CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM4_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/****************** 72MHz / 200 / 7200 = 50Hz*******************/
	TIM_TimeBaseStructure.TIM_Period = 200 - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	  	//���ͨ��
  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_Pulse = 0;//��ʼ״̬ռ�ձ�Ϊ0
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ��Ԥװ��	
    TIM_ARRPreloadConfig(TIM4, ENABLE);			 										//ʹ��TIM���ؼĴ���ARR
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4����	
}
