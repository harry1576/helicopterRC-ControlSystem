//*****************************************************************************
// Thurs PM Group 23
//
// mainRotorController.c - This file contains a PID controller that controls
// the main rotor. It additionally has the files required to setup the main rotor.
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
#include "uartCommunication.h"
#include "driverlib/uart.h"
#include "stdlib.h"
#include "inc/hw_memmap.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"


// PWM configuration
#define PWM_START_RATE_HZ 250
#define PWM_RATE_STEP_HZ 50
#define PWM_RATE_MIN_HZ 250
#define PWM_RATE_MAX_HZ 250
#define PWM_FIXED_DUTY 0
#define PWM_DIVIDER_CODE SYSCTL_PWMDIV_4
#define PWM_DIVIDER 4

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#define PWM_MAIN_BASE PWM0_BASE
#define PWM_MAIN_GEN PWM_GEN_3
#define PWM_MAIN_OUTNUM PWM_OUT_7
#define PWM_MAIN_OUTBIT PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN GPIO_PIN_5
#define OUTPUT_MAX 98
#define OUTPUT_MIN 2


float errorSignal;
float errorIntegral;
int16_t errorSignalPrevious = 0;
int32_t dutyCycle;

//*****************************************************************************
//
// @Description This function is used to set the duty cycle and frequency of
// the main rotor PWM.
// @Param ui32Freq is the desired PWM frequency
// @param ui32Duty is the desired duty cycle for the main rotor
// @Return none
//
//*****************************************************************************
void setMainPWM(uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, ui32Period * ui32Duty / 100);
}

//*****************************************************************************
//
// @Description This function is used initialize the main rotor PWM
// @Param void
// @Return void
//
//*****************************************************************************
void initialiseMainRotorPWM(void)
{

    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM); // Enable the PWM peripheral
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO); // Enable the GPIO peripheral
    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    //setMainPWM(PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true); // set output states of rotors to true


}

//*****************************************************************************
//
// @Description This function is used to control the main rotor
// @Param void
// @Return the duty cycle of the main rotor
//
//*****************************************************************************
int32_t mainRotorControlLoop(int16_t currentHeliHeight, int16_t desiredHeliHeight, int16_t groundReference) {

    const int8_t mainRotorKp = 14; // set the gains for the control system
    const int8_t mainRotorKi = 12;
    const int8_t mainRotorKd = 10;
    const int8_t divisor = 100;

    errorSignal = (currentHeliHeight - (groundReference - ((124 * desiredHeliHeight)/10))); // calculate error signal
    float errorDerivative = (errorSignal - errorSignalPrevious) * 160; // multiplying by 160 is more efficient then dividing by 0.00625, but mathematically equivalent.

    dutyCycle = ((errorSignal * mainRotorKp) + (errorIntegral * mainRotorKi) + (errorDerivative * mainRotorKd)) / divisor;

    // output error signal within the parameters
    if (dutyCycle >= OUTPUT_MAX)
    {
        dutyCycle = OUTPUT_MAX;
    }
    else if (dutyCycle <= OUTPUT_MIN)
    {
        dutyCycle = OUTPUT_MIN;
    }
    else
    {
        errorIntegral += errorSignal/160; // dividing by 160 is more efficient then multiplying by 0.00625, but mathematically equivalent.
    }

    errorSignalPrevious = errorSignal;
    setMainPWM(PWM_START_RATE_HZ, dutyCycle);

    return dutyCycle;
}




