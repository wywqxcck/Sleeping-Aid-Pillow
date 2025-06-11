#include "stm32f10x.h"                  // Device header
#include "APP.h"

int main(void)
{
	APP_Init();
	while (1)
	{
		APP_Run();
	}
}
