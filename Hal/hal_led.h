#ifndef _HAL_LED_H
#define _HAL_LED_H

#define LED_PORT			PA
#define LED_PIN			    BIT8


void hal_LedInit(void);
void hal_LedHandle(void);

#endif

