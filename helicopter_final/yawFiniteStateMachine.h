#ifndef YAW_FINITE_H_
#define YAW_FINITE_H_

//*****************************************************************************
// Thurs PM Group 23

// yawFiniteStateMachine.h - Program that utilises a FSM to calculate the yaw
// angle of the helicopter. It uses quadrature decoding.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 2.6.2019
//
//*****************************************************************************


//*****************************************************************************
//
// @Description The yaw FSM is able to work out which direction the helicopter is rotating
// in. By using this info it can update the current angle of the helicopter.
// @Param void
// @Return none
//
//*****************************************************************************
void yawFSM(void);


//*****************************************************************************
//
// @Description Initialises the yaw FSM. An interrupt is setup to be triggered
// when either GPIO input has a rising or falling edge.
// @Param void
// @Return none
//
//*****************************************************************************
void yawFSMInit(void);


//*****************************************************************************
//
// @Description the interrupt routine for the reference point , it sets the
// current angle back to an origin and sets a variable true to show the angle
// has been set and disables this interrupt to prevent it from happening
// unnecessarily
// @Param void
// @Return none
//
//*****************************************************************************
void referenceTriggerHandler(void);


//*****************************************************************************
//
// @Description Initializes the reference point interrupt which is triggered
// when GPIO_PIN_4 is low
// @Param void
// @Return none
//
//*****************************************************************************
void initReferenceTrigger(void);


//*****************************************************************************
//
// @Description Getter method used to retrieve the current angle of the heli
// @Param void
// @Return the current angle of the helicopter
//
//*****************************************************************************
int16_t getCurrentAngle(void);


//*****************************************************************************
//
// @Description Getter method used to retrieve the variable of whether the
// reference angle of the heli has been set
// @Param void
// @Return the state of wether the reference angle has been set
//
//*****************************************************************************
int8_t getReferenceAngleSetState(void);


//*****************************************************************************
//
// @Description Setter method used to set the variable of whether the
// reference angle of the heli has been set. Used when the helicopter goes from
// landing - landed mode. To ensure it finds reference on next take off.
// @Param the state you wish to set the helicopter to
// @Return none
//
//*****************************************************************************
void setReferenceAngleSetState(int8_t state);


#endif /*YAW_FINITE_H_*/
