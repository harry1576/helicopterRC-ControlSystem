#ifndef YAW_FINITE_H_
#define YAW_FINITE_H_

// *******************************************************
// Thurs PM Group 23
//
// yawFiniteStateMachine.h - A FSM that determines the
// yaw angle of the helicopter by using quadrapture
// decoding and the inputs from two GPIOs.
//
// Authors:  Harry Dobbs, Sam Purdy, Sam Dunshea
// Last modified:   25.4.2019
//
// *******************************************************


// *******************************************************
// Variables that can be accessed by other files.
// *******************************************************
extern volatile uint32_t currentAngle; // Angle of the helicopter

//*****************************************************************************
//
// @Description Uses the current and previous state of the FSM to work out
// which way the helicopter is rotating.  Then increases or decreases the
// angle dependent.
// @Param void
// @Return nothing
//
//*****************************************************************************
void yawFSM(void);

//*****************************************************************************
//
// @Description Initalses the FSM and works out the current state of the
// machine based on the inputs from two GPIO pins. Sets the FSM interrupt to
// be triggered when either GPIO pin has a falling or rising edge.
// @Param void
// @Return nothing
//
//*****************************************************************************
void yawFSMInit(void);


#endif /*YAW_FINITE_H_*/

