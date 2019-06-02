//*****************************************************************************
// Thurs PM Group 23
//
// tailRotorController.c - This file contains a PID controller that controls
// the tail rotor. It additionally has the files required to setup the tail rotor.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 2.6.2019
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"


// PWM configuration
#define PWM_START_RATE_HZ  250
#define PWM_RATE_STEP_HZ   50
#define PWM_RATE_MIN_HZ    150
#define PWM_RATE_MAX_HZ    300
#define PWM_FIXED_DUTY     0
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

#define TAIL_OUTPUT_MAX 98
#define TAIL_OUTPUT_MIN 2

// Global Variables
int32_t dutyCycle;
int16_t tailErrorSignal;
int16_t tailErrorSignalPrevious = 0;
float errorIntegral;


//*****************************************************************************
//
// @Description This function is used to set the duty cycle and frequency of
// the tail rotor PWM.
// @Param ui32Freq is the desired PWM frequency
// @param ui32Duty is the desired duty cycle for the tail rotor
// @Return none
//
//*****************************************************************************
void setTailPWM (uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,ui32Period * ui32Duty / 100);
}

//*****************************************************************************
//
// @Description This function is used initialize the tail rotor PWM
// @Param void
// @Return void
//
//*****************************************************************************
void initialiseTailRotorPWM (void){


    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);

    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    //setTailPWM(PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);

}


//*****************************************************************************
//
// @Description This function is used to control the tail rotor
// @Param void
// @Return the duty cycle of the tail rotor
//
//*****************************************************************************
uint32_t tailRotorControlLoop(uint16_t currentHelicopterAngle,uint16_t desiredAngle)
{
    const int16_t tailRotorKp = 1200;
    const int16_t tailRotorKi = 100;
    const int16_t tailRotorKd = 400;
    const int16_t divisor = 1000;

    tailErrorSignal = (desiredAngle) - currentHelicopterAngle;
    float errorDerivative = (tailErrorSignal - tailErrorSignalPrevious) * 160;// multiplying by 160 is more efficient then dividing by 0.00625, but mathematically equivalent.

    dutyCycle = ((tailErrorSignal * tailRotorKp) + (errorIntegral * tailRotorKi) + (errorDerivative * tailRotorKd))/divisor;

    // output error signal within the parameters
    if (dutyCycle >= TAIL_OUTPUT_MAX)
    {
        dutyCycle = TAIL_OUTPUT_MAX;
    }
    else if (dutyCycle <= TAIL_OUTPUT_MIN)
    {
        dutyCycle = TAIL_OUTPUT_MIN;
    }
    else
    {
        errorIntegral += tailErrorSignal / 160; // dividing by 160 is more efficient then multiplying by 0.00625, but mathematically equivalent.// Integral Control
    }

    tailErrorSignalPrevious = tailErrorSignal;

    setTailPWM(PWM_START_RATE_HZ,dutyCycle);
    return dutyCycle;
}

