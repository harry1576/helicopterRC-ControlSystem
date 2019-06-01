//*****************************************************************************
// Thurs PM Group 23
//
// helicopterMain.c - This routine pieces together various other files and
// functions to implement the control logic of the helicopter .
// Some of the files used in this program have been authored by P.J. Bones
// UCECE. Theses files include acknowledgement that he is the creator or that
// the code contains elements of his.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//
// Last modified: 25.5.2019
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "helicopterAltitude.h"
#include "userDisplay.h"
#include "yawFiniteStateMachine.h"
#include "mainRotorController.h"
#include "tailRotorController.h"
#include "uartCommunication.h"
#include "driverlib/uart.h"
#include "helperFunctions.h"
#include "helicopterMain.h"


int16_t groundReference; // Initialise the ground reference at the maximum height. Guaranteeing the first ground reading will be below this point.
int16_t maxHeight; // variable to store the maximum height the helicopter can reach.
volatile int16_t currentHeight; // variable to store the current helicopter height.


int32_t tempAngle = 0;
volatile int16_t currentHelicopterAngle;

volatile int8_t tailDutyCycle;
volatile int8_t mainRotorDutyCycle;

int8_t referenceAngleSetState;

int8_t flightMode = 1;

volatile int8_t desiredHelicopterHeightPercentage;
volatile int16_t desiredHelicopterAngle;




//*****************************************************************************
// Global Variables
//*****************************************************************************
int8_t PIDFlag;
int8_t displayUARTFlag;
int8_t displayOLEDFlag;
int8_t buttonUpdateFlag;


//*****************************************************************************
//
// @Description The interrupt handler for the for SysTick interrupt.
// @Param void
// @Return nothing
//
//*****************************************************************************
void SysTickIntHandler(void) {
    ADCProcessorTrigger(ADC0_BASE, 3); // Triggers the ADC to do a conversion
    g_ulSampCnt++;
    PIDFlag = 1;
    buttonUpdateFlag = 1;
    if (g_ulSampCnt % 40 == 0) {
       displayUARTFlag = 1;
    }
    if(g_ulSampCnt % 320 == 0) {
        displayOLEDFlag = 1;
    }
}

//*****************************************************************************
//
// @Description Initialisation function for the clock.
// ADC, display.
// @Param void
// @Return nothing
//
//*****************************************************************************
void initClock(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Set the clock rate to 20 MHz
}

//*****************************************************************************
//
// @Description Initialisation function for the timer (SysTick). Which is used
// by display and ADC.
// @Param void
// @Return nothing
//
//*****************************************************************************
void initTimer(void) {
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    SysTickIntRegister(SysTickIntHandler); // Register the interrupt handler
    SysTickIntEnable(); // Enable interrupt and device
    SysTickEnable();
}

int8_t getFlightMode()
{
    return flightMode;
}

void setFlightMode(int8_t state)
{
    flightMode = state;
}


void flightController(void) {

    if (flightMode == TAKINGOFF) {

        desiredHelicopterHeightPercentage = 10;
        desiredHelicopterAngle = 0;
        mainRotorDutyCycle = mainRotorControlLoop(currentHeight, desiredHelicopterHeightPercentage, groundReference);
        referenceAngleSetState = getReferenceAngleSetState();

        if(referenceAngleSetState == 1)
        {
           tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, 0); // increment rotation till at reference point
        }
        else
        {
           tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, 360); // increment rotation till at reference point

        }
        if(referenceAngleSetState == 1 && heightAsPercentage(maxHeight, currentHeight, groundReference) == 10)// prevents looping when heli hasnt reach 10 altitude
        {
            flightMode = FLYING;
        }
    }

    else if (flightMode == FLYING) {

          mainRotorDutyCycle = mainRotorControlLoop(currentHeight, desiredHelicopterHeightPercentage, groundReference);
          tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, desiredHelicopterAngle);
          //When the mode switch is flicked an interrupt is triggered in buttons4.c
          //this will change the mode from FLYING to LANDED
    }

    else if (flightMode == LANDING) {
        desiredHelicopterHeightPercentage = 10;
        desiredHelicopterAngle = 0;

        if((currentHelicopterAngle < 10 && currentHelicopterAngle > 5) || (currentHelicopterAngle > -10 && currentHelicopterAngle < -5) && heightAsPercentage(maxHeight, currentHeight, groundReference) == 10)
        {
            desiredHelicopterHeightPercentage = 5;
        }
        if(currentHelicopterAngle <= 5 && currentHelicopterAngle >= -5)
        {
            desiredHelicopterHeightPercentage = 0;
        }

        tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, 0); // increment rotation till at reference point
        mainRotorDutyCycle = mainRotorControlLoop(currentHeight, desiredHelicopterHeightPercentage, groundReference);

        if(heightAsPercentage(maxHeight, currentHeight, groundReference) == 0)
        {
            flightMode = LANDED;
        }
    }
    if (flightMode == LANDED) {
        setMainPWM(250, 0);
        setTailPWM(250, 0);
        mainRotorDutyCycle = 0;
        GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
        setReferenceAngleSetState(0);
        //When the mode switch is flicked an interrupt is triggered in buttons4.c
        //this will change the mode from LANDED to TAKEOFF
    }

}

//*****************************************************************************
//
// @Description The main function used to  initialise the required componentry
// by calling the various initialise functions. Reads values from circular
// buffer to calculate the current height and then polls the display.
// @Param void
// @Return nothing
//
//*****************************************************************************
int main(void) {

    displayUARTFlag = 0;
    displayOLEDFlag = 0;
    PIDFlag = 0;

    // Initialise required systems
    initClock();
    initTimer();
    initCircBuf( & g_inBuffer, BUF_SIZE);
    initADC();
    initDisplay();
    initButtons();
    yawFSMInit();
    initialiseUSB_UART();
    initReferenceTrigger();
    initResetandSwitchISR();
    initialiseMainRotorPWM(); // initalise the PWM for the motors
    initialiseTailRotorPWM();

    groundReference = updateAltitude();
    maxHeight = groundReference - 1240; //(4095*(1)/3.3) = Calculate maximum height as we know maximum height is 0.8V less than ground.

    IntMasterEnable(); // Enable interrupts to the processor.

    while (1) {

        if (displayUARTFlag == 1) // update display every 200ms.
        {
            updateUARTOutput(desiredHelicopterAngle, currentHelicopterAngle, mainRotorDutyCycle, tailDutyCycle, maxHeight, currentHeight, groundReference,flightMode);
            displayUARTFlag = 0;
        }

        if(displayOLEDFlag == 1)
        {
            updateOLEDDisplay(currentHeight,currentHelicopterAngle,mainRotorDutyCycle,tailDutyCycle);
            displayOLEDFlag = 0;
        }


        if(flightMode == FLYING && buttonUpdateFlag == 1) // called every 0.00625
        {
            updateDesiredAltAndYawValue(); // get data from buttons once taken
            desiredHelicopterHeightPercentage = getDesiredHeightPercentage();
            desiredHelicopterAngle = getdesiredAngle();
            buttonUpdateFlag = 0;
        }

        if (PIDFlag == 1) // called every 0.00625ms
        {
            currentHelicopterAngle = getCurrentAngle();
            currentHeight = updateAltitude();
            flightController();
            PIDFlag = 0;
        }

    }
}
