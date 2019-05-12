#ifndef TAIL_ROTOR_CONTROLLER_H_
#define TAIL_ROTOR_CONTROLLER_H_

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

void setTailPWM (uint32_t ui32Freq, uint32_t ui32Duty);

void initialiseTailRotorPWM (void);

void tailRotorControlLoop(uint16_t desiredAngle,uint16_t refAngle);

#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT


#endif /* MAIN_ROTOR_CONTROLLER_H_ */
