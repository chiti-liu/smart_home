#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

///* 使用标准的固件库控制IO*/
//#define LED1(a)	if (a)	\
//					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

//#define LED2(a)	if (a)	\
//					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

//#define LED3(a)	if (a)	\
//					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
//					else		\
//					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		 digitalToggle(BRE_RED_TIM_LED_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(BRE_RED_TIM_LED_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(BRE_RED_TIM_LED_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define   BRE_TIMx                      TIM3
 
//中断相关
//#define   BRE_TIMx_IRQn                TIM3_IRQn             
//#define   BRE_TIMx_IRQHandler          TIM3_IRQHandler

//时钟
#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
#define   BRE_TIM_CLK                   RCC_APB1Periph_TIM3
#define   BRE_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

//红灯的引脚需要重映射
#define   BRE_GPIO_REMAP_FUN()						GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 				

/************红灯***************/
#define  BRE_RED_TIM_LED_PORT               GPIOB
#define  BRE_RED_TIM_LED_PIN                GPIO_Pin_5

#define  BRE_RED_TIM_OCxInit                TIM_OC2Init            //通道初始化函数
#define  BRE_RED_TIM_OCxPreloadConfig       TIM_OC2PreloadConfig //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：BRE_TIMx->BRE_RED_CCRx ，可访问该通道的比较寄存器
#define  BRE_RED_CCRx                       	CCR2		

/************绿灯***************/

#define  BRE_GREEN_TIM_LED_PORT               GPIOB
#define  BRE_GREEN_TIM_LED_PIN                GPIO_Pin_0

#define  BRE_GREEN_TIM_OCxInit                TIM_OC3Init            //通道初始化函数
#define  BRE_GREEN_TIM_OCxPreloadConfig       TIM_OC3PreloadConfig //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：BRE_TIMx->BRE_GREEN_CCRx ，可访问该通道的比较寄存器
#define  BRE_GREEN_CCRx                       CCR3

/************蓝灯***************/
#define   BRE_BLUE_TIM_LED_PORT             GPIOB
#define   BRE_BLUE_TIM_LED_PIN              GPIO_Pin_1

#define   BRE_BLUE_TIM_OCxInit              TIM_OC4Init            //通道初始化函数
#define   BRE_BLUE_TIM_OCxPreloadConfig    TIM_OC4PreloadConfig  //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：BRE_TIMx->BRE_BLUE_CCRx ，可访问该通道的比较寄存器
#define   BRE_BLUE_CCRx                      CCR4


void TIMx_PWM_Init(void);

		 				    
#endif
