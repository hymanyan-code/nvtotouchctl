#ifndef _HAL_LED_H
#define _HAL_LED_H

#define LED_PORT			PB
#define LED_PIN			    BIT5
#define LED_PORT_PIN	    PB5

void hal_LedInit(void);
void hal_LedHandle(void);

#endif

