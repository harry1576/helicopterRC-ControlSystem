#ifndef HELI_MAIN_H_
#define HELI_MAIN_H_



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

void flightController(void);

int8_t getFlightMode(void);

void setFlightMode(int8_t state);

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

