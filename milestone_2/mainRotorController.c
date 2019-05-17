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
#include "uartCommunication.h"
#include "driverlib/uart.h"
#include "stdlib.h"
#include "inc/hw_memmap.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"


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
#define OUTPUT_MAX 75
#define OUTPUT_MIN 20

float errorSignal;
int16_t errorSignalPrevious = 0;
int16_t startTime;
double errorIntegral;
int32_t dutyCycle;

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void setMainPWM(uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, ui32Period * ui32Duty / 100);
}

/*********************************************************
 * initialisePWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor motor
 *********************************************************/
void initialiseMainRotorPWM(void)
{

    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);
    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setMainPWM(PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);

}

int32_t mainRotorControlLoop(int16_t currentHeliHeight, int16_t desiredHeliHeight, int16_t groundReference) {

    float mainRotorKp = 0.5;
    float mainRotorKi = 0.04; // 1 //0.44/0.8/0.3
    float mainRotorKd = 0.7;

    errorSignal = (currentHeliHeight - (groundReference - (12.4 * desiredHeliHeight)));
    float errorDerivative = (errorSignal - errorSignalPrevious) / (0.00625);

    dutyCycle = ((errorSignal * mainRotorKp) + (errorIntegral * mainRotorKi) + (errorDerivative * mainRotorKd));

    // output error signal within the parameters
    if (dutyCycle >= OUTPUT_MAX) {
        dutyCycle = OUTPUT_MAX;
    } else if (dutyCycle <= OUTPUT_MIN) {
        dutyCycle = OUTPUT_MIN;
    }
    else
    {
    errorIntegral += errorSignal * 0.00625;
    }

    errorSignalPrevious = errorSignal;
    setMainPWM(PWM_START_RATE_HZ, dutyCycle);

    return dutyCycle;
}
