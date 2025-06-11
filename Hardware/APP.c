#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "Delay.h"
#include "OLED.h"
#include "Buzzer.h"
#include "LED.h"
#include "Key.h"
#include "Power.h"
#include "timer.h"
#include "DHT11.h"
#include "Wifi.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "Massage.h"
#include "Sound.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
u8 temp,hum; 
uint8_t xue;
uint8_t heart;
uint8_t RxData;
dataPoint_t currentDataPoint;
extern u8 wifi_flag;
u8 sound_num;
void Gizwits_Init(void)
{
    TIM4_Int_Init(9, 7199);
    usart3_init(9600); // wifi初始化 波特率必须为9600
    memset((uint8_t *)&currentDataPoint, 0, sizeof(dataPoint_t));
    gizwitsInit();
}

void APP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	TIM1_Int_Init(9,7199);
	KEY_GPIO_Init();
	button_init(&btn1, read_button1_GPIO, 0);
	button_attach(&btn1, PRESS_REPEAT,BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
	button_start(&btn1);
	PowerOut_Init();
	Power3_ON();
	Power5_ON();
	Buzzer_Init();
	Gizwits_Init();
	Wifi_Init();
//	gizwitsSetMode(WIFI_AIRLINK_MODE);
	OLED_Init();
	LED_Init();	
	LED3_OFF();
	Sound_Init();
	DHT11_Init();
	Massage_Init();
	usart_init(9600);
	usart2_init(38400);
	Usart2_SendByte(USART2, 0x8a);
	
}

void userHandle(void)
{
    currentDataPoint.valueLamp = GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0);//Add Sensor Data Collection
    currentDataPoint.valueMassage = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11);//Add Sensor Data Collection
//    currentDataPoint.valueSleep = ;//Add Sensor Data Collection
    currentDataPoint.valuetemp = temp;//Add Sensor Data Collection
    currentDataPoint.valuehumi = hum;//Add Sensor Data Collection
    currentDataPoint.valueHeart_rate = heart;//Add Sensor Data Collection
    currentDataPoint.valueBlood_oxygen = xue;//Add Sensor Data Collection
    currentDataPoint.valueSnoring = sound_num;//Add Sensor Data Collection 
}
//void key_scanf(void)
//{
//	if(WIFI1 == RESET)
//	{
//		Delay_ms(10);
//		if(WIFI1 == RESET)
//		{
//			LED3_ON();
//			gizwitsSetMode(WIFI_AIRLINK_MODE);
//		}
//		while(!WIFI1);
//	}
//}
void Read_DHT11(void)
{
	DHT11_Read_Data(&temp,&hum);
}

void Voice_deal(void)
{
	if(Serial_GetRxFlag() == 1)
	{
		RxData = Serial_GetRxData();
		if(RxData == 0x01)
		{
			LED1_ON();             //打开灯
			LED2_ON();
		}
		else if(RxData == 0x02)
		{
			LED1_OFF();				//关闭灯
			LED2_OFF();
		}
		if(RxData == 0x03)
		{
			Massage_ON();
		}
		else if(RxData == 0x04)
		{
			Massage_OFF();
		}
	}
}
void Soil_deal(void)
{
	if(heart < 50 && heart != 0)
	{
		Serial_SendByte(0XC1);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
	   {
		sound_num++;
	   }
	    currentDataPoint.valueSleep = 2;
	}
	else if(heart > 80 || heart == 0)
	{
		 currentDataPoint.valueSleep = 0;
	}
	else 
	{
		currentDataPoint.valueSleep = 1;
	}
	

}
void OLED_Show(void)
{
	char arr[100];
	sprintf(arr,"T=%d H=%d",temp,hum);
	OLED_ShowString(0,0,(u8*)arr,16);
	sprintf(arr,"x=%d",xue);
	OLED_ShowString(0,2,(u8*)arr,16);
	sprintf(arr,"h=%.3d",heart);
	OLED_ShowString(0,4,(u8*)arr,16);
	sprintf(arr,"S=%d",sound_num);
	OLED_ShowString(0,6,(u8*)arr,16);
}
void APP_Run(void)
{
//	key_scanf();
	if(finish_Flag)
		{
			finish_Flag=0;
			xue = USART2_BUF[66];//血氧数据
			heart = USART2_BUF[65];//心率数据
		}
	Read_DHT11();
	Voice_deal();
	Soil_deal();
	userHandle();
	OLED_Show();
	gizwitsHandle((dataPoint_t *)&currentDataPoint);
}
