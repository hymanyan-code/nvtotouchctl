#ifndef ___HAL_IO_H_
#define ___HAL_IO_H_

#define INPUT_MAX   (9)
#define OUTPUT_MAX   (8)
#define RELAY_MAX   (2)

#define IN0_PORT        PD
#define IN0_PIN         BIT7
#define IN0_PORT_PIN    PD7

#define IN1_PORT        PD
#define IN1_PIN         BIT14
#define IN1_PORT_PIN    PD14

#define IN2_PORT        PD
#define IN2_PIN         BIT15
#define IN2_PORT_PIN    PD15

#define IN3_PORT        PA
#define IN3_PIN         BIT15
#define IN3_PORT_PIN    PA15

#define IN4_PORT        PA
#define IN4_PIN         BIT14
#define IN4_PORT_PIN    PA14

#define IN5_PORT        PA
#define IN5_PIN         BIT13
#define IN5_PORT_PIN    PA13

#define IN6_PORT        PA
#define IN6_PIN         BIT12
#define IN6_PORT_PIN    PA12

#define IN7_PORT        PA
#define IN7_PIN         BIT2
#define IN7_PORT_PIN    PA2


#define IN8_PORT        PA
#define IN8_PIN         BIT3
#define IN8_PORT_PIN    PA3



#define OUT0_PORT        PB
#define OUT0_PIN         BIT12
#define OUT0_PORT_PIN    PB12

#define OUT1_PORT        PF
#define OUT1_PIN         BIT5
#define OUT1_PORT_PIN    PF5

#define OUT2_PORT        PF
#define OUT2_PIN         BIT4
#define OUT2_PORT_PIN    PF4

#define OUT3_PORT        PA
#define OUT3_PIN         BIT11
#define OUT3_PORT_PIN    PA11

#define OUT4_PORT        PA
#define OUT4_PIN         BIT10
#define OUT4_PORT_PIN    PA10

#define OUT5_PORT        PA
#define OUT5_PIN         BIT9
#define OUT5_PORT_PIN    PA9

#define OUT6_PORT        PA
#define OUT6_PIN         BIT8
#define OUT6_PORT_PIN    PA8

#define OUT7_PORT        PB
#define OUT7_PIN         BIT4
#define OUT7_PORT_PIN    PB4

#define RELAY0_PORT        PB
#define RELAY0_PIN         BIT2
#define RELAY0_PORT_PIN    PB2

#define RELAY1_PORT        PB
#define RELAY1_PIN         BIT3
#define RELAY1_PORT_PIN    PB3



void hal_io_init(void);
void hal_handle_input_10ms_loop(void);
void hal_handle_output_10ms_loop(void);
void hal_handle_relay_10ms_loop(void);

extern volatile uint16_t g_input_state;
extern volatile uint16_t g_output_state;
extern volatile uint16_t g_relay_state;
#endif
