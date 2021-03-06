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
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850), P.J. Bones UCECE
// Last modified: 25.4.2019
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
#include "helicopterAltitude.h"


//*****************************************************************************
// Global variables
//*****************************************************************************
circBuf_t g_inBuffer; // Buffer of size BUF_SIZE integers (sample values)
uint32_t g_ulSampCnt; // Counter for the interrupts


//*****************************************************************************
//
// The handler for the ADC conversion interrupt.
// Writes to the circular buffer.
// @Author P.J. Bones UCECE
//*****************************************************************************
void ADCIntHandler(void)
{
    uint32_t ulValue;

    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue); // Get sample from ADC0
    writeCircBuf (&g_inBuffer, ulValue); // Write the sample to circular buffer
    ADCIntClear(ADC0_BASE, 3); // Cleanup - clear the interrupt
}

//*****************************************************************************
//
// Initialisation of the ADC interrupt
// @Author P.J. Bones UCECE
//*****************************************************************************
void initADC (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);     // The ADC0 peripheral must be enabled for configuration and use.

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion. The trigger is configured in "helicopterMasterController"
    // and is generated using the system tick.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 3); // Since sample sequence 3 is now configured, it must be enabled.

    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler); // Register the interrupt handler

    ADCIntEnable(ADC0_BASE, 3); // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
}

//*****************************************************************************
//
// @Description Reads data from circular buffer that is used for altitude reading
// @Param void
// @Return the average readings from the buffer
//
//*****************************************************************************
int32_t updateAltitude(void)
{
    // Background task: calculate the (approximate) mean of the values in the
    // circular buffer and display it, together with the sample number.

    uint8_t i; // Variable used in for loop to cycle through buffer
    int32_t sum; // The summation of the data read from the buffer

    sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf( & g_inBuffer); // Calculate and display the rounded mean of the buffer contents
    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

}
