// *******************************************************
// 
// circBufT.c
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  8.3.2017
// 
// *******************************************************


#include <stdint.h>
#include "stdlib.h"
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



void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}

void clearOLED(void)
{
    OLEDStringDraw ("                ", 0, 0);
    OLEDStringDraw ("                ", 0, 1);
    OLEDStringDraw ("                ", 0, 2);
    OLEDStringDraw ("                ", 0, 3);
}


//*****************************************************************************
//
// Function to display the mean ADC value (10-bit value, note) and sample count.
//
//*****************************************************************************
void
displayMeanVal(uint16_t meanVal, uint32_t count)
{
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("ADC demo 1", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean ADC = %3d", meanVal);
    // Update line on display.
    OLEDStringDraw (string, 0, 1);

    usnprintf (string, sizeof(string), "Sample # %5d", count);
    OLEDStringDraw (string, 0, 3);
}



//*****************************************************************************
//
// Function to display the height as 12 bit value (10-bit value, note).
//
//*****************************************************************************
void
displayAltitude12Bit(uint16_t meanVal)
{
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("   Helicopter", 0, 0);
    OLEDStringDraw (" Altitude 12bit", 0, 1);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "      %4d      ", meanVal);
    // Update line on display.
    OLEDStringDraw (string, 0, 3);

}




//*****************************************************************************
//
// Function to display the height as percentage (10-bit value, note).
//
//*****************************************************************************
void
displayAltitudePercent(uint16_t meanVal)
{
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("   Helicopter", 0, 0);
    OLEDStringDraw ("  Altitude (%)", 0, 1);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "        %2d       ", meanVal);
    // Update line on display.
    OLEDStringDraw (string, 0, 2);

    if (meanVal == 0)
    {
        OLEDStringDraw ("     Landed", 0, 3);
    }
    else
    {
        OLEDStringDraw ("     Flying", 0, 3);
    }

}


