#include "M0518.h"
#include "hal_beep.h"

void hal_beep_init(void)
{
	GPIO_SetMode(PD, BIT6, GPIO_PMD_OUTPUT);    //beep
    PD6 = 0;

}


void hal_beep_on(void)
{
	PD6 = 1;
}

void hal_beep_off(void)
{
	PD6 = 0;
}

