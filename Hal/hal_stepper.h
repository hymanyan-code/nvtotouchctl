#ifndef ___HAL_STEPPER_H_
#define ___HAL_STEPPER_H_




#define STEP_PORT        PB
#define STEP_PIN         BIT8
#define STEP_PORT_PIN    PB8

#define DIR_PORT         PF
#define DIR_PIN          BIT8
#define DIR_PORT_PIN     PF8




void HalStepperInit(void);

void HalStepperSetDir(uint8_t dir);

void HalStepperStart(void);

void HalStepperStop(void);

void HalSetStepperSpeed(uint32_t speed);

uint32_t HalGetStepperSpeed(void);

uint8_t HalStepperGetStepState(void);

uint8_t HalStepperGetDir(void);


typedef struct
{
    uint8_t state;
    uint8_t dir;
    uint32_t speed;
} HalStepper;








#endif
