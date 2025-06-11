#include "stm32f10x.h"                  // Device header

void PowerOut_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ��GPIOBʱ�� 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);

    // ����PB5Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // ��ʼ״̬��POWER_OUTΪ�͵�ƽ
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}
void Power3_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}
void Power3_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}
void Power5_ON(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}
void Power5_OFF(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}
