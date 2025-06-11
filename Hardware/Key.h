#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_

#include "stdint.h"
#include "string.h"
#include "stm32f10x.h"


//According to your need to modify the constants.
#define TICKS_INTERVAL 5 //ms，系统节拍5ms
#define DEBOUNCE_TICKS 5 //MAX 8，去抖的次数
#define SHORT_TICKS (300 / TICKS_INTERVAL)	//单击短按键的时间是300ms以上
#define LONG_TICKS (1000 / TICKS_INTERVAL)	//长按键的时间是1000ms以上
#define LONG_HOLD_CYC (500 / TICKS_INTERVAL)	//处于长按键保持触发状态时，每500ms调用一次回调函数
#define KEY_NO					0
#define KEY_ENTER				1
#define KEY_RETURN			    2

//按键相关设置
#define GPIOX 										GPIOA
#define RCC_APB2Periph_GPIOX      (RCC_APB2Periph_GPIOA)
#define GPIO_Pin_X								(GPIO_Pin_15)


typedef void (*BtnCallback)(void *);

//声明一个按键事件的枚举类型，包括了所有的按键操作类型
typedef enum
{
	PRESS_DOWN = 0,	//按键按下，每次按下都触发 
	PRESS_UP,		//按键弹起，每次松开都触发
	SINGLE_CLICK,	//单击按键事件
	DOUBLE_CLICK,	//双击按键事件
	PRESS_REPEAT,	//三击按键事件
	LONG_PRESS_START,	//达到长按时间阈值时触发一次
	LONG_PRESS_HOLD,	//长按期间一直触发
	number_of_event,	//事件数量
	NONE_PRESS		//没有任何按键事件
} PressEvent;

//声明一个按键结构体类型（链表）
typedef struct Button
{
	uint16_t ticks;			//系统节拍计数
	uint8_t repeat : 4;		//重复按键，双击、三击……
	uint8_t event : 4;		//当前按键事件
	uint8_t state : 3;		//当前按键状态
	uint8_t debounce_cnt : 3;	//去抖次数	
	uint8_t active_level : 1;	//按键按下的有效电平			
	uint8_t button_level : 1;	//按键电平状态，获取一次更新一次
	uint8_t (*hal_button_Level)(void);	//获取按键电平的函数指针
	BtnCallback cb[number_of_event];	//按键回调函数指针数组
	struct Button *next;	//指向链表的下一个
} Button;

extern Button btn1;



#ifdef __cplusplus
extern "C"
{
#endif
	//按键结构体初始化，赋予按键获取电平的函数指针，并读出当前电平
	void button_init(struct Button *handle, uint8_t (*pin_level)(), uint8_t active_level);
	//按键注册（赋予特定的按键事件以回调函数）
	void button_attach(struct Button *handle, PressEvent event, BtnCallback cb);
	//获取按键事件返回到按键结构体
	PressEvent get_button_event(struct Button *handle);
	//启动一个按键，即新增一个按键结构体链表节点
	int button_start(struct Button *handle);
	//停止一个按键，即从链表中删除该按键节点
	void button_stop(struct Button *handle);
	//按键节拍，每一次节拍事件遍历按键列表中所有按键，调用驱动函数
	void button_ticks(void);
	//获取按键状态
	uint8_t read_button1_GPIO(void);
	//按键引脚初始化
	void KEY_GPIO_Init(void);
	//单击回调函数
	void BTN1_SINGLE_Click_Handler(void* btn);
	//双击回调函数
	void BTN1_DOUBLE_Click_Handler(void* btn);
	//三连击回74调函数
	void BTN1_PRESS_REPEAT_Handler(void* btn);
	//长按回调函数
	void BTN1_LONG_PRESS_START_Handler(void* btn);
#ifdef __cplusplus
}
#endif

#endif
