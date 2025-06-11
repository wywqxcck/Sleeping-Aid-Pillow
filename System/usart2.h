#ifndef _USATR2_H
#define _USATR2_H

#include "stm32f10x.h"
#define USART2_LEN  			88  	//定义最大接收字节数 
extern u8 USART2_BUF[USART2_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern u8 finish_Flag;
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void usart2_init(u32 bound);

#endif

