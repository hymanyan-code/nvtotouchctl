#include "M0518.h"
#include "hal_timer.h"
#include "string.h"


volatile Stu_TimerTypedef Stu_Timer[T_SUM];

/*******************************************************************************
* Function Name  : static void hal_timer4Config(void)
* Description    : ��ʱ��Ӳ�����ú���
* Input          : None
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
static void hal_timer1Config(void)
{

    /* Open Timer1 in periodic mode, enable interrupt and 2 interrupt ticks per second */
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 20000); //50us cycle
    TIMER_EnableInt(TIMER1);

    NVIC_EnableIRQ(TMR1_IRQn);

    TIMER_Start(TIMER1);

}


/*******************************************************************************
* Function Name  : void hal_timerInit(void)
* Description    : ��ʱ����ʼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
void hal_MatrixtimerInit(void)
{
    unsigned char i;
    hal_timer1Config();

    for (i = 0; i < T_SUM; i++)
    {
        Stu_Timer[i].state = T_STA_STOP;
        Stu_Timer[i].CurrentCount = 0;
        Stu_Timer[i].Period = 0;
        Stu_Timer[i].func = 0;
    }
}

//////////////////////////////



/*******************************************************************************
* Function Name  : hal_CreatTimer(TIMER_ID_TYPEDEF id,void (*proc)(void), unsigned short Period,unsigned char state)
* Description    : ������ʱ��
* Input          : - id����ʱ��ID
*									- (*proc)() ����ָ��
*									- Period ��ʱ���ڣ���λ50us
* 								- state ��ʱ����ʼ״̬
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
void hal_CreatTimer(TIMER_ID_TYPEDEF id, void (*proc)(void), unsigned short Period, TIMER_STATE_TYPEDEF state)
{
    Stu_Timer[id].state = state;

    Stu_Timer[id].CurrentCount = 0;
    Stu_Timer[id].Period = Period;
    Stu_Timer[id].func = proc;
}



/*******************************************************************************
* Function Name  : unsigned char hal_CtrlTimerAction(TIMER_ID_TYPEDEF id,TIMER_STATE_TYPEDEF sta)
* Description    : ���ƶ�ʱ������
* Input          : - id����ʱ��ID
*								 	 - sta ��ʱ��״̬
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
TIMER_RESULT_TYPEDEF hal_CtrlTimerAction(TIMER_ID_TYPEDEF id, TIMER_STATE_TYPEDEF sta)
{
    if (Stu_Timer[id].func)		//�ж϶�ʱ���Ƿ����
    {
        Stu_Timer[id].state = sta;
        return T_SUCCESS;
    }
    else
    {
        return T_FAIL;
    }
}

/*******************************************************************************
* Function Name  : TIMER_STATE_RESULT_TYPEDEF	hal_GetTimerState(TIMER_ID_TYPEDEF id)
* Description    : ��ȡ��ʱ��״̬
* Input          : - id����ʱ��ID
*								 	 - sta ��ʱ��״̬
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
TIMER_STATE_TYPEDEF	hal_GetTimerState(TIMER_ID_TYPEDEF id)
{
    if (Stu_Timer[id].func)		//�ж϶�ʱ���Ƿ����
    {
        return Stu_Timer[id].state;

    }
    else
    {
        return T_STA_INVAILD;
    }
}


/*******************************************************************************
* Function Name  : hal_DeleteTimer(TIMER_ID_TYPEDEF id)
* Description    : ɾ����ʱ��
* Input          : - id����ʱ��ID
*
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
TIMER_RESULT_TYPEDEF hal_DeleteTimer(TIMER_ID_TYPEDEF id)
{
    if (Stu_Timer[id].func)
    {
        Stu_Timer[id].state = T_STA_STOP;

        Stu_Timer[id].CurrentCount = 0;
        //Stu_Timer[id].Period = 0;
        Stu_Timer[id].func = 0;
        return T_SUCCESS;
    }
    else
    {
        return T_FAIL;
    }
}

/*******************************************************************************
* Function Name  : hal_ResetTimer(TIMER_ID_TYPEDEF id,TIMER_STATE_TYPEDEF sta)
* Description    : ��λ��ʱ��״̬�ͼ�ʱʱ��
* Input          : - id����ʱ��ID
*								 	 - sta ��ʱ��״̬
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
TIMER_RESULT_TYPEDEF hal_ResetTimer(TIMER_ID_TYPEDEF id, TIMER_STATE_TYPEDEF sta)
{
    if (Stu_Timer[id].func)		//�ж϶�ʱ���Ƿ����
    {
        Stu_Timer[id].state = sta;
        Stu_Timer[id].CurrentCount = 0;

        return T_SUCCESS;
    }
    else
    {
        return T_FAIL;
    }
}

/*******************************************************************************
* Function Name  : static void Hal_TimerHandle(void)
* Description    : ��ʱ���жϼ�ʱ����
* Input          : None
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
static void Hal_TimerHandle(void)
{
    unsigned char i;

    for (i = 0; i < T_SUM; i++)
    {
        if ((Stu_Timer[i].func) && (Stu_Timer[i].state == T_STA_START))
        {
            Stu_Timer[i].CurrentCount++;

            if (Stu_Timer[i].CurrentCount >= Stu_Timer[i].Period)
            {
                Stu_Timer[i].state = T_STA_STOP;
                Stu_Timer[i].func();
            }
        }
    }
}

/*******************************************************************************
* Function Name  : void TIM4_IRQHandler(void)
* Description    : ��ʱ���жϻص�����
* Input          : None
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/

void TMR1_IRQHandler(void)
{
    if (TIMER_GetIntFlag(TIMER1) == 1)
    {
        /* Clear Timer1 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER1);
        Hal_TimerHandle();
    }
}



/////////////////////////////////


