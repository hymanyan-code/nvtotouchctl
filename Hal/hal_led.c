#include "M0518.h"
#include "hal_led.h"
#include <stdint.h>
#include <stdio.h>
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
    GPIO_SetMode(LED_PORT, LED_PIN, GPIO_PMD_OUTPUT); // led
    LED_PORT_PIN = 0;								 // OFF
}

////

static void hal_Led1Turn(void)
{
    static unsigned char i = 0;
	  //printf("LED turn!\n");
    if (i == 0)
    {
        LED_PORT_PIN = 0; // ON
				//PB5 = 0;
        i = 1;
    }
    else
    {
        LED_PORT_PIN = 1; // OFF
			//PB5 = 1;
        i = 0;
    }
}
