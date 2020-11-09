#ifndef HELPER_FUNCTION_H_
#define HELPER_FUNCTION_H_

//*****************************************************************************
// Thurs PM Group 23
//
// helperFunctions.c - This file contains useful functions that perform simple
// tasks. Containing these functions in one spot has helped maintain tidiness.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//
// Last modified: 2.6.2019
//
//*****************************************************************************


//*****************************************************************************
//
// @Description Used to calculate the height as a percentage based
// on the maximum, minimum and current height values.
// @Param max is the maximum altitude the helicopter can reach as 12bit number.
//        current is the height the helicopter is current at as a 12bit number.
//        minimum is the ground height and the lowest altitude the helicopter
//        can reach as a 12 bit
// @Return percentage is the percentage of the maximum height the helicopter
//  is current at. It is a value within the range 0-100 inclusive
//
//*****************************************************************************
int16_t heightAsPercentage(int16_t max, int16_t current, int16_t min);


//*****************************************************************************
//
// @Description Takes the current angle value converts it to a value between
// -179 to 180.
// @Param angle is the current angle which is an 32 bit integer
// @Return displayAngle is an angle between -179 to 180.
//
//*****************************************************************************
int32_t findDisplayAngle(int32_t Angle);


//*****************************************************************************
//
// @Description Takes the current mode and returns a char pointer that responds
// to that current mode.
// @Param num is the current mode as an 16 bit integer
// @Return is a * char value that corresponds to the flightmode.
//
//*****************************************************************************
const char * findMode (int16_t num);

#endif /* HELPER_FUNCTION_H_ */
