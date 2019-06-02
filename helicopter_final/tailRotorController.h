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

extern int32_t TAIL_PWM;

void setTailPWM (uint32_t ui32Freq, uint32_t ui32Duty);

void initialiseTailRotorPWM (void);

uint32_t tailRotorControlLoop(uint16_t desiredAngle,uint16_t refAngle);

int32_t mainDutyCycle(void);



#endif /* MAIN_ROTOR_CONTROLLER_H_ */
