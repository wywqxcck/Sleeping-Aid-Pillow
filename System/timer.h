#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"
void TIM1_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr, u16 psc);
#endif
