// *******************************************************
// Thurs PM Group 23
//
// userDisplay.c - This file includes all the necessary
// functions required to use the OLED display on the TIVA
// board.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 25.4.2019
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


//*****************************************************************************
//
// Initialises the Orbit OLED display
//
//*****************************************************************************
void initDisplay (void)
{

    OLEDInitialise ();  // Initialise the Orbit OLED display

}

//*****************************************************************************
//
// Clears the Orbit OLED display by clearing all the characters.
//
//*****************************************************************************
void clearOLED(void)
{
    OLEDStringDraw ("                ", 0, 0);// clear top row
    OLEDStringDraw ("                ", 0, 1);// clear second row
    OLEDStringDraw ("                ", 0, 2);// clear third row
    OLEDStringDraw ("                ", 0, 3);// clear bottom row
}


//*****************************************************************************
//
// Function to display the height as percentage and angle of helicopter in
// degrees.
//
//*****************************************************************************
void displayAltitudePercentAndYaw(uint16_t meanVal, uint32_t Angle)
{
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("  Altitude  (%) ", 0, 0);

    usnprintf (string, sizeof(string), "        %2d       ", meanVal); // Form a
    //new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    OLEDStringDraw (string, 0, 1);    // Update line on display.


    OLEDStringDraw ("   Angle (deg)  ", 0, 2);

    usnprintf (string, sizeof(string), "        %2d       ", Angle);   // Update line on display.
    OLEDStringDraw (string, 0, 3);

}






