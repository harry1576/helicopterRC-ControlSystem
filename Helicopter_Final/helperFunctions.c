//*****************************************************************************
// Thurs PM Group 23
//
// helperFunctions.c - This file contains useful functions that perform simple
// tasks. Containing these functions in one spot has helped maintain tidiness.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//
// Last modified: 25.5.2019
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
// @Description Function used to calculate the height as a percentage based
// on the maximum, minimum and current height values.
// @Param max is the maximum altitude the helicopter can reach as 12bit number.
//        current is the height the helicopter is current at as a 12bit number.
//        minimum is the ground height and the lowest altitude the helicopter
//        can reach as a 12 bit number.bookings/index/pid/20
// @Return percentage is the percentage of the maximum height the helicopter
//  is current at. It is a value within the range 0-100 inclusive
//
//*****************************************************************************
int16_t heightAsPercentage(int16_t max, int16_t current, int16_t min)
{
    int16_t percentage;
    percentage = ((current - min) * 100 + ((max - min) / 2)) / (max - min); // prevents rounding error
    if (percentage < 0) {
        percentage = 0;
    } else if (percentage > 100) {
        percentage = 100;
    }
    return percentage;
}

//*****************************************************************************
//
// Takes the currently angle value which is a continuously summing
// value and converts it to a value between -179 to 180.
//
//*****************************************************************************
int32_t findDisplayAngle(int32_t Angle)
{
    int32_t displayAngle;

    // Finding the angle in a the display format
    if (Angle >= 0 && Angle % 360 <= 180)
    {
        displayAngle = Angle % 180;
    }
    else if (Angle >= 0 && Angle % 360 > 180)
    {
        displayAngle = -180 + (Angle % 180);
    }
    else if (Angle < 0 && (-Angle) % 360 <= 180)
    {
        displayAngle = -((-Angle)%360);
    }
    else if (Angle < 0 && Angle % 360 > 180)
    {
        displayAngle = 360 -((-Angle)%360);
    }
    return displayAngle;
}

const char * findMode (int16_t num)
{
    if (num == 0) {
        return "LANDING";
    }
    else if (num == 1) {
        return "LANDED";
    }
    else if (num == 2) {
        return "TAKINGOFF";
    }
    else if (num == 3) {
        return "FLYING";
    }
    return 0;
}

