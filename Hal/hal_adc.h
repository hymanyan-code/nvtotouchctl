#ifndef ___HAL_ADC_H_
#define ___HAL_ADC_H_

void HalAdcInit(void);

#define ACD_BUF_LEN       (10)



typedef enum
{
    ADC_CH1,
    ADC_CH2,
    ADC_CH_MAX
}ACD_CHANNEL;

typedef void (*AdcDataPro_Callback_t)(uint16_t data);


void HalAdcConverteFlow(void);
void HalAdc1Callback(uint16_t data);


extern uint16_t adc_average[ADC_CH_MAX];

#endif
