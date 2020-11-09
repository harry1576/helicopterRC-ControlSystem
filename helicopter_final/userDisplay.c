
// *******************************************************
// Thurs PM Group 23
//
// userDisplay.h - In charge of the OLED display.
// includes functions to clear the display aswell as to
// display the yaw angle and altitude.
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
#include "mainRotorController.h"
#include "tailRotorController.h"
#include "helperFunctions.h"


//*****************************************************************************
//
// @Description This function is used to initalise the OLED display
// @Param void
// @Return none
//
//*****************************************************************************
void initDisplay (void)
{

    OLEDInitialise ();  // Initialise the Orbit OLED display

}

//*****************************************************************************
//
// @Description  Clears the Orbit OLED display by clearing all the characters.
// @Param void
// @Return none
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
// @Description  Function to display the height as percentage and angle of helicopter in
// degrees.
// @Param height the current height of the helicopter
// @Param Angle the current angle of the helicopter
// @Param mainDuty the duty cycle of the main rotor
// @Param TailDuty the duty cycle of the tail rotor
// @Return none
//
//*****************************************************************************
void updateOLEDDisplay(int32_t height, int32_t Angle,int32_t mainDuty,int32_t TailDuty)
{
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Altitude", 0, 0);

    usnprintf (string, sizeof(string), "%3d", height); // Form a
    //new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    OLEDStringDraw (string , 9, 0);    // Update line on display.
    OLEDStringDraw (" %" , 12, 0);    // Update line on display.


    OLEDStringDraw ("Angle", 0, 1);
    usnprintf (string, sizeof(string), "%4d", findDisplayAngle(Angle));   // Update line on display.
    OLEDStringDraw (string, 8, 1);
    OLEDStringDraw (" deg", 12, 1);


    OLEDStringDraw ("Main PWM", 0, 2);
    usnprintf (string, sizeof(string), "%3d", mainDuty);   // Update line on display.
    OLEDStringDraw (string, 9, 2);
    OLEDStringDraw ("%", 13, 2);


    OLEDStringDraw ("Tail PWM", 0, 3);
    usnprintf (string, sizeof(string), "%3d", TailDuty);   // Update line on display.
    OLEDStringDraw (string, 9, 3);
    OLEDStringDraw ("%", 13, 3);
}


