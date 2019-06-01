#ifndef USERDISPLAY_H_
#define USERDISPLAY_H_

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
void updateOLEDDisplay(int32_t height, int32_t Angle,int32_t mainDuty,int32_t TailDuty);

const char * findMode (int16_t num);


#endif /*USERDISPLAY_H_*/
