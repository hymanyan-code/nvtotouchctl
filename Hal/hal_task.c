#include "M0518.h"
#include "hal_task.h"
#include "hal_led.h"



void hal_task_init(void)
{
	//hal_timerInit();	
	//hal_GpioConfig_init();		
	hal_LedInit();
}

void hal_task(void)
{
//	hal_GetTemHumProc();
    hal_LedHandle();
}



