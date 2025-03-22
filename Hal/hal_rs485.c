#include "M0518.h"
#include "hal_rs485.h"
#include <stdint.h>
#include "mb.h"
#include "hal_io.h"
#define REG_HOLDING_START 0x00+1
#define REG_HOLDING_NREGS  1

#define MB_SLAVE_ADRESS 0x31
USHORT usRegHoldingBuf[REG_HOLDING_NREGS];



void HalRs485Init(void)
{
    ( void )eMBInit( MB_RTU, MB_SLAVE_ADRESS, 4 , 19200, MB_PAR_EVEN );

   // ModbusRegisterInitial();

    /* Enable the Modbus Protocol Stack. */
   ( void )eMBEnable(  );
}

void UpdateHoldingRegs(USHORT index)
{
    if(index == 0)
    {
        usRegHoldingBuf[index]=((USHORT)g_input_state<<8 )| (g_output_state);
    }
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


