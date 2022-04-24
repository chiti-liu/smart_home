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

///* ʹ�ñ�׼�Ĺ̼������IO*/
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


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
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
 
//�ж����
//#define   BRE_TIMx_IRQn                TIM3_IRQn             
//#define   BRE_TIMx_IRQHandler          TIM3_IRQHandler

//ʱ��
#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
#define   BRE_TIM_CLK                   RCC_APB1Periph_TIM3
#define   BRE_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

//��Ƶ�������Ҫ��ӳ��
#define   BRE_GPIO_REMAP_FUN()						GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 				

/************���***************/
#define  BRE_RED_TIM_LED_PORT               GPIOB
#define  BRE_RED_TIM_LED_PIN                GPIO_Pin_5

#define  BRE_RED_TIM_OCxInit                TIM_OC2Init            //ͨ����ʼ������
#define  BRE_RED_TIM_OCxPreloadConfig       TIM_OC2PreloadConfig //ͨ���������ú���

//ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
//�Ժ��װ��ʹ��������ʽ��BRE_TIMx->BRE_RED_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
#define  BRE_RED_CCRx                       	CCR2		

/************�̵�***************/

#define  BRE_GREEN_TIM_LED_PORT               GPIOB
#define  BRE_GREEN_TIM_LED_PIN                GPIO_Pin_0

#define  BRE_GREEN_TIM_OCxInit                TIM_OC3Init            //ͨ����ʼ������
#define  BRE_GREEN_TIM_OCxPreloadConfig       TIM_OC3PreloadConfig //ͨ���������ú���

//ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
//�Ժ��װ��ʹ��������ʽ��BRE_TIMx->BRE_GREEN_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
#define  BRE_GREEN_CCRx                       CCR3

/************����***************/
#define   BRE_BLUE_TIM_LED_PORT             GPIOB
#define   BRE_BLUE_TIM_LED_PIN              GPIO_Pin_1

#define   BRE_BLUE_TIM_OCxInit              TIM_OC4Init            //ͨ����ʼ������
#define   BRE_BLUE_TIM_OCxPreloadConfig    TIM_OC4PreloadConfig  //ͨ���������ú���

//ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
//�Ժ��װ��ʹ��������ʽ��BRE_TIMx->BRE_BLUE_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
#define   BRE_BLUE_CCRx                      CCR4


void TIMx_PWM_Init(void);

		 				    
#endif
