#ifndef ___HAL_STEPPER_H_
#define ___HAL_STEPPER_H_




#define STEP_PORT        PB
#define STEP_PIN         BIT8
#define STEP_PORT_PIN    PB8

#define DIR_PORT         PF
#define DIR_PIN          BIT8
#define DIR_PORT_PIN     PF8


#define STEP_EN_PORT         PA
#define STEP_EN_PIN          BIT7
#define STEP_EN_PORT_PIN     PA7


void HalStepperInit(void);

void HalStepperSetDir(uint8_t dir);

#if 0
void HalStepperStart(void);

void HalStepperStop(void);
#endif


void HalSetStepperSpeed(uint32_t speed);

uint32_t HalGetStepperSpeed(void);

uint8_t HalStepperGetStepperEnableState(void);

uint8_t HalStepperGetDir(void);

void HalStepperEnHigh(void);

void HalStepperEnLow(void);
typedef struct
{
    uint8_t enable;
    uint8_t dir;
    uint32_t speed;
} HalStepper;








#endif
