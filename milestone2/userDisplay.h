#ifndef USERDISPLAY_H_
#define USERDISPLAY_H_


void initDisplay (void);

void clearOLED(void);

//*****************************************************************************
//
// Function to display the mean ADC value (10-bit value, note) and sample count.
//
//*****************************************************************************
void displayMeanVal(uint16_t meanVal, uint32_t count);

//*****************************************************************************
//
// Function to display the height as 12 bit value (10-bit value, note).
//
//*****************************************************************************
void displayAltitude12Bit(uint16_t meanVal);

//*****************************************************************************
//
// Function to display the height as percentage (10-bit value, note).
//
//*****************************************************************************
void displayAltitudePercentAndYaw(uint16_t meanVal,int32_t Angle);


#endif /*USERDISPLAY_H_*/
