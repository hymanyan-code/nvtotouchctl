/*
 * FreeModbus Libary: MSP430 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "M0518.h"

#include "stdio.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
//#include "log.h"
/* ----------------------- Defines ------------------------------------------*/






/* ----------------------- Static variables ---------------------------------*/
UCHAR           ucGIEWasEnabled = FALSE;
UCHAR           ucCriticalNesting = 0x00;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
   //ENTER_CRITICAL_SECTION(  );
    if( xRxEnable )
    {
        //UART_EnableInt(UART4, (UART_IER_RDA_IEN_Msk ));
       UART_ENABLE_INT(UART4, (UART_IER_RDA_IEN_Msk ));
        //printf("enable rx\r\n");
    }
    else
    {
        UART_DISABLE_INT(UART4, UART_IER_RDA_IEN_Msk);
        //UART_DisableInt(UART4, (UART_IER_RDA_IEN_Msk ));
       // printf("disable rx\r\n");
    }
    if( xTxEnable )
    {
        UART_ENABLE_INT(UART4, UART_IER_THRE_IEN_Msk);
        //UART_EnableInt(UART4, ( UART_IER_THRE_IEN_Msk ));
    }
    else
    {
        UART_DISABLE_INT(UART4, UART_IER_THRE_IEN_Msk);
       // UART_DisableInt(UART4, ( UART_IER_TOUT_IEN_Msk));
    }
   //EXIT_CRITICAL_SECTION(  );
}

BOOL
xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bInitialized = TRUE;
    ULONG           u32parity;

    switch ( eParity )
    {
    case MB_PAR_NONE:
        u32parity = UART_PARITY_NONE;
        break;
    case MB_PAR_ODD:
         u32parity = UART_PARITY_ODD;
        break;
    case MB_PAR_EVEN:
         u32parity = UART_PARITY_EVEN;
        break;
    }

    if( bInitialized )
    {
        ENTER_CRITICAL_SECTION(  );
         /* Reset UART0 module */
        SYS_ResetModule(UART4_RST);

        /* Configure UART0 and set UART0 Baudrate */
        UART_Open(UART4, 115200);
         //UART_SetLine_Config(UART4, ulBaudRate, UART_WORD_LEN_8,  u32parity,  UART_STOP_BIT_1);
      // NVIC_EnableIRQ(UART4_IRQn);
      //UART_ENABLE_INT(UART4, (UART_IER_RDA_IEN_Msk ));
        NVIC_EnableIRQ(UART4_IRQn);
       // UART4->FCR &= ~(UART_FCR_RFITL_Msk | UART_FCR_RTS_TRI_LEV_Msk);

      //  UART4->FIFO =((UART4->FIFO&(~UART_FIFO_RFITL_MASK))|UART4_FIFO_RFITL_4BYTES);
        //UART_EnableInt(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk ));
        EXIT_CRITICAL_SECTION(  );
   
    }
    return bInitialized;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    UART_Write(UART4,(uint8_t*)(&ucByte),1);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    //*pucByte = UART_READ(UART4);
    UART_Read(UART4,(uint8_t*)pucByte,1);
   // printf("%02x\r\n",*pucByte);
    return TRUE;
}



void EnterCriticalSection(void)
{
    uint32_t primask;
    
    // 保存当前的中断状态并禁用中断
    primask = __get_PRIMASK();
    __disable_irq();
    
    if (ucCriticalNesting == 0)
    {
        ucGIEWasEnabled = (primask == 0) ? TRUE : FALSE;
    }
    
    ucCriticalNesting++;
}

void ExitCriticalSection(void)
{
    ucCriticalNesting--;
    
    if (ucCriticalNesting == 0)
    {
        if (ucGIEWasEnabled)
        {
            __enable_irq();  // 恢复中断
        }
    }
}


void UART4_IRQHandler(void)
{
 
    uint32_t u32IntSts = UART4->ISR;
    //printf("u32IntSts:%d\r\n",u32IntSts);
    /* Receive Data Available Interrupt Handle */
    if(u32IntSts & UART_ISR_RDA_INT_Msk)
    {
        pxMBFrameCBByteReceived( );
    }

    /* Transmit Holding Register Empty Interrupt Handle */
    if(u32IntSts & UART_ISR_THRE_INT_Msk)
    {
        pxMBFrameCBTransmitterEmpty( );
    }
}
