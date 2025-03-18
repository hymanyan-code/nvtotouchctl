#ifndef ___HAL_IO_H_
#define ___HAL_IO_H_



#define IN1_PORT        PD
#define IN1_PIN         BIT7
#define IN1_PORT_PIN    PD7

#define IN2_PORT        PD
#define IN2_PIN         BIT14
#define IN2_PORT_PIN    PD14

#define IN3_PORT        PD
#define IN3_PIN         BIT15 
#define IN3_PORT_PIN    PD15
    
#define IN4_PORT        PA
#define IN4_PIN         BIT15
#define IN4_PORT_PIN    PA15
    
#define IN5_PORT        PA
#define IN5_PIN         BIT14
#define IN5_PORT_PIN    PA14
    
#define IN6_PORT        PA
#define IN6_PIN         BIT13
#define IN6_PORT_PIN    PA13
    
#define IN7_PORT        PA
#define IN7_PIN         BIT12
#define IN7_PORT_PIN    PA12
    
#define IN8_PORT        PB
#define IN8_PIN         BIT15
#define IN8_PORT_PIN    PB15



#define OUT1_PORT        PB
#define OUT1_PIN         BIT12   
#define OUT1_PORT_PIN    PB12    
     
#define OUT2_PORT        PF
#define OUT2_PIN         BIT5
#define OUT2_PORT_PIN    PF5
     
#define OUT3_PORT        PF
#define OUT3_PIN         BIT4
#define OUT3_PORT_PIN    PF4
     
#define OUT4_PORT        PA
#define OUT4_PIN         BIT11
#define OUT4_PORT_PIN    PA11
     
#define OUT5_PORT        PA
#define OUT5_PIN         BIT10
#define OUT5_PORT_PIN    PA10

#define OUT6_PORT        PA
#define OUT6_PIN         BIT9
#define OUT6_PORT_PIN    PA9



void hal_io_init(void);
void hal_handle_input_1ms_loop(void);
void hal_handle_output_1ms_loop(void);


#endif
