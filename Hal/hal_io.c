#include "M0518.h"
#include "hal_io.h"
#include "hal_beep.h"
#include "hal_timer.h"
#include "stdio.h"
volatile uint16_t g_input_state = 0;
volatile uint16_t g_output_state = 0;
volatile uint16_t g_relay_state = 0;


void hal_io_init(void)
{

    GPIO_SetMode(IN0_PORT, IN0_PIN, GPIO_PMD_INPUT);
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
    GPIO_SetMode(OUT7_PORT, OUT7_PIN, GPIO_PMD_OUTPUT);

    GPIO_SetMode(RELAY0_PORT, RELAY0_PIN, GPIO_PMD_OUTPUT);
    GPIO_SetMode(RELAY1_PORT, RELAY1_PIN, GPIO_PMD_OUTPUT);


    OUT1_PORT_PIN = 0;
    OUT2_PORT_PIN = 0;
    OUT3_PORT_PIN = 0;
    OUT4_PORT_PIN = 0;
    OUT5_PORT_PIN = 0;
    OUT6_PORT_PIN = 0;
    OUT7_PORT_PIN = 0;

    RELAY0_PORT_PIN = 0;
    RELAY1_PORT_PIN = 0;
}



uint8_t  hal_input_read(unsigned char port)
{
    uint8_t state;

    switch (port)
    {
    case 0:
        state = IN0_PORT_PIN;
        break;
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

void hal_relay_state_control(uint8_t id, uint8_t state)
{
    if(id == 0)
    {
        if(state)
        {
            RELAY0_PORT_PIN = 1;
        }
        else
        {
            RELAY0_PORT_PIN = 0;
        }
    }
    else (id == 1)
    {
        if(state)
        {
            RELAY1_PORT_PIN = 1;
        }
        else
        {
            RELAY1_PORT_PIN = 0;
        }

    }

}

void hal_output_set(unsigned char port, unsigned char state)
{
    switch (port)
    {
    case 0:
        if (state)
        {
            OUT0_PORT_PIN = 1;
        }
        else
        {
            OUT0_PORT_PIN = 0;
        }

        break;

    case 1:
        if (state)
        {
            OUT1_PORT_PIN = 1;
        }
        else
        {
            OUT1_PORT_PIN = 0;
        }

        break;

    case 2:
        if (state)
        {
            OUT2_PORT_PIN = 1;
        }
        else
        {
            OUT2_PORT_PIN = 0;
        }

        break;

    case 3:
        if (state)
        {
            OUT3_PORT_PIN = 1;
        }
        else
        {
            OUT3_PORT_PIN = 0;
        }

        break;

    case 4:
        if (state)
        {
            OUT4_PORT_PIN = 1;
        }
        else
        {
            OUT4_PORT_PIN = 0;
        }

        break;

    case 5:
        if (state)
        {
            OUT5_PORT_PIN = 1;
        }
        else
        {
            OUT5_PORT_PIN = 0;
        }

        break;

    case 6:
        if (state)
        {
            OUT6_PORT_PIN = 1;
        }
        else
        {
            OUT6_PORT_PIN = 0;
        }
    case 7:
        if (state)
        {
            OUT7_PORT_PIN = 1;
        }
        else
        {
            OUT7_PORT_PIN = 0;
        }

        break;

    case 8:
        if (state)
        {
            RELAY0_PORT_PIN = 1;
        }
        else
        {
            RELAY0_PORT_PIN = 0;
        }

    case 9:
        if (state)
        {
            RELAY1_PORT_PIN = 1;
        }
        else
        {
            RELAY1_PORT_PIN = 0;
        }

    default:
        break;
    }
}

void hal_handle_input_10ms_loop(void)
{
    static uint8_t filter_cnt[INPUT_MAX] = {0};
    static uint16_t bit_state = 0;
    //static uint8_t bit_state_previou = 0;

    bit_state = (BIT0 & hal_input_read(0)) | (BIT1 & (hal_input_read(1) << 1)) | (BIT2 & (hal_input_read(2) << 2)) |
     (BIT3 & (hal_input_read(3) << 3)) | (BIT4 & (hal_input_read(4) << 4)) | (BIT5 & (hal_input_read(5) << 5)) | (BIT6 & (hal_input_read(6) << 6)) | 
     (BIT7 & (hal_input_read(7) << 7)) | (BIT8 & (hal_input_read(8) << 8));

    for (uint8_t i = 0; i < INPUT_MAX; i++)
    {
        if ((bit_state & (1 << i)) != (g_input_state & (1 << i)))
        {
            filter_cnt[i] ++;
            if (filter_cnt[i] ++ > 5) //50ms
            {              
                if (bit_state & (1 << i))
                {
                    //bit_state_previou |=  (1 << i);
                    printf("input %d is on\r\n", i);
                    g_input_state |= (1 << i);
                }
                else
                {
                    //bit_state_previou &= ~(1 << i);
                    printf("input %d is off\r\n", i);
                    g_input_state &= ~(1 << i);
                }
            }
        }
        else
        {
            filter_cnt[i] = 0;
        }
    }
}

void hal_handle_output_10ms_loop(void)
{
    static uint16_t bit_state;
    static uint16_t bit_state_previou;
    bit_state = g_output_state;

    for (uint8_t i = 0; i < OUTPUT_MAX; i++)
    {
        if ((bit_state & (1 << i)) != (bit_state_previou & (1 << i)))
        {
            if (bit_state & (1 << i))
            {
                hal_relay_state_control(i , 1);
                printf("relay %d is on\r\n", i );
            }
            else
            {
                hal_relay_state_control(i , 0);
                printf("relay %d is off\r\n", i );
            }
        }
    }

    bit_state_previou = bit_state;


}

void hal_handle_relay_10ms_loop(void)
{
    static uint16_t relay_state;
    for(uint8_t i =0; i<RELAY_MAX;i++)
    {
        if ((g_relay_state & (1 << i)) != (relay_state & (1 << i)))
        {
            if (g_relay_state & (1 << i))
            {
                hal_output_set(i , 1);
                printf("Output %d is on\r\n", i );
            }
            else
            {
                hal_output_set(i , 0);
                printf("Output %d is off\r\n", i );
            }
        }


    }

    relay_state = g_relay_state;
}