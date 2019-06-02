#ifndef HELI_MAIN_H_
#define HELI_MAIN_H_

//*****************************************************************************
// Thurs PM Group 23
//
// helicopterMain.h - This routine pieces together various files and
// functions to implement the control logic of the helicopter .
// Some of the files used in this program have been authored by P.J. Bones
// UCECE. Theses files include acknowledgement that he is the creator or that
// the code contains elements of his.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//
// Last modified: 2.6.2019
//
//*****************************************************************************


//*****************************************************************************
// Constants
//*****************************************************************************
#define SAMPLE_RATE_HZ 160
enum helicopterModes {LANDING = 0, LANDED = 1, TAKINGOFF = 2, FLYING = 3};


//*****************************************************************************
//
// @Description The interrupt handler for the for SysTick interrupt.
// @Param void
// @Return nothing
//
//*****************************************************************************
void SysTickIntHandler(void);


//*****************************************************************************
//
// @Description Initialisation function for the clock.
// ADC, display.
// @Param void
// @Return nothing
//
//*****************************************************************************
void initClock(void);


//*****************************************************************************
//
// @Description Initialisation function for the timer (SysTick). Which is used
// by display and ADC.
// @Param void
// @Return nothing
//
//*****************************************************************************
void initTimer(void);


//*****************************************************************************
//
// @Description Getter function to allow other files to know the helicopter
// flight mode without using an Extern variable.
// @Param void
// @Return the current flightmode
//
//*****************************************************************************
int8_t getFlightMode(void);


//*****************************************************************************
//
// @Description Setter function to allow other files to set the helicopter
// flight mode. This is used by the mode switch interrupt (buttons4.c) to change
// the state of the helicopter after the mode switch is used i.e. landed to takeoff.
// @Param void
// @Return the current flightmode
//
//*****************************************************************************
void setFlightMode(int8_t state);


//*****************************************************************************
//
// @Description This is a FSM that has 4 different states the helicopter can be
// in. Depending on user input the helicopter will move to another state.
// @Param void
// @Return none
//
//*****************************************************************************
void flightController(void);


//*****************************************************************************
//
// @Description The main function used to  initialise the required componentry
// by calling the various initialise functions. Reads values from circular
// buffer to calculate the current height and then polls the display.
// @Param void
// @Return nothing
//
//*****************************************************************************
int main(void);


#endif /* HELI_MAIN_H_ */

