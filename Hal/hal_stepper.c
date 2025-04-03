#include "M0518.h"
#include "hal_stepper.h"
#include <stdint.h>
uint32_t stepper_speed = 0;


HalStepper stepper = {0, 0, 0};

void HalStepperInit(void)
{
    GPIO_SetMode(DIR_PORT, DIR_PIN, GPIO_PMD_OUTPUT);
    DIR_PORT_PIN = 0;

    BPWM_ConfigOutputChannel(BPWM1, 2, 30000, 50);

    // Enable output of BPWM0 channel 0
    BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);

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
    BPWM_Start(BPWM1, BPWM_CH_2_MASK);
    stepper.state = 1;
}

void HalStepperStop(void)
{
    BPWM_Stop(BPWM1, BPWM_CH_2_MASK);
    stepper.state = 0;
}

void HalSetStepperSpeed(uint32_t speed)
{
    BPWM_ConfigOutputChannel(BPWM1, 2, speed, 50);
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
