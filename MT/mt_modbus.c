#include "M0518.h"
#include "mt_modbus.h"
#include <stdint.h>
#include "mb.h"
#include "hal_io.h"
#include "stdio.h"
#include "hal_adc.h"
#include "hal_stepper.h"
#include "hal_dac.h"
#include <math.h>
#define SW_VERSION    (0X0001)

#define REG_HOLDING_START (0x00+1)


#define REG_HONGING_PRODUCT          (0)
#define REG_HONGING_SW               (1)
#define REG_HONGING_IO               (2) 
#define REG_HONGING_ADC1             (3)
#define REG_HONGING_ADC2             (4)
#define REG_HONGING_TEMP_COEF_K      (5)
#define REG_HONGING_TEMP_COEF_B      (6)
#define REG_HONGING_TEMP_RES         (7)  
#define REG_HONGING_TEMP             (8)  
#define REG_HONGING_STEP_CTL        (9)
#define REG_HONGING_STEP_SPEED      (10)

#define REG_HONGING_DAC1          (12)
#define REG_HONGING_DAC2          (13)




#define REG_HOLDING_NREGS            (14)

#define MB_SLAVE_ADRESS 0x31
USHORT usRegHoldingBuf[REG_HOLDING_NREGS];

volatile uint16_t dac_voltage[2]={0};

uint32_t floatToUint(float floatValue)
{
    return *((uint32_t *)&floatValue);
}

// PT100温度计算
#define R0 100.0  // PT100 在 0℃ 时的电阻
#define A  3.9083e-3
#define B -5.775e-7
#define R_MIN 18.52  // -200℃
#define R_MAX 390.48 // 850℃
float calculate_temperature(float Rt) {
    if (Rt < R_MIN || Rt > R_MAX) {
        printf("Error: Resistance out of range!\n");
        return -999;  // 返回错误标志值
    }
    float temp;
    temp = (-A + sqrt(A * A - 4 * B * (1 - Rt / R0))) / (2 * B);
   // printf("%f\r\n",temp);
    return temp;
}

int16_t pt100_coef_k = 10000;
int16_t pt100_coef_b = -3000;
float pt100_res;
float pt100_temp;;
void HalAdc1Callback(uint16_t data)
{
   // pt100_res = data*40.63/4095*5*pt100_coef_k/10000-pt100_coef_b/1000;
    pt100_res = (float)data * 40.7986/4095 * 5 * pt100_coef_k / 10000 + pt100_coef_b / 1000;
    pt100_temp = calculate_temperature(pt100_res);
}


//温度系数k和b的设置
//k乘10000，b乘1000


//电阻值单位欧姆/100
//温度单位摄氏度/10
//数字转模拟电压单位mV




void ModbusRegistersInit(void)
{
    usRegHoldingBuf[REG_HONGING_PRODUCT] = 0x3333;
    usRegHoldingBuf[REG_HONGING_SW] = SW_VERSION;
}

void MtModbusInit(void)
{
    ( void )eMBInit( MB_RTU, MB_SLAVE_ADRESS, 4 , 19200, MB_PAR_NONE );  

    ModbusRegistersInit();

    /* Enable the Modbus Protocol Stack. */
   ( void )eMBEnable(  );
}

static void UpdateHoldingRegs(USHORT index)
{
    if(index == REG_HONGING_IO)
    {
        usRegHoldingBuf[REG_HONGING_IO]=((USHORT)g_input_state<<8 )| (g_output_state);
    }
    else if(index == REG_HONGING_ADC1)
    {
        usRegHoldingBuf[REG_HONGING_ADC1] = adc_average[ADC_CH1];  
    }
    else if(index == REG_HONGING_ADC2)
    {
        usRegHoldingBuf[REG_HONGING_ADC2] = adc_average[ADC_CH2];  
    }
    else if(index == REG_HONGING_TEMP_COEF_K)
    {
        usRegHoldingBuf[REG_HONGING_TEMP_COEF_K] = pt100_coef_k;
        
    }
    else if(index == REG_HONGING_TEMP_COEF_B)
    {
        usRegHoldingBuf[REG_HONGING_TEMP_COEF_B] = pt100_coef_b;
        
    }
    else if(index == REG_HONGING_TEMP_RES)
    {
        //ultemp_data = floatToUint(pt100_res);
        usRegHoldingBuf[REG_HONGING_TEMP_RES] = (uint16_t)(pt100_res*100);

    }
    else if(index == REG_HONGING_TEMP)
    {
       // ultemp_data = floatToUint(pt100_temp);
        //printf("TEMP:%d\r\n",ultemp_data);
        usRegHoldingBuf[REG_HONGING_TEMP] = (uint16_t)(pt100_temp*10);

    }
    else if(index == REG_HONGING_STEP_CTL)
    {
        usRegHoldingBuf[REG_HONGING_STEP_CTL] = (uint16_t)HalStepperGetStepState()<<8 | HalStepperGetDir();  
    }
    else if(index == REG_HONGING_STEP_SPEED|| index == REG_HONGING_STEP_SPEED +1)
    {
        usRegHoldingBuf[REG_HONGING_STEP_SPEED] =  (uint16_t)(HalGetStepperSpeed()>>16);
        usRegHoldingBuf[REG_HONGING_STEP_SPEED+1] = (uint16_t)HalGetStepperSpeed();
    }
    else if (index == REG_HONGING_DAC1)
    {
        
        usRegHoldingBuf[REG_HONGING_DAC1] = dac_voltage[0];
        //printf("update reg12:%d\r\n",usRegHoldingBuf[REG_HONGING_DAC1]);
    }
    else if (index == REG_HONGING_DAC2)
    {
        usRegHoldingBuf[REG_HONGING_DAC2] = dac_voltage[1];
        //printf("update reg13:%d\r\n",usRegHoldingBuf[REG_HONGING_DAC2]);
    }
    //printf("index__%d\r\n",index);

}


static eMBErrorCode WtiteHoldingRegs(USHORT index, USHORT value)
{
    eMBErrorCode eStatus = MB_ENOERR;
    static uint32_t u32value = 0;
    if(index == REG_HONGING_IO)
    {
        
        g_output_state = (uint8_t)value;

    }
    else if(index == REG_HONGING_TEMP_COEF_K)
    {
        pt100_coef_k = (int16_t)value;
    }
    else if(index == REG_HONGING_TEMP_COEF_B)
    {
        pt100_coef_b = (int16_t)value;
    }
    else if (index == REG_HONGING_STEP_CTL)
    {
        if(value&0xFF00)
        {
            HalStepperStart();
            printf("pwm start!\r\n");
        }
        else
        {
            HalStepperStop();
             printf("pwm stop!\r\n");
        }
        HalStepperSetDir((uint8_t)(value&0x00FF));
    }
    else if(index == REG_HONGING_STEP_SPEED)
    {
        u32value = (uint32_t)value<<16;
    }
    else if(index == REG_HONGING_STEP_SPEED+1)
    {
        u32value |= (uint32_t)value;
        if(u32value>=50&&u32value<=200000)
        {
            HalSetStepperSpeed(u32value);
            u32value=0;
        }
        else
        {
            eStatus = MB_EINVAL;
        }
        
    }
    else if(index == REG_HONGING_DAC1)
    {
        dac_voltage[0] = (uint16_t)value;
        printf("dac1 voltage:[%d][%d]\r\n",value,dac_voltage[0]);
        HalDac1SetVoltage((float)value/1000/2);
    }
    else if(index == REG_HONGING_DAC2)
    {
        dac_voltage[1] = (uint16_t)value;
        printf("dac2 voltage:[%d][%d]\r\n",value,dac_voltage[1]);
        HalDac2SetVoltage((float)value/1000/2);
    }
    else
    {
        eStatus = MB_EINVAL;
    }
   
    return eStatus;
}




eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    USHORT reg_value_temp;
    if(( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {

            iRegIndex = ( int )( usAddress - REG_HOLDING_START );


        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                //printf("iRegIndex:%d\r\n",iRegIndex);
                UpdateHoldingRegs(iRegIndex);
				//SrvMbUpdateHoldingRegsForRead(REG_HOLDING_START1,iRegIndex);
				*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
				*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
			
 
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                reg_value_temp = *pucRegBuffer++ << 8;
                reg_value_temp |=*pucRegBuffer++;
                eStatus = WtiteHoldingRegs(iRegIndex,reg_value_temp);
                // if(SrvMbExcuteWriteHoldingRegsCommand(reg_location,iRegIndex,reg_value_temp))
                // {
                //     eStatus = MB_EINVAL;
                //     break;
                // }             

                iRegIndex++;
                usNRegs--;
            }
            
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


