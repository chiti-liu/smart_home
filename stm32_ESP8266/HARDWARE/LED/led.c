#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	   	    
//LED IO��ʼ��
 static void TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(BRE_TIM_GPIO_CLK, ENABLE); 
//��
    
	GPIO_InitStructure.GPIO_Pin =  BRE_GREEN_TIM_LED_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BRE_GREEN_TIM_LED_PORT, &GPIO_InitStructure);
	
	//��
	GPIO_InitStructure.GPIO_Pin =  BRE_BLUE_TIM_LED_PIN ;
    GPIO_Init(BRE_BLUE_TIM_LED_PORT, &GPIO_InitStructure);
  	/*������ӳ��*/
	BRE_GPIO_REMAP_FUN();
  /* ����LED���õ������� */
	//��
    GPIO_InitStructure.GPIO_Pin =  BRE_RED_TIM_LED_PIN ;	
    GPIO_Init(BRE_RED_TIM_LED_PORT, &GPIO_InitStructure);
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);//�رղ�����ӳ��
//	
    /* �ر�����led��	*/
		GPIO_SetBits(BRE_RED_TIM_LED_PORT, BRE_RED_TIM_LED_PIN );
		
		/* �ر�����led��	*/
		GPIO_SetBits(BRE_GREEN_TIM_LED_PORT, BRE_GREEN_TIM_LED_PIN );	 
    
    /* �ر�����led��	*/
		GPIO_SetBits(BRE_BLUE_TIM_LED_PORT, BRE_BLUE_TIM_LED_PIN);
}


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
//static void NVIC_Config_PWM(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//  
//  /* ����TIM3_IRQ�ж�Ϊ�ж�Դ */
//  NVIC_InitStructure.NVIC_IRQChannel = BRE_TIMx_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

/**
  * @brief  ����TIM�����PWM�źŵ�ģʽ�������ڡ�����
  * @param  ��
  * @retval ��
  */

static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	
	/* ����TIMCLK ʱ�� */
	BRE_TIM_APBxClock_FUN ( BRE_TIM_CLK, ENABLE ); 
	
  TIM_TimeBaseStructure.TIM_Period = (100-1);       							  //����ʱ����0������ TIM_Period+1 Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = (720-1);	    							//����Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(BRE_TIMx, &TIM_TimeBaseStructure);

  /* PWMģʽ���� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ�����
  TIM_OCInitStructure.TIM_Pulse = 0;				 						  			//���ó�ʼPWM������Ϊ0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR_ValʱΪ�͵�ƽ

 
	//RGBͨ��
  BRE_RED_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	
  BRE_RED_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

  BRE_GREEN_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	
  BRE_GREEN_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

  BRE_BLUE_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	
  BRE_BLUE_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

  TIM_ARRPreloadConfig(BRE_TIMx, ENABLE);			 										//ʹ��TIM���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(BRE_TIMx, ENABLE);                   										//ʹ�ܶ�ʱ��	
	
//	TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);										//ʹ��update�ж�
		
//	NVIC_Config_PWM();																					//�����ж����ȼ�		
	
}

/**
  * @brief  TIM �����Ƴ�ʼ��
  *         ����PWMģʽ��GPIO
  * @param  ��
  * @retval ��
  */
void TIMx_PWM_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}
