#ifndef MAIN_ROTOR_CONTROLLER_H_
#define MAIN_ROTOR_CONTROLLER_H_

//*****************************************************************************
// Thurs PM Group 23
//
// mainRotorController.c - This file contains a PID controller that controls
// the main rotor. It additionally has the files required to setup the main rotor.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 2.6.2019
//
//*****************************************************************************

//*****************************************************************************
//
// @Description This function is used to set the duty cycle and frequency of
// the main PWM.
// @Param ui32Freq
// @param ui32Duty
// @Return none
//
//*****************************************************************************
void setMainPWM (uint32_t ui32Freq, uint32_t ui32Duty);

//*****************************************************************************
//
// @Description This function is used initialize the main rotor PWM
// @Param void
// @Return void
//
//*****************************************************************************
void initialiseMainRotorPWM (void);

//*****************************************************************************
//
// @Description This function is used to control the main rotor
// @Param currentHeliHeight current Heli height as 12bit number
// @Param desiredHeli Height as a percentage
// @Param groundReference as a 12bit number
// @Param samplingrate is the rate in which the PID control loop is called
// @Return the duty cycle of the main rotor
//
//*****************************************************************************
int32_t mainRotorControlLoop(int16_t currentHeliHeight,int16_t desiredHeliHeight,int16_t groundReference,int16_t samplingRate);

#endif /* MAIN_ROTOR_CONTROLLER_H_ */
