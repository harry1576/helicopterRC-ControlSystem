#ifndef HELICOPTER_ALTITUDE_H_
#define HELICOPTER_ALTITUDE_H_


// *******************************************************
// 
// circBufT.h
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  7.3.2017
// 
// *******************************************************

extern circBuf_t g_inBuffer;
extern uint32_t g_ulSampCnt;


void
ADCIntHandler(void);

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************


void initADC (void);





#endif /* HELICOPTER_ALTITUDE_H_ */
