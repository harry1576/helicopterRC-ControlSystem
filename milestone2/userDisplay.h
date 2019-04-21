#ifndef USERDISPLAY_H_
#define USERDISPLAY_H_


void initDisplay (void);

void clearOLED(void);


//*****************************************************************************
//
// Function to display the height as percentage (10-bit value, note).
//
//*****************************************************************************
void displayAltitudePercentAndYaw(uint16_t meanVal,int32_t Angle);


#endif /*USERDISPLAY_H_*/
