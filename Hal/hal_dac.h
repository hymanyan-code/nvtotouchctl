#ifndef ___HAL_DAC_H_
#define ___HAL_DAC_H_


#include "stdint.h"
















uint8_t HalDacInit(void);
uint8_t HalDac1SetVoltage(uint8_t dac_line, float vol);














#endif
