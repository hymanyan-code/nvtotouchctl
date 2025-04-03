#ifndef ___HAL_DAC_H_
#define ___HAL_DAC_H_


#include "stdint.h"
















uint8_t HalDacInit(void);
uint8_t HalDac1SetVoltage(float vol);
uint8_t HalDac2SetVoltage(float vol);








extern uint16_t dac_voltage[2];





#endif
