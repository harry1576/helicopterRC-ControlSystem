#ifndef USERDISPLAY_H_
#define USERDISPLAY_H_

// *******************************************************
// Thurs PM Group 23
//
// userDisplay.h - In charge of the OLED display.
// includes functions to clear the display aswell as to
// display the yaw angle and altitude.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea
// Last modified: 25.4.2019
//
// *******************************************************


//*****************************************************************************
//
// @Description Initalses the display so it can be used
// @Param void
// @Return nothing
//
//*****************************************************************************
void initDisplay (void);

//*****************************************************************************
//
// @Description Clears the display so it is empty.
// @Param void
// @Return nothing
//
//*****************************************************************************
void clearOLED(void);


//*****************************************************************************
//
// @Description Function used to display the altitude as a percentage and the
// helicopter yaw as an angle in degrees.
// @Param meanVal is the average height value as a percentage
// @Param Angle is the angle of the helicopter from -360 to 360 degrees
// @Return nothing
//
//*****************************************************************************
void displayAltitudePercentAndYaw(uint16_t meanVal,int32_t Angle);

#endif /*USERDISPLAY_H_*/
