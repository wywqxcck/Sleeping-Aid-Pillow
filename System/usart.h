#ifndef   _USART_H
#define   _USART_H
#include "stm32f10x.h"
#include <stdio.h>

#define  RINGBUFF_LEN  		200  	//定义最大接收字节数 200

/*  设置三个串口的波特率  */
void usart_init(u32 val_1);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif

