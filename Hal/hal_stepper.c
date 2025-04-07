#include "M0518.h"
#include "hal_stepper.h"
#include <stdint.h>



HalStepper stepper = {0, 0, 200};

void HalStepperInit(void)
{
    GPIO_SetMode(DIR_PORT, DIR_PIN, GPIO_PMD_OUTPUT);
    DIR_PORT_PIN = 0;

    

    // Enable output of BPWM0 channel 0
    //BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
    //BPWM_Start(BPWM1, BPWM_CH_2_MASK);
    BPWM_ConfigOutputChannel(BPWM1, 2, 200, 20);
    //BPWM_Start(BPWM1, BPWM_CH_2_MASK);

}


void HalStepperSetDir(uint8_t dir)
{
    if (dir == 0)
    {
        DIR_PORT_PIN = 0;
        stepper.dir = 0;
    }
    else
    {
        DIR_PORT_PIN = 1;
        stepper.dir = 1;
    }
}

void HalStepperStart(void)
{
    BPWM_ConfigOutputChannel(BPWM1, 2,  stepper.speed, 20);
    BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
    BPWM_Start(BPWM1, BPWM_CH_2_MASK);
    stepper.state = 1;
}

void HalStepperStop(void)
{
    BPWM_DisableOutput(BPWM1, BPWM_CH_2_MASK);
    BPWM_Stop(BPWM1, BPWM_CH_2_MASK);
    stepper.state = 0;
}

void HalSetStepperSpeed(uint32_t speed)
{
    BPWM_ConfigOutputChannel(BPWM1, 2, speed, 20);
    stepper.speed = speed;
}

uint32_t HalGetStepperSpeed(void)
{
    return stepper.speed;
}

uint8_t HalStepperGetStepState(void)
{
    return stepper.state;
}

uint8_t HalStepperGetDir(void)
{
    return stepper.dir;
}
