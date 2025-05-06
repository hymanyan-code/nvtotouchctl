
#include <stdint.h>
#include "M0518.h"
#include <stdio.h>
#include "OS_System.h"
#include "CPU.h"
#include "hal_task.h"
#include "mt_task.h"
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
    CLK_EnableModuleClock(UART4_MODULE);
    CLK_EnableModuleClock(UART5_MODULE);

    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_EnableModuleClock(ADC_MODULE);

    CLK_EnableModuleClock(BPWM1_MODULE);


    /* Select UART module clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(UART4_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(5));
    CLK_SetModuleClock(UART5_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(5));

    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HCLK, 0);
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HCLK, CLK_CLKDIV_ADC(50));  //5Mhz

    CLK_SetModuleClock(BPWM1_MODULE, CLK_CLKSEL3_BPWM1_S_PCLK, 0);


    SYS_ResetModule(BPWM1_RST);
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD);

    //SYS->GPB_MFP &= ~(SYS_GPC_MFP_PC7_Msk | SYS_GPC_MFP_PC6_Msk);
   //SYS->GPB_MFP |= (SYS_GPC_MFP_PC7_UART4_RXD | SYS_GPC_MFP_PC6_UART4_TXD);
    SYS->GPC_MFP &= ~(SYS_GPC_MFP_PC7_Msk|SYS_GPC_MFP_PC6_Msk);
    SYS->GPC_MFP |= (SYS_GPC_MFP_PC7_UART4_RXD | SYS_GPC_MFP_PC6_UART4_TXD);

    //uart5 pa0 pa1
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA0_Msk|SYS_GPA_MFP_PA1_Msk);
    SYS->GPA_MFP |= (SYS_GPA_MFP_PA0_UART5_TXD | SYS_GPA_MFP_PA1_UART5_RXD);

    SYS->ALT_MFP4 &=~(SYS_ALT_MFP4_PA0_Msk|SYS_ALT_MFP4_PA1_Msk);
    SYS->ALT_MFP4 |=(SYS_ALT_MFP4_PA1_UART5_RXD|SYS_ALT_MFP4_PA0_UART5_TXD);

   SYS->ALT_MFP3 &=~(SYS_ALT_MFP3_PA0_Msk|SYS_ALT_MFP3_PA1_PWM0_CH5);
  
    /* Disable the GPA5 - GPA6 digital input path to avoid the leakage current. ADC5 ADC6 config*/
    GPIO_DISABLE_DIGITAL_PATH(PA, 0x60);

    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA5_Msk|SYS_GPA_MFP_PA6_Msk);
    SYS->GPA_MFP |= (SYS_GPA_MFP_PA5_ADC5 | SYS_GPA_MFP_PA6_ADC6);


     /* Set GPC multi-function pins for BPWM1 Channel 2 */

     //SYS->GPB_MFP = (SYS->GPB_MFP & (~SYS_GPB_MFP_PB8_Msk));
      SYS->GPB_MFP &= (~SYS_GPB_MFP_PB8_Msk);
     SYS->GPB_MFP |= SYS_GPB_MFP_PB8_BPWM1_CH2;

     SYS->ALT_MFP3 &= ~(SYS_ALT_MFP3_PB8_Msk);
     SYS->ALT_MFP3 |= SYS_ALT_MFP3_PB8_BPWM1_CH2;


    SYS->ALT_MFP &= ~(SYS_ALT_MFP_PB8_Msk);
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function: ADC_GetConversionRate                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   Return the A/D conversion rate (sample/second)                                                        */
/*                                                                                                         */
/* Description:                                                                                            */
/*   The conversion rate depends on the clock source of ADC clock.                                         */
/*   It only needs 21 ADC clocks to complete an A/D conversion.                                            */
/*---------------------------------------------------------------------------------------------------------*/
static __INLINE uint32_t ADC_GetConversionRate()
{
    uint32_t u32AdcClkSrcSel;
    uint32_t u32ClkTbl[4] = {__HXT, 0, 0, __HIRC};

    /* Set the PLL clock frequency */
    u32ClkTbl[1] = PllClock;
    /* Set the system core clock frequency */
    u32ClkTbl[2] = SystemCoreClock;
    /* Get the clock source setting */
    u32AdcClkSrcSel = ((CLK->CLKSEL1 & CLK_CLKSEL1_ADC_S_Msk) >> CLK_CLKSEL1_ADC_S_Pos);
    /* Return the ADC conversion rate */
    return ((u32ClkTbl[u32AdcClkSrcSel]) / (((CLK->CLKDIV & CLK_CLKDIV_ADC_N_Msk) >> CLK_CLKDIV_ADC_N_Pos) + 1) / 21);
}



void DEBUG_UART5_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART1 module */
    SYS_ResetModule(UART5_RST);

    /* Configure UART1 and set UART0 Baudrate */
    UART_Open(UART5, 115200);


}


int main(void)
{
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
    DEBUG_UART5_Init();
	

    hal_CPUInit();
	OS_TaskInit();

    printf("\n\nCPU @ %d Hz \n", SystemCoreClock);
    printf("PLL @ %d Hz \n", CLK_GetPLLClockFreq());
    printf("OS Initialized!!!!\n");
   // GPIO_SetMode(PD, BIT6, GPIO_PMD_OUTPUT);    //beep
   // PD6 = 0;


    hal_task_init();	
    printf("Task Initialized,ADC @%d Hz\n",ADC_GetConversionRate());	
	OS_CreatTask(OS_TASK1,hal_task,1,OS_RUN);	

    //mt_task_init();	
	//OS_CreatTask(OS_TASK2,mt_task,1,OS_RUN);	
	
    
    OS_Start();	 



   
}


#define xPSR_ISR_Msk   (0x1FFUL)

/**
  * @brief      User defined hard fault handler
  * @param      stack   A pointer to current stack
  * @return     None
  * @details    This function is an example to show how to implement user's hard fault handler
  *
  */
uint32_t ProcessHardFault(uint32_t lr, uint32_t msp, uint32_t psp)
{
    uint32_t exception_num;
    uint32_t r0, r1, r2, r3, r12, pc, psr;
    uint32_t *stack;

    if (lr & 4)
        stack = (uint32_t *)psp;
    else
        stack = (uint32_t *)msp;

    /* Get information from stack */
    r0  = stack[0];
    r1  = stack[1];
    r2  = stack[2];
    r3  = stack[3];
    r12 = stack[4];
    lr  = stack[5];
    pc  = stack[6];
    psr = stack[7];


    /* Check T bit of psr */
    if((psr & (1 << 24)) == 0)
    {
        printf("PSR T bit is 0.\nHard fault caused by changing to ARM mode!\n");
        while(1);
    }

    /* Check hard fault caused by ISR */
    exception_num = psr & xPSR_ISR_Msk;
    if(exception_num > 0)
    {
        /*
        Exception number
            0 = Thread mode
            1 = Reserved
            2 = NMI
            3 = HardFault
            4-10 = Reserved11 = SVCall
            12, 13 = Reserved
            14 = PendSV
            15 = SysTick, if implemented[a]
            16 = IRQ0.
                .
                .
            n+15 = IRQ(n-1)[b]
            (n+16) to 63 = Reserved.
        The number of interrupts, n, is 32
        */

        printf("Hard fault is caused in IRQ #%d\n", exception_num - 16);

        while(1);
    }

    printf("Hard fault location is at 0x%x\n", pc);
    /*
        If the hard fault location is a memory access instruction, You may debug the load/store issues.

        Memory access faults can be caused by:
            Invalid address - read/write wrong address
            Data alignment issue - Violate alignment rule of Cortex-M processor
            Memory access permission - MPU violations or unprivileged access (Cortex-M0+)
            Bus components or peripheral returned an error response.
    */


    printf("r0  = 0x%x\n", r0);
    printf("r1  = 0x%x\n", r1);
    printf("r2  = 0x%x\n", r2);
    printf("r3  = 0x%x\n", r3);
    printf("r12 = 0x%x\n", r12);
    printf("lr  = 0x%x\n", lr);
    printf("pc  = 0x%x\n", pc);
    printf("psr = 0x%x\n", psr);

    while(1);

}
