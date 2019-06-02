//*****************************************************************************
// Thurs PM Group 23
//
// helicopterMain.c - This routine pieces together various files and
// functions to implement the control logic of the helicopter .
// Some of the files used in this program have been authored by P.J. Bones
// UCECE. Theses files include acknowledgment that he is the creator or that
// the code contains elements of his.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//
// Last modified: 2.6.2019
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


//*****************************************************************************
// Global Variables
//*****************************************************************************

int16_t groundReference;
volatile int16_t currentHeight; // Volatile as subject to change unexpectedly
int16_t maxHeight;

volatile int32_t currentHelicopterAngle;// Volatile as subject to change unexpectedly

volatile int8_t tailDutyCycle;
volatile int8_t mainRotorDutyCycle;

int8_t referenceAngleSetState; // Variable to tell if reference point has been set

int8_t flightMode = 1; // Initialize mode as Landed.

volatile int8_t desiredHelicopterHeightPercentage;
volatile int32_t desiredHelicopterAngle;

int8_t PIDFlag; // Variables to hold flag values to allow intertask communication
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
    PIDFlag = 1; // sets flag true every 6.25ms
    buttonUpdateFlag = 1; // sets flag true every 6.25ms
    if (g_ulSampCnt % 40 == 0) {
       displayUARTFlag = 1; //sets flag true every 250ms
    }
    if(g_ulSampCnt % 160 == 0) {
        displayOLEDFlag = 1; // sets flag true every 1000ms
    }
}

//*****************************************************************************
//
// @Description Initialization function for the clock.
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
// @Description Initialization function for the timer (SysTick). Which is used
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

//*****************************************************************************
//
// @Description Getter function to allow other files to know the helicopter
// flight mode without using an Extern variable.
// @Param void
// @Return the current flightmode
//
//*****************************************************************************
int8_t getFlightMode()
{
    return flightMode;
}

//*****************************************************************************
//
// @Description Setter function to allow other files to set the helicopter
// flight mode. This is used by the mode switch interrupt (buttons4.c) to change
// the state of the helicopter after the mode switch is used i.e. landed to takeoff.
// @Param void
// @Return the current flightmode
//
//*****************************************************************************
void setFlightMode(int8_t state)
{
    flightMode = state;
}

//*****************************************************************************
//
// @Description This is a FSM that has 4 different states the helicopter can be
// in. Depending on user input the helicopter will move to another state.
// @Param void
// @Return none
//
//*****************************************************************************
void flightController(void) {

    if (flightMode == TAKINGOFF) {

        desiredHelicopterHeightPercentage = 10; // Take helicopter to 10% altitude
        desiredHelicopterAngle = 0; // Try and get helicopter to reference point
        mainRotorDutyCycle = mainRotorControlLoop(currentHeight, desiredHelicopterHeightPercentage, groundReference,SAMPLE_RATE_HZ);// control of main rotor
        referenceAngleSetState = getReferenceAngleSetState(); // Check if the reference angle is set

        if(referenceAngleSetState == 1)
        {
           tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, 0,SAMPLE_RATE_HZ); // increment rotation till at reference point
        }
        else
        {
           tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, 360,SAMPLE_RATE_HZ); // Do a 360 to find the reference signal

        }
        if(referenceAngleSetState == 1 && heightAsPercentage(maxHeight, currentHeight, groundReference) > 10 && currentHelicopterAngle < 5 && currentHelicopterAngle > -5)// prevents looping when heli hasnt reach 10 altitude
        {
            flightMode = FLYING; // If helicopter is at 10 Height and facing reference point change mode to flying.
        }
    }

    else if (flightMode == FLYING) {
          // set rotors to appropriate duty cycles to get the desired angle and altitude.
          mainRotorDutyCycle = mainRotorControlLoop(currentHeight, desiredHelicopterHeightPercentage, groundReference,SAMPLE_RATE_HZ);
          tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, desiredHelicopterAngle,SAMPLE_RATE_HZ);
          //When the mode switch is flicked an interrupt is triggered in buttons4.c
          //this will change the mode from FLYING to LANDING
    }

    else if (flightMode == LANDING) {
        desiredHelicopterHeightPercentage = 10;
        desiredHelicopterAngle = 0;

        if((currentHelicopterAngle < 10 && currentHelicopterAngle > 5) || (currentHelicopterAngle > -10 && currentHelicopterAngle < -5) && heightAsPercentage(maxHeight, currentHeight, groundReference) == 10)
        {
            // Get Helicopter to within 10 degrees of reference and to 10% altitude then set desired altitude to 5
            desiredHelicopterHeightPercentage = 5;
        }
        if(currentHelicopterAngle <= 4 && currentHelicopterAngle >= -4 )
        {
            // Get Helicopter to within 4 degrees of reference and to 5% altitude then set desired altitude to 0
            desiredHelicopterHeightPercentage = 0;
        }

        tailDutyCycle = tailRotorControlLoop(currentHelicopterAngle, 0, SAMPLE_RATE_HZ); // Get to reference point
        mainRotorDutyCycle = mainRotorControlLoop(currentHeight, desiredHelicopterHeightPercentage, groundReference, SAMPLE_RATE_HZ);// Change height depending on other statements.

        if(heightAsPercentage(maxHeight, currentHeight, groundReference) == 0)
        {
            //If helicopter is on the ground set the state to landed.
            flightMode = LANDED;
        }
    }
    if (flightMode == LANDED) {
        // Turn off all the motors and enable the reference signal interrupt.
        setMainPWM(250, 0);
        setTailPWM(250, 0);
        mainRotorDutyCycle = 0; // set equal to zero for display
        tailDutyCycle = 0;
        GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);// enable reference angle interrupt
        setReferenceAngleSetState(0);
        resetDesiredHeightAndAngle(); // set the desired height and angle back to 0

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

        if (displayUARTFlag == 1) // update display every 250ms.
        {
            updateUARTOutput(desiredHelicopterAngle, currentHelicopterAngle, mainRotorDutyCycle, tailDutyCycle, maxHeight, currentHeight, groundReference,flightMode);
            displayUARTFlag = 0;
        }

        if(displayOLEDFlag == 1) // update OLED every 1000ms - this is a relatively slow update rate as the OLED is surplus when using UART but this feature is still required as per specification
        {
            updateOLEDDisplay(heightAsPercentage(maxHeight, currentHeight, groundReference),currentHelicopterAngle,mainRotorDutyCycle,tailDutyCycle);
            displayOLEDFlag = 0;
        }

        if(flightMode == FLYING && buttonUpdateFlag == 1) // called every 0.00625 - Only fetch the desired values when in flying mode as they don't effect other modes.
        {
            desiredHelicopterHeightPercentage = getDesiredHeightPercentage(); // This function fetches the desired height value from the buttons file
            desiredHelicopterAngle = getdesiredAngle(); //This function fetches the desired angle value from the buttons file
            buttonUpdateFlag = 0;
        }

        if (PIDFlag == 1) // called every 0.00625ms
        {
            currentHelicopterAngle = getCurrentAngle();
            currentHeight = updateAltitude();
            flightController();
            PIDFlag = 0;
        }

        pollButtons(); // this function polls the buttons to update the desired values of angle and height.

    }
}
