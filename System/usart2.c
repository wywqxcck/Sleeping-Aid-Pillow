/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, temp_duan, China, DY.
**                           All Rights Reserved
**
**                           By(德阳科派科技有限公司)
**                             http://www.kpiot.top
**
**----------------------------------文件信息------------------------------------
** 文件名称: usart2.c
** 创建人员: temp_duan
** 创建日期: 2019-08-01
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "usart2.h"
#include "sys.h"


u8 finish_Flag=0;
u8 USART2_BUF[USART2_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_STA=0;       //接收状态标记	  

/*******************************************************************************
** 函数名称: usart2_init
** 功能描述: 
** 参数说明: bound: [输入/出] 
** 返回说明: None
** 创建人员: temp_duan
** 创建日期: 2019-08-01
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/
void usart2_init(u32 bound)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	                       //GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);                         //串口2时钟使能

    //USART2_TX   PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                     //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                               //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA2

    //USART2_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA3

    USART_InitStructure.USART_BaudRate = bound;                                    //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式

    USART_Init(USART2, &USART_InitStructure);       //初始化串口2


    USART_Cmd(USART2, ENABLE);                      //使能串口

    //使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //开启中断

    //设置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}

void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*******************************************************************************
** 函数名称: USART2_IRQHandler
** 功能描述: 
** 参数说明: None
** 返回说明: None
** 创建人员: temp_duan
** 创建日期: 2019-08-01
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/
void USART2_IRQHandler(void)
{
    u8 Res;
	static u8 flag;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
    {
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Res =USART_ReceiveData(USART2);					//读取接收到的数据
//		Usart3_SendByte(USART3,Res); 
		if (Res==(0xff)) 									//当数据包的首字节为0xff,则继续接收,否则丢弃  
		{
			flag = 1;
		}
		if(flag)
		{
			USART2_BUF[USART2_STA]=Res;					//每次进中断在这里赋值给数组
			USART2_STA++;																		
		}
		if(USART2_STA>(USART2_LEN-1))					//达到76字节时,回到数组第1位
		{
			USART2_STA=0;
			flag= 0;
			finish_Flag=1;
		}
				
    }
}
