#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	   	    
//LED IO初始化
 static void TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(BRE_TIM_GPIO_CLK, ENABLE); 
//绿
    
	GPIO_InitStructure.GPIO_Pin =  BRE_GREEN_TIM_LED_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BRE_GREEN_TIM_LED_PORT, &GPIO_InitStructure);
	
	//蓝
	GPIO_InitStructure.GPIO_Pin =  BRE_BLUE_TIM_LED_PIN ;
    GPIO_Init(BRE_BLUE_TIM_LED_PORT, &GPIO_InitStructure);
  	/*部分重映射*/
	BRE_GPIO_REMAP_FUN();
  /* 配置LED灯用到的引脚 */
	//红
    GPIO_InitStructure.GPIO_Pin =  BRE_RED_TIM_LED_PIN ;	
    GPIO_Init(BRE_RED_TIM_LED_PORT, &GPIO_InitStructure);
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);//关闭部分重映射
//	
    /* 关闭所有led灯	*/
		GPIO_SetBits(BRE_RED_TIM_LED_PORT, BRE_RED_TIM_LED_PIN );
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(BRE_GREEN_TIM_LED_PORT, BRE_GREEN_TIM_LED_PIN );	 
    
    /* 关闭所有led灯	*/
		GPIO_SetBits(BRE_BLUE_TIM_LED_PORT, BRE_BLUE_TIM_LED_PIN);
}


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
//static void NVIC_Config_PWM(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//  
//  /* 配置TIM3_IRQ中断为中断源 */
//  NVIC_InitStructure.NVIC_IRQChannel = BRE_TIMx_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

/**
  * @brief  配置TIM输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */

static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	
	/* 设置TIMCLK 时钟 */
	BRE_TIM_APBxClock_FUN ( BRE_TIM_CLK, ENABLE ); 
	
  TIM_TimeBaseStructure.TIM_Period = (100-1);       							  //当定时器从0计数到 TIM_Period+1 为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = (720-1);	    							//设置预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
  TIM_TimeBaseInit(BRE_TIMx, &TIM_TimeBaseStructure);

  /* PWM模式配置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;				 						  			//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR_Val时为低电平

 
	//RGB通道
  BRE_RED_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	
  BRE_RED_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//使能预装载	

  BRE_GREEN_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	
  BRE_GREEN_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//使能预装载	

  BRE_BLUE_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	
  BRE_BLUE_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//使能预装载	

  TIM_ARRPreloadConfig(BRE_TIMx, ENABLE);			 										//使能TIM重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(BRE_TIMx, ENABLE);                   										//使能定时器	
	
//	TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);										//使能update中断
		
//	NVIC_Config_PWM();																					//配置中断优先级		
	
}

/**
  * @brief  TIM 呼吸灯初始化
  *         配置PWM模式和GPIO
  * @param  无
  * @retval 无
  */
void TIMx_PWM_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}
