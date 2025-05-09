#include "M0518.h"
#include "hal_task.h"
#include "hal_led.h"
#include "hal_timer.h"
#include "hal_beep.h"
#include "hal_io.h"
#include "hal_rs485.h"
#include "hal_adc.h"
#include "hal_stepper.h"
#include "hal_iflash.h"



void hal_task_init(void)
{
    HalIflashInit();
    hal_MatrixtimerInit();
    hal_beep_init();
    hal_LedInit();
    hal_io_init();
    HalAdcInit();
    HalStepperInit();

    hal_beep_on();
    hal_CreatTimer(T_BEEP, hal_beep_off, 10000, T_STA_START); //2 seconds

}

void hal_task(void)
{

   hal_handle_input_10ms_loop();
 
    hal_LedHandle();
    HalAdcConverteFlow();
}



