#ifndef _USATR2_H
#define _USATR2_H

#include "stm32f10x.h"
#define USART2_LEN  			88  	//�����������ֽ��� 
extern u8 USART2_BUF[USART2_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern u8 finish_Flag;
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void usart2_init(u32 bound);

#endif

