#ifndef __SERVO_H
#define __SERVO_H	 
#include "sys.h"


//#define  BRE_SERVO_TIM_PORT               GPIOB
//#define  BRE_SERVO_TIM_PIN                GPIO_Pin_9

//#define  BRE_SERVO_TIM_OCxInit                TIM_OC1Init            //ͨ����ʼ������
//#define  BRE_SERVO_TIM_OCxPreloadConfig       TIM_OC1PreloadConfig //ͨ���������ú���

////ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
////�Ժ��װ��ʹ��������ʽ��BRE_TIMx->BRE_RED_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
//#define  BRE_SERVO_CCRx                       	CCR4		
void Servo_Control(uint16_t angle);
void TIM4_Init(void);
#endif
