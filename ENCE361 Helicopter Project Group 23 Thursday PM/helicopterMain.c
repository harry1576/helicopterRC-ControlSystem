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


//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 16
#define SAMPLE_RATE_HZ 160

//*****************************************************************************
// Global Variables
//*****************************************************************************
int8_t PIDFlag;
int8_t displayFlag;
volatile int16_t currentHeight; // variable to store the current helicopter height.

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
    if(g_ulSampCnt % 32 == 0)
    {
        displayFlag = 1;
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
void initClock(void)
{
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
void initTimer(void)
{
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    SysTickIntRegister(SysTickIntHandler); // Register the interrupt handler
    SysTickIntEnable(); // Enable interrupt and device
    SysTickEnable();
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
int main(void)
{

    int16_t groundReference = 0; // Initialise the ground reference at the maximum height. Guaranteeing the first ground reading will be below this point.
    int16_t maxHeight; // variable to store the maximum height the helicopter can reach.
    int32_t displayheight;
    int32_t dersiredDisplayAngle;
    int32_t currentDisplayAngle;
    int8_t countUp = 0;
    int32_t tailDutyCycle = 0;
    int32_t mainDutyCycle = 0;
    int32_t tempAngle = 0;
    int32_t countUp2 = 0;

    displayFlag = 0;
    PIDFlag = 0;

    // Initialise required systems
    initClock();
    initTimer();
    initCircBuf( & g_inBuffer, BUF_SIZE);
    initSwitch();
    initADC();
    initDisplay();
    initButtons();
    initReset();
    yawFSMInit();
    initialiseUSB_UART();


    initialiseMainRotorPWM(); // initalise the PWM for the motors
    initialiseTailRotorPWM();

    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true); // set output states of rotors to true
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);

    IntMasterEnable(); // Enable interrupts to the processor.

    while (1) {
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.

        uint8_t i; // Variable used in for loop to cycle through buffer
        int32_t sum; // The summation of the data read from the buffer

        sum = 0;
        for (i = 0; i < BUF_SIZE; i++)
            sum = sum + readCircBuf( & g_inBuffer); // Calculate and display the rounded mean of the buffer contents
        currentHeight = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

        if (groundReference == 0) // set ground reference on first loop or when button is pushed
        {
            groundReference = currentHeight;
            maxHeight = groundReference - 1240; //(4095*(1)/3.3) = Calculate maximum height as we know maximum height is 0.8V less than ground.
        }

        //updateDesiredAltAndYawValue();
        if (displayFlag == 1) // update display every 200ms.
        {
            dersiredDisplayAngle = findDisplayAngle(desiredAngle);
            currentDisplayAngle = findDisplayAngle(currentAngle);
            displayheight = heightAsPercentage(maxHeight, currentHeight, groundReference);
            char string[128];
            usprintf(string, "Yaw: %5d [%5d]\n\rTail: %5d\n\rHeight: %5d [%5d]\n\rMain: %5d\n\rMode: %s\n\r", currentDisplayAngle, dersiredDisplayAngle, tailDutyCycle, displayheight, desiredHeightPercentage, mainDutyCycle,
(flightMode));
            UARTSend(string);
            displayFlag = 0;
            //displayAltitudePercentAndYaw(displayheight, currentDisplayAngle);

        }

        if (PIDFlag == 1) // called every 0.00625ms
        {
            if (flightMode == FLYING) {
                if (referenceAngleSet == 1) {
                    updateDesiredAltAndYawValue(); // get data from buttons once taken
                    mainDutyCycle = mainRotorControlLoop(currentHeight, desiredHeightPercentage, groundReference);
                    tailDutyCycle = tailRotorControlLoop(currentAngle, desiredAngle);
                    PIDFlag = 0;
                } /*else {
                    desiredHeightPercentage = 10;
                    mainDutyCycle = mainRotorControlLoop(currentHeight, desiredHeightPercentage, groundReference);
                    tailDutyCycle = tailRotorControlLoop(currentAngle, 360); // increment rotation till at reference point
                    //tempAngle += 1;
                    PIDFlag = 0;
                }*/
            }
            if (flightMode == TAKINGOFF)
            {
                countUp2 ++;
                desiredHeightPercentage = 10;
                desiredAngle = 0;
                mainDutyCycle = mainRotorControlLoop(currentHeight, desiredHeightPercentage, groundReference);
                tailDutyCycle = tailRotorControlLoop(currentAngle, tempAngle); // increment rotation till at reference point
                if(countUp2 % 3 == 0){
                tempAngle += 1;}

                PIDFlag = 0;
                if (taken_off != 0 && referenceAngleSet) {
                    flightMode = FLYING;
                }
            }
            if (flightMode == LANDING) {
                desiredHeightPercentage = 10;
                desiredAngle = 0;
                if (heightAsPercentage(maxHeight, currentHeight, groundReference) > desiredHeightPercentage || (currentAngle > 4 || currentAngle < -4)) {
                    mainDutyCycle = mainRotorControlLoop(currentHeight, desiredHeightPercentage, groundReference);
                    tailDutyCycle = tailRotorControlLoop(currentAngle, desiredAngle); // centre position
                    PIDFlag = 0;
                }
                else if (heightAsPercentage(maxHeight, currentHeight, groundReference) == desiredHeightPercentage && mainDutyCycle > 0) {
                    setMainPWM(250, mainDutyCycle);
                    countUp++;
                    tailDutyCycle = tailRotorControlLoop(currentAngle, desiredAngle); // centre position
                    if (countUp % 150 == 0) // decrease duty cycle by 1% every 0.15625 * 2 seconds
                    {
                        mainDutyCycle--;
                    }
                    PIDFlag = 0;
                } else {
                    setTailPWM(250, 0);
                    flightMode = LANDED;
                    PIDFlag = 0;
                }
            }
            if (flightMode == LANDED)
            {
                desiredHeightPercentage = 0;
                countUp = 0;
                referenceAngleSet = 0;
                setMainPWM(250, 0);
                setTailPWM(250, 0);
                mainDutyCycle = 0;
                tailDutyCycle = 0;
                countUp2 = 0;
                PIDFlag = 0;
            }
        }

        int16_t pin = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6);
        if (pin == 0) {
            SysCtlReset();
        }
    }
}
