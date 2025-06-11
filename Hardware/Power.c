#include "stm32f10x.h"                  // Device header

void PowerOut_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOB时钟 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PB5为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 初始状态下POWER_OUT为低电平
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
