/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, temp_duan, China, DY.
**                           All Rights Reserved
**
**                           By(德阳科派科技有限公司)
**                             http://www.kpiot.top
**
**----------------------------------文件信息------------------------------------
** 文件名称: DHT11.c
** 创建人员: temp_duan
** 创建日期: 2019-07-09
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
/********************************End of File************************************/
#include "stm32f10x.h"
#include "DHT11.h"
void DHT11_IO_IN(void)//温湿度模块输入函数
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = IO_DHT11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_DHT11, &GPIO_InitStructure);
}

void DHT11_IO_OUT(void)//温湿度模块输出函数
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = IO_DHT11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_DHT11, &GPIO_InitStructure);
}

//复位DHT11
void DHT11_Rst(void)
{
    DHT11_IO_OUT(); //SET OUTPUT
    DHT11_DQ_Low; //DQ=0
    Delay_ms(20);    //拉低至少18ms
    DHT11_DQ_High; //DQ=1
    Delay_us(30);     //主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void)
{
    u8 retry = 0; //定义临时变量
    DHT11_IO_IN();//SET INPUT
    while ((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 1) && retry < 100) //DHT11会拉低40~80us
    {
        retry++;
        Delay_us(1);
    };
    if(retry >= 100)return 1;
    else retry = 0;
    while ((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 0) && retry < 100) //DHT11拉低后会再次拉高40~80us
    {
        retry++;
        Delay_us(1);
    };
    if(retry >= 100)return 1;
    return 0;
}
//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void)
{
    u8 retry = 0;
    while((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 1) && retry < 100) //等待变为低电平
    {
        retry++;
        Delay_us(1);
    }
    retry = 0;
    while((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 0) && retry < 100) //等待变高电平
    {
        retry++;
        Delay_us(1);
    }
    Delay_us(40);//等待40us
    if(GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 1)
        return 1;
    else
        return 0;
}
//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;
    DHT11_Rst();
    if(DHT11_Check() == 0)
    {
        for(i = 0; i < 5; i++) //读取40位数据
        {
            buf[i] = DHT11_Read_Byte();
        }
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
    }
    else return 1;
    return 0;
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在
void DHT11_Init(void)
{
    DHT11_Rst();  //复位DHT11
    DHT11_Check();//等待DHT11的回应
}
