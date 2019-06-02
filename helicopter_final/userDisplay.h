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
// @Description This function is used to initalise the OLED display
// @Param void
// @Return none
//
//*****************************************************************************
void initDisplay (void);

//*****************************************************************************
//
// @Description  Clears the Orbit OLED display by clearing all the characters.
// @Param void
// @Return none
//
//*****************************************************************************
void clearOLED(void);

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
void updateOLEDDisplay(int32_t height, int32_t Angle,int32_t mainDuty,int32_t TailDuty);

#endif /*USERDISPLAY_H_*/
