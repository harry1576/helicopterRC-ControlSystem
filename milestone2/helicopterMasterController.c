//*****************************************************************************
//
// helicopterMasterController.c - program that controls the main aspects of
// the helicopter by including other relevant code.
//
// Author:  Harry Dobbs, Sam Purdy, Sam Dunshea
// Last modified:   25.4.2019
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "helicopterAltitude.h"
#include "userDisplay.h"



//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10
#define SAMPLE_RATE_HZ 150

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //
    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;
}

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}






// Function to calculate height as a percentage given the minimum height , maximum height and current height.
int16_t heightAsPercentage(int16_t max, int16_t current, int16_t min)
{
    return -(current *100  - min *100)/(min-max);
}

int
main(void)
{
    uint16_t i;
    int16_t currentHeight;
    int16_t groundReference;
    int16_t maxHeight;
    int16_t heightAboveGround;
    int32_t sum;
    uint8_t butState;
    uint8_t displayNumber;

    initClock ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initADC ();
    initDisplay ();
    initButtons();


    // Enable interrupts to the processor.
    IntMasterEnable();

    groundReference = 0;
    displayNumber = 0;

    while (1)
    {

        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++)
            sum = sum + readCircBuf (&g_inBuffer);
        // Calculate and display the rounded mean of the buffer contents{
        currentHeight = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

        updateButtons ();
        butState = checkButton (LEFT);
        if (groundReference == 0 || butState == PUSHED )
        {
           groundReference = currentHeight;
           maxHeight = groundReference - (4905*(0.8/3));
        }
        butState = checkButton (UP);
        if (butState == PUSHED )
        {
            clearOLED();
            displayNumber ++;
            if (displayNumber == 3)
            {
                displayNumber = 0;
            }
        }

        heightAboveGround = groundReference - currentHeight;

        if (displayNumber == 0)
        {
           displayAltitudePercent(heightAsPercentage(maxHeight,currentHeight,groundReference)); // displays altitiude as percent
        }
        if (displayNumber == 1)
        {
            displayAltitude12Bit(heightAboveGround); // displays height at 12bit number
        }

        SysCtlDelay (SysCtlClockGet() / 150);  // Update display at ~ 2 Hz

    }
}

