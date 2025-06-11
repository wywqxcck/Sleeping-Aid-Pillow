#include "key.h"
#include "Power.h"

Button btn1;




uint8_t read_button1_GPIO(void)
{
		return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
}

void KEY_GPIO_Init(void)
{
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX,ENABLE);//使能PORTA,PORTE时钟
	
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_X;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOX, &GPIO_InitStructure);
}

void BTN1_SINGLE_Click_Handler(void* btn)//单击回调函数
{
	
}

void BTN1_DOUBLE_Click_Handler(void* btn)//双击回调函数
{
	Power3_OFF();
	Power5_OFF();
}

void BTN1_PRESS_REPEAT_Handler(void* btn)//三连击回调函数
{
	
}

void BTN1_LONG_PRESS_START_Handler(void* btn)//长按回调函数
{
	
}


#define EVENT_CB(ev)    \
	if (handle->cb[ev]) \
	handle->cb[ev]((Button *)handle)

//button handle list head.
//按键结构体链表，初始化指针为NULL，空链表
static struct Button *head_handle = NULL;

/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @retval None
  */
 //按键结构体初始化，赋予按键获取电平的函数指针，并读出当前电平
void button_init(struct Button *handle, uint8_t (*pin_level)(), uint8_t active_level)
{
		memset(handle, 0, sizeof(struct Button));		   //结构体初始化为全0
		handle->event = (uint8_t)NONE_PRESS;			   //初始化为没有按键事件
		handle->hal_button_Level = pin_level;			   //读取按键电平状态函数指针
		handle->button_level = handle->hal_button_Level(); //读取一次当前按键电平
		handle->active_level = active_level;			   //按下的有效电平是1还是0
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
 //按键注册（赋予特定的按键事件以回调函数）
void button_attach(struct Button *handle, PressEvent event, BtnCallback cb)
{
	handle->cb[event] = cb; //赋予某特定按键事件的回调函数指针
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
 //获取按键事件返回到按键结构体
PressEvent get_button_event(struct Button *handle)
{
	return (PressEvent)(handle->event); //返回按键事件
}

/**
  * @brief  Button driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @retval None
  */
//按键驱动核心函数，驱动状态机
void button_handler(struct Button *handle)
{
	//读取当前电平状态
	uint8_t read_gpio_level = handle->hal_button_Level();

	//ticks counter working..
	//只要按键状态不是0，就递增记录系统节拍次数
	if ((handle->state) > 0)
		handle->ticks++;

	/*------------button debounce handle---------------*/
	//如果这一次获取的电平状态与上一次状态不符，则连读几次去抖才能赋值当前电平
	if (read_gpio_level != handle->button_level)
	{ //not equal to prev one
		//continue read 3 times same new level change
		if (++(handle->debounce_cnt) >= DEBOUNCE_TICKS)
		{
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	}
	//否则不用去抖检测处理
	else
	{ //leved not change ,counter reset.
		handle->debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (handle->state)
	{
	case 0: //如果前次按键状态是0，且当前按键电平等于有效电平，则认为是按键开始按下了
		if (handle->button_level == handle->active_level)
		{										 //start press down
			handle->event = (uint8_t)PRESS_DOWN; //有按键按下事件发生
			EVENT_CB(PRESS_DOWN);				 //如果存在回调函数则执行
			handle->ticks = 0;					 //节拍计数清零
			handle->repeat = 1;					 //事件重复次数为1
			handle->state = 1;					 //按键状态更新为1（按下）
		}
		else //如果不是有效电平，则无按键事件
		{
			handle->event = (uint8_t)NONE_PRESS;
		}
		break;

	case 1: //如果前次按键状态是1（按下了），且当前电平不是有效电平，则认为按键松开了
		if (handle->button_level != handle->active_level)
		{									   //released press up
			handle->event = (uint8_t)PRESS_UP; //有按键松开弹起事件发生
			EVENT_CB(PRESS_UP);				   //如果存在回调函数则执行
			handle->ticks = 0;				   //节拍计数清零
			handle->state = 2;				   //按键状态更新为2（松开）
		}
		//如果节拍计数超过了长按键阀值，则认为是一次长按键事件
		else if (handle->ticks > LONG_TICKS)
		{
			handle->event = (uint8_t)LONG_PRESS_START; //有长按键开始事件发生
			EVENT_CB(LONG_PRESS_START);				   //如果存在回调函数则执行
			handle->state = 5;						   //按键状态更新为5（长按开始）
		}
		break;

	case 2: //如果前次按键状态是2（松开了），且当前电平是有效电平，则认为按键又按下了
		if (handle->button_level == handle->active_level)
		{										 //press down again
			//handle->event = (uint8_t)PRESS_DOWN; //有按键按下事件发生
			EVENT_CB(PRESS_DOWN);				 //如果存在回调函数则执行
			handle->repeat++;					 //又一次按下，重复次数加1
			handle->ticks = 0;					 //节拍计数清零
			handle->state = 3;					 //按键状态更新为3（重复按键）
		}
		//如果不是有效电平，说明松开了，那么检查摁下的时间是否超过了短按键的节拍计数阀值
		else if (handle->ticks > SHORT_TICKS)
		{ //released timeout
			//超过阀值，且只按下了1次，则认为是一次单击事件
			if (handle->repeat == 1)
			{
				handle->event = (uint8_t)SINGLE_CLICK; //有单击事件发生
				EVENT_CB(SINGLE_CLICK);				   //如果存在回调函数则执行
			}
			//如果按下了2次，则认为是一次双击事件
			else if (handle->repeat == 2)
			{
				handle->event = (uint8_t)DOUBLE_CLICK; //有双击事件发生
				EVENT_CB(DOUBLE_CLICK);				   // repeat hit 如果存在回调函数则执行
			}
			else if (handle->repeat == 3)
			{
				handle->event = (uint8_t)PRESS_REPEAT; //有三击事件发生
				EVENT_CB(PRESS_REPEAT);				   // repeat hit 如果存在回调函数则执行
			}
			handle->state = 0; //按键状态更新为0（未按下）
		}
		break;

	case 3: //如果前次按键状态是3（重复按下），且当前电平不是有效电平，则认为是按键弹起或短按键事件
		if (handle->button_level != handle->active_level)
		{									   //released press up
			handle->event = (uint8_t)PRESS_UP; //有按键弹起事件发生
			EVENT_CB(PRESS_UP);				   //如果存在回调函数则执行
			//如果记录的节拍时间小于300ms，
			if (handle->ticks < SHORT_TICKS)
			{
				handle->ticks = 0; //节拍计数清零
				handle->state = 2; //repeat press按键状态更新为2（松开）
			}
			else
			{
				handle->state = 0; //按键状态更新为0（未按下）
			}
		}
		break;

	case 5: //如果前次按键状态是5（长按键开始），且当前电平是有效电平，则认为处于长按键保持出发状态
		if (handle->button_level == handle->active_level)
		{
			//continue hold trigger
			handle->event = (uint8_t)LONG_PRESS_HOLD; //有按键弹起事件发生
			if (handle->ticks % LONG_HOLD_CYC == 0)	//每500ms调用一次回调函数
			{
				EVENT_CB(LONG_PRESS_HOLD); //如果存在回调函数则执行
			}
		}
		else								   //如果不是有效电平，则认为是按键弹起事件
		{									   //releasd
			handle->event = (uint8_t)PRESS_UP; //有按键弹起事件发生
			EVENT_CB(PRESS_UP);				   //如果存在回调函数则执行
			handle->state = 0;				   //reset按键状态复位
		}
		break;
	}
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
 //启动一个按键，即新增一个按键结构体链表节点
int button_start(struct Button *handle)
{
	struct Button *target = head_handle;
	while (target)
	{
		if (target == handle)
			return -1; //already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
 //停止一个按键，即从链表中删除该按键节点
void button_stop(struct Button *handle)
{
	struct Button **curr;
	for (curr = &head_handle; *curr;)
	{
		struct Button *entry = *curr;
		if (entry == handle)
		{
			*curr = entry->next;
			//			free(entry);
		}
		else
			curr = &entry->next;
	}
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
 //按键节拍，每一次节拍事件遍历按键列表中所有按键，调用驱动函数
void button_ticks()
{
	struct Button *target;
	for (target = head_handle; target; target = target->next)
	{
		button_handler(target);
	}
}
