//*****************************************************************************
// Thurs PM Group 23
//
// helicopterAltitude.c - File that includes the functions to set up an
// interrupt that receives analogue values based on helicopter altitude.
// The interrupt stores the values inside a circular buffer. This code is
// based on code supplied in the UC ENCE361 Lab4. The code inside Lab4
// was written by P.J. Bones. This code has minor changes from P.J. Bones
// code such as receiving the data from GPIO rather than the TIVA potentiometer.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 25.4.2019
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


int32_t TAIL_PWM;

int32_t dutyCycle;
int16_t tailErrorSignal;
int16_t tailErrorSignalPrevious = 0;
int16_t tailStartTime;
double errorIntegral;


/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void setTailPWM (uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,ui32Period * ui32Duty / 100);
}


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



uint32_t tailRotorControlLoop(uint16_t currentHelicopterAngle,uint16_t desiredAngle)
{
    int16_t tailRotorKp = 1200;//1.2//0.1/0.258 no kp rotates anti  //working on heli2  3.4
    int16_t tailRotorKi = 100;                      //0.04
    int16_t tailRotorKd = 400;                      //0.03
    int16_t divisor = 1000;

    tailErrorSignal = (desiredAngle) - currentHelicopterAngle;
    float errorDerivative = (tailErrorSignal - tailErrorSignalPrevious)/(0.00625);

    dutyCycle = ((tailErrorSignal * tailRotorKp) + (errorIntegral * tailRotorKi) + (errorDerivative * tailRotorKd))/divisor;

    // output error signal within the parameters
    if (dutyCycle >= TAIL_OUTPUT_MAX){
        dutyCycle = TAIL_OUTPUT_MAX;
    }

    else if (dutyCycle <= TAIL_OUTPUT_MIN){
        dutyCycle = TAIL_OUTPUT_MIN;
    }
    else
    {
        errorIntegral += tailErrorSignal * 0.00625;
    }


    tailErrorSignalPrevious = tailErrorSignal;

    setTailPWM(PWM_START_RATE_HZ,dutyCycle);
    TAIL_PWM = dutyCycle;
    return dutyCycle;
}


