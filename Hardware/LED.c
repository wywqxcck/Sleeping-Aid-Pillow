#include "stm32f10x.h"                  // Device header

void LED_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ��GPIOB��GPIOC��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    // ����PB0Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ����PB1Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ����PC13Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // ��ʼ״̬LEDϨ��
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED1_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

void LED1_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void LED2_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void LED2_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void LED3_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED3_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
