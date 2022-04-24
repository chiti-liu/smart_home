#ifndef __SERVO_H
#define __SERVO_H	 
#include "sys.h"


//#define  BRE_SERVO_TIM_PORT               GPIOB
//#define  BRE_SERVO_TIM_PIN                GPIO_Pin_9

//#define  BRE_SERVO_TIM_OCxInit                TIM_OC1Init            //通道初始化函数
//#define  BRE_SERVO_TIM_OCxPreloadConfig       TIM_OC1PreloadConfig //通道重载配置函数

////通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
////以宏封装后，使用这种形式：BRE_TIMx->BRE_RED_CCRx ，可访问该通道的比较寄存器
//#define  BRE_SERVO_CCRx                       	CCR4		
void Servo_Control(uint16_t angle);
void TIM4_Init(void);
#endif
