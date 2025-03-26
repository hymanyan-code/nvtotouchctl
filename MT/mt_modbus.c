#include "M0518.h"
#include "mt_modbus.h"
#include <stdint.h>
#include "mb.h"
#include "hal_io.h"
#include "stdio.h"
#include "hal_adc.h"

#define SW_VERSION    (0X0001)

#define REG_HOLDING_START (0x00+1)


#define REG_HONGING_PRODUCT          (0)
#define REG_HONGING_SW               (1)
#define REG_HONGING_IO               (2) 
#define REG_HONGING_ADC1             (3)
#define REG_HONGING_ADC2             (4)


#define REG_HOLDING_NREGS            (5)

#define MB_SLAVE_ADRESS 0x31
USHORT usRegHoldingBuf[REG_HOLDING_NREGS];







void ModbusRegistersInitial(void)
{
    usRegHoldingBuf[REG_HONGING_PRODUCT] = 0x3333;
    usRegHoldingBuf[REG_HONGING_SW] = SW_VERSION;
}

void MtModbusInit(void)
{
    ( void )eMBInit( MB_RTU, MB_SLAVE_ADRESS, 4 , 115200, MB_PAR_NONE );  

    ModbusRegistersInitial();

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
}


static eMBErrorCode WtiteHoldingRegs(USHORT index, USHORT value)
{
    eMBErrorCode eStatus = MB_ENOERR;
    if(index == REG_HONGING_IO)
    {
        
        g_output_state = (uint8_t)value;

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


