#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	    


void usart3_init(u32 bound);			//����3��ʼ�� 
void USART_SendStr(USART_TypeDef *USARTx,uint8_t *str);
#endif
