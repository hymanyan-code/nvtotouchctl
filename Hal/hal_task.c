#include "M0518.h"
#include "hal_task.h"
#include "hal_led.h"
#include "hal_timer.h"
#include "hal_beep.h"
#include "hal_io.h"
#include "hal_rs485.h"
#include "mb.h"
void hal_task_init(void)
{
	hal_MatrixtimerInit();	
	hal_beep_init();	
	hal_LedInit();
	hal_io_init();
	hal_beep_on();
	HalRs485Init();

	hal_CreatTimer(T_BEEP,hal_beep_off, 40000, T_STA_START); //2 seconds
	hal_CreatTimer(T_INPUT,hal_handle_input_1ms_loop, 20, T_STA_START); //1ms		
	hal_CreatTimer(T_OUTPUT,hal_handle_output_1ms_loop, 20, T_STA_START); //1ms
}

void hal_task(void)
{
//	hal_GetTemHumProc();
    hal_LedHandle();
	( void )eMBPoll(  );
}



