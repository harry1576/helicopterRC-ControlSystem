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
// Takes the currently angle value which is a continuously summing
// value and converts it to a value between -179 to 180.
//
//*****************************************************************************
int32_t findDisplayAngle (int32_t Angle)
{
    int32_t displayAngle;   

    // Checking the sign of the total angle
    if (Angle >= 0)
    {
        if ((Angle % 360) < 180)         // Uses the mod to determine which half of the rotation it lays in
        {
            displayAngle = Angle % 180;
        }
        else
        {
            displayAngle = -180 + Angle % 180;
        }
    }
    
    // same process for when angle is negative, using mod as well in the later if statements
    else
    {
        if ((Angle % 360) < 180)
        {
            displayAngle = (Angle % 180);
        }
        else
        {
            displayAngle = 180 + Angle % 180;
        }
    }
    if (displayAngle == 180)
        {
            displayAngle = -180;
        }
    return displayAngle;
}


//*****************************************************************************
//
// Function to display the height as percentage and angle of helicopter in
// degrees.
//
//*****************************************************************************
void displayAltitudePercentAndYaw(uint16_t meanVal, int32_t Angle)
{
    char string[17];  // 16 characters across the display
    OLEDStringDraw ("Altitude", 0, 0);

    usnprintf (string, sizeof(string), "%3d", meanVal); // Form a
    //new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    OLEDStringDraw (string , 9, 0);    // Update line on display.
    OLEDStringDraw ("%" , 12, 0);    // Update line on display.


    OLEDStringDraw ("Angle", 0, 1);

    usnprintf (string, sizeof(string), "%4d", findDisplayAngle(Angle));   // Update line on display.
    OLEDStringDraw (string, 6, 1);
    OLEDStringDraw ("(deg)", 11, 1);


}






