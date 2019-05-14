#ifndef YAW_FINITE_H_
#define YAW_FINITE_H_

// *******************************************************
// Thurs PM Group 23
//
// yawFiniteStateMachine.h - A FSM that determines the
// yaw angle of the helicopter by using quadrapture
// decoding and the inputs from two GPIOs.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 25.4.2019
//
// *******************************************************


// *******************************************************
// Variables that can be accessed by other files.
// *******************************************************
extern volatile int32_t currentAngle; // Angle of the helicopter
extern uint8_t angleReferenced;


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



//*****************************************************************************
//
// @Description Takes the currently angle value which is a continuously summing
// value and converts it to a value between -179 to 180.
// @Param int32_t
// @Return int32_t
//
//*****************************************************************************
int32_t findDisplayAngle (int32_t Angle);


#endif /*YAW_FINITE_H_*/

