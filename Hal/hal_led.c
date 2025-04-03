#include "M0518.h"
#include "hal_led.h"
#include <stdint.h>
static void hal_LedConfig(void);
static void hal_Led1Turn(void);
uint8_t array[5] = {0x01, 0x33, 0x44, 0x55, 0x10};
void hal_LedInit(void)
{
    hal_LedConfig();
}

void hal_LedHandle(void)
{
    static unsigned char i = 0;

    if (i++ > 100)
    {
        hal_Led1Turn();
        i = 0;
    }
}


static void hal_LedConfig(void)
{
    GPIO_SetMode(PA, BIT8, GPIO_PMD_OUTPUT); // led
    PA8 = 1;								 // OFF
}

////

static void hal_Led1Turn(void)
{
    static unsigned char i = 0;

    if (i == 0)
    {
        PA8 = 0; // ON
        i = 1;
    }
    else
    {
        PA8 = 1; // OFF
        i = 0;
    }
}
