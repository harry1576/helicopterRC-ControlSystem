#ifndef HELICOPTER_ALTITUDE_H_
#define HELICOPTER_ALTITUDE_H_

// *******************************************************
// Thurs PM Group 23
//
// helicopterAltitude.h
//
// Used to store values from the altitude sensor inside a
// circular buffer. An interrupt is responsible for writing
// these values to the buffer. This file includes the
// functions to setup the interrupt and the interrupt
// functionality. This code is based on code supplied in
// the UC ENCE361 Lab4. The code inside Lab4 was written
// by P.J. Bones.
//
//  Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//  Last modified: 25.4.2019
// 
// *******************************************************

// *******************************************************
// Variables that can be accessed by other files.
// *******************************************************

extern circBuf_t g_inBuffer; // Buffer of size BUF_SIZE integers (sample values)
extern uint32_t g_ulSampCnt; // Counter for the interrupts

//*****************************************************************************
//
// @Description The handler for the ADC conversion interrupt. Gathers value
// from ADC and writes it to the circular buffer.
// @Param void
// @Return nothing
//
//*****************************************************************************
void ADCIntHandler(void);

//*****************************************************************************
//
// @Description The initialiser for the ADC conversion interrupt. Sets up
// the interrupt to be called from the processor trigger.
// @Param void
// @Return nothing
//
//*****************************************************************************
void initADC (void);

#endif /* HELICOPTER_ALTITUDE_H_ */
