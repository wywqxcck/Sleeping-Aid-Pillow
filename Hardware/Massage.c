#include "stm32f10x.h"                  // Device header

void Massage_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);				
}
void Massage_ON(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_11);//高
}
void Massage_OFF(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);	//低	
}
