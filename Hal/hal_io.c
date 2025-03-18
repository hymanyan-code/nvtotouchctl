#include "M0518.h"
#include "hal_io.h"
#include "hal_beep.h"
#include "hal_timer.h"
#include "stdio.h"
volatile uint8_t g_input_state;
volatile uint8_t g_output_state;



void hal_io_init(void)
{
	GPIO_SetMode(IN1_PORT, IN1_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN2_PORT, IN2_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN3_PORT, IN3_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN4_PORT, IN4_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN5_PORT, IN5_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN6_PORT, IN6_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN7_PORT, IN7_PIN, GPIO_PMD_INPUT);    
    GPIO_SetMode(IN8_PORT, IN8_PIN, GPIO_PMD_INPUT); 

    GPIO_SetMode(OUT1_PORT, OUT1_PIN, GPIO_PMD_OUTPUT); 
    GPIO_SetMode(OUT2_PORT, OUT2_PIN, GPIO_PMD_OUTPUT);
    GPIO_SetMode(OUT3_PORT, OUT3_PIN, GPIO_PMD_OUTPUT); 
    GPIO_SetMode(OUT4_PORT, OUT4_PIN, GPIO_PMD_OUTPUT);
    GPIO_SetMode(OUT5_PORT, OUT5_PIN, GPIO_PMD_OUTPUT);
		GPIO_SetMode(OUT6_PORT, OUT6_PIN, GPIO_PMD_OUTPUT);
    OUT1_PORT_PIN = 0;
    OUT2_PORT_PIN = 0; 
    OUT3_PORT_PIN = 0;
    OUT4_PORT_PIN = 0;
    OUT5_PORT_PIN = 0;
    OUT6_PORT_PIN = 0;

}

uint8_t  hal_input_read(unsigned char port)
{
    uint8_t state;
    switch(port)
    {
        case 1:
            state = IN1_PORT_PIN;
            break;
        case 2:
            state = IN2_PORT_PIN;
            break;
        case 3:
            state = IN3_PORT_PIN;
            break;
        case 4:
            state = IN4_PORT_PIN;
            break;
        case 5:
            state = IN5_PORT_PIN;
            break;
        case 6:
            state = IN6_PORT_PIN;
            break;
        case 7:
            state = IN7_PORT_PIN;
            break;
        case 8:
            state = IN8_PORT_PIN;
            break;
        default:
            break;
    }
    return (!state);
}


void hal_output_set(unsigned char port, unsigned char state)
{
    switch(port)
    {
        case 1:
            if(state)
                OUT1_PORT_PIN = 1;
            else
                OUT1_PORT_PIN = 0;
            break;
        case 2:
            if(state)
                OUT2_PORT_PIN = 1;
            else
                OUT2_PORT_PIN = 0;
            break;
        case 3:
            if(state)
                OUT3_PORT_PIN = 1;
            else
                OUT3_PORT_PIN = 0;
            break;
        case 4:
            if(state)
                OUT4_PORT_PIN = 1;
            else
                OUT4_PORT_PIN = 0;
            break;
        case 5:
            if(state)
                OUT5_PORT_PIN = 1;
            else
                OUT5_PORT_PIN = 0;
            break;
        case 6:
            if(state)
                OUT6_PORT_PIN = 1;
            else
                OUT6_PORT_PIN = 0;
            break;
        default:
            break;
    }
}

void hal_handle_input_1ms_loop(void)
{
    static uint8_t filter_cnt[8] = {0};
    static uint8_t bit_state;
    static uint8_t bit_state_previou;
    static uint8_t trigger_flag[8] = {0};   

    bit_state = (BIT0 & hal_input_read(1)) | (BIT1 & (hal_input_read(2)<<1) )| (BIT2 & (hal_input_read(3) <<2))| (BIT3 & (hal_input_read(4)<<3)) | (BIT4 & (hal_input_read(5)<<4)) | (BIT5 & (hal_input_read(6)<<5)) | (BIT6 & (hal_input_read(7)<<6) )| (BIT7 & (hal_input_read(8)<<7));

    for(uint8_t i = 0; i < 8; i++)
    {
        if((bit_state & (1 << i)) != (bit_state_previou & (1 << i)))
        {
            filter_cnt[i] = 0;
            trigger_flag[i] = 1 ;
        }
        else
        {
            filter_cnt[i]++;
        }  

        if(filter_cnt[i] >=10 && trigger_flag[i]==1)  //10ms
        {
            trigger_flag[i] = 0;
            if(bit_state & (1 << i))
            {
								hal_beep_on();
	              hal_CreatTimer(T_BEEP,hal_beep_off, 40000, T_STA_START); //2 seconds
                printf("input %d is on", i);
                g_input_state |= (1 << i);
                g_output_state |= (1 << i);
            }
            else
            {
                printf("input %d is off", i);
                g_input_state &= ~(1 << i);
                g_output_state &= ~(1 << i);
            }
        }

    }

    bit_state_previou = bit_state;
    hal_ResetTimer(T_INPUT, T_STA_START);
}

void hal_handle_output_1ms_loop(void)
{
    static uint8_t bit_state;
    static uint8_t bit_state_previou;
	bit_state = g_output_state;
    for (uint8_t i = 0; i < 8; i++)
    {
        if((bit_state & (1 << i)) != (bit_state_previou & (1 << i)))
        {
            if(bit_state & (1 << i))
            {
                hal_output_set(i+1, 1);
            }
            else
            {
                hal_output_set(i+1, 0);
            }
        }
    }

    bit_state_previou = bit_state;
    hal_ResetTimer(T_OUTPUT, T_STA_START);

}
