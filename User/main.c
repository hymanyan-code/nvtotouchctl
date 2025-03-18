#include <stdint.h>
#include "M0518.h"
#include <stdio.h>
#include "OS_System.h"
#include "CPU.h"
#include "hal_task.h"
#define PLL_CLOCK   50000000

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
   // CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART1_MODULE);
    CLK_EnableModuleClock(TMR1_MODULE);

    /* Select UART module clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HCLK, 0);



    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD);

}



void Delay(uint32_t ms)
{
    uint16_t j = 0;

    for (; ms > 0; ms--)
    {
        for (j = 0xFFFF; j > 0 ; j--)
        {
        }
    }
}

void DEBUG_UART1_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART1 module */
    SYS_ResetModule(UART1_RST);

    /* Configure UART1 and set UART0 Baudrate */
    UART_Open(UART1, 115200);
}


int main(void)
{
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
    DEBUG_UART1_Init();
	printf("\n\nCPU @ %d Hz\n", SystemCoreClock);
    printf("HardWare Initialize!!!!\n");

    hal_CPUInit();
	OS_TaskInit();
	
   // GPIO_SetMode(PD, BIT6, GPIO_PMD_OUTPUT);    //beep
   // PD6 = 0;


    hal_task_init();		
	OS_CreatTask(OS_TASK1,hal_task,1,OS_RUN);	

    
    OS_Start();	 



   
}
