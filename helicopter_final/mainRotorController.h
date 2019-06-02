#ifndef MAIN_ROTOR_CONTROLLER_H_
#define MAIN_ROTOR_CONTROLLER_H_

// *******************************************************
// Thurs PM Group 23
//
// helicopterAltitude.h
//
// ghghgfhgfhfdd
//
//  Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//  Last modified: 25.4.2019
//
// *******************************************************

void initialiseMainRotorPWM (void);
void setMainPWM (uint32_t ui32Freq, uint32_t ui32Duty);
int32_t mainRotorControlLoop(int16_t currentHeliHeight,int16_t desiredHeliHeight,int16_t groundReference);
int32_t mainDutyCycle(void);


#endif /* MAIN_ROTOR_CONTROLLER_H_ */
