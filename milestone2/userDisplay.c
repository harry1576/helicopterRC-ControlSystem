// *******************************************************
// Thurs PM Group 23
//
// userDisplay.c - This file includes all the neccessary
// functions required to use the OLED display on the TIVA
// board.
//
// Authors:  Harry Dobbs, Sam Purdy, Sam Dunshea
// Last modified:   25.4.2019
// 
// *******************************************************


#include <stdint.h>
#include "stdlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"



void initDisplay (void)
{
    OLEDInitialise ();  // intialise the Orbit OLED display
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
// Function to display the height as percentage (10-bit value, note).
//
//*****************************************************************************
void
displayAltitudePercentAndYaw(uint16_t meanVal, uint32_t Angle)
{
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("  Altitude  (%) ", 0, 0);
    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "        %2d       ", meanVal);
    // Update line on display.
    OLEDStringDraw (string, 0, 1);

    OLEDStringDraw ("   Angle (deg)  ", 0, 2);
    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "        %2d       ", Angle);
    // Update line on display.
    OLEDStringDraw (string, 0, 3);




}






