#include "M0518.h"
#include "hal_adc.h"

AdcDataPro_Callback_t adc_ch1_callback,adc_ch2_callback;

uint16_t adc_raw_value[ADC_CH_MAX][ACD_BUF_LEN];
uint16_t adc_average[ADC_CH_MAX];


static uint32_t AdcFilter(uint16_t *p_addr, int num)
{
    int len = num;
    int i = 0;
    int max = 0;
    int min = 4096;
    uint16_t *ptr;
    int sum = 0;
    uint16_t normal = 0;
	uint16_t abnormal = 0;

    ptr = p_addr;
    sum = 0;

    for (i = 0; i < len; i++)
    {
        if (*ptr > max)
        {
            max = *ptr;
        }

        if (*ptr < min)
        {
            min = *ptr;
        }

        sum += *ptr;
        ptr += 1;
    }

    sum = (sum - max - min) / (len - 2);
    ptr = p_addr;

    for (i = 0; i < len / 2; i++)
    {
        if (*ptr < (sum + 50) && *ptr > (sum - 50))
        {
            normal++;
        }
        else
        {
            abnormal++;
        }

        ptr += 1;
    }

    if ((normal / 2) < abnormal)
    {
        sum = -1;
    }

    return sum;
}




void HalAdcInit(void)
{
	ADC_POWER_ON(ADC);
	ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE_CYCLE, 0x60);

}




#include "stdio.h"

void HalAdcConverteFlow(void)
{
	static uint8_t conv_step=0;
	static uint8_t index = 0;
   // static uint8_t printcnt=0;
   // uint32_t u32TimeOutCnt;
	uint8_t i;	
	if(conv_step == 0)
	{
     //  ADC_POWER_ON(ADC);
	 //   ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE_CYCLE, 0xF);
       // ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

		ADC_START_CONV(ADC);
        // u32TimeOutCnt = SystemCoreClock; /* 1 second time-out */
        // while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT))
        // {
        //     if(--u32TimeOutCnt == 0)
        //     {
        //         printf("Wait for ADC conversion done time-out!\n");
        //         return;
        //     }
        // }
       
		conv_step = 1;
       // printf("adc_1\n");
	}
	else if(conv_step == 1)
	{
       // printf("adc_2\n");
       for(i = 0; i < ADC_CH_MAX; i++)
       {
           adc_raw_value[i][index] = ADC_GET_CONVERSION_DATA(ADC,( i+5));
           //printf("adc:%d\n",adc_raw_value[i][index] );
       }
		index =  (index+1)%ACD_BUF_LEN;
		conv_step = 0;
        if(index == 0)
        {
            adc_average[ADC_CH1] =	AdcFilter(adc_raw_value[0],ACD_BUF_LEN);
            adc_average[ADC_CH2] =	AdcFilter(adc_raw_value[1],ACD_BUF_LEN);
            //10*ACD_BUF_LEN = 100ms update	average 
           // printf("adc_3\n");
           // adc_ch1_callback(adc_average[ADC_CH1]);
           // adc_ch2_callback(adc_average[ADC_CH2]);
        //     if(printcnt++>20){
        //         printcnt=0;
        //    printf("\r\nadc1:");
        //     for(uint8_t j=0;j<ACD_BUF_LEN;j++)
        //     {
        //         printf("%d ",adc_raw_value[0][j]);
        //     }
        //     printf("\r\nadc2:");
        //     for(uint8_t j=0;j<ACD_BUF_LEN;j++)
        //     {
        //         printf("%d ",adc_raw_value[1][j]);
        //     }
        //     printf("\r\n");
        // }
            HalAdc1Callback(adc_average[ADC_CH1]);
           // printf("adc1:%d, adc2%d\n",adc_average[ADC_CH1],adc_average[ADC_CH2] );
        }

	}

}


__weak void HalAdc1Callback(uint16_t data)
{

}


void HalAdcCh1CallbackRegister(AdcDataPro_Callback_t pCBS)
{
	if(adc_ch1_callback == 0)
	{
		adc_ch1_callback = pCBS;
	}
}

void HalAdcCh2CallbackRegister(AdcDataPro_Callback_t pCBS)
{
	if(adc_ch2_callback == 0)
	{
		adc_ch2_callback = pCBS;
	}
}

