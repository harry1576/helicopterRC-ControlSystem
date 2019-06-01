//*****************************************************************************
// Thurs PM Group 23

// yawFiniteStateMachine.c - Program that utilises a FSM to calculate the yaw
// angle of the helicopter. It uses quadrature decoding.
//
// Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
// Last modified: 25.4.2019
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
#include "buttons4.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "helicopterMain.h"



// Variable that is used to hold the current angle of the helicopter.
volatile int32_t currentAngle = 0;
int8_t referenceAngleSet = 0;

// Variables used to hold the current and previous states. This is required for
// the FSM to function correctly.
uint8_t currentState;
uint8_t previousState;

uint32_t ChannelA;// Variables to hold the values of the two digital sensors, focused on the
uint32_t ChannelB;// rotor disc

uint8_t angleReferenced = 0;
int32_t slotCount; // variable that holds the amount of steps the tranducer has
uint32_t referenceSensor;
//read

//*****************************************************************************
//
// The yaw FSM is able to work out which direction the helicopter is rotating
// in. By using this info it can update the current angle of the helicopter.
//
//*****************************************************************************
void yawFSM(void)
{

    ChannelA = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    ChannelB = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    // Works out the current state of the helicopter

    if (ChannelA == 0 && ChannelB == 0)
    {
        currentState = 1;
    } else if (ChannelA == 0 && ChannelB != 0)
    {
        currentState = 2;
    } else if (ChannelA != 0 && ChannelB != 0)
    {
        currentState = 3;
    } else if (ChannelA != 0 && ChannelB == 0)
    {
        currentState = 4;
    }
    // works out which way the disc is rotating in order
    // to know which way the helicopter is rotating.
    switch (previousState) {

    case 1:
        if (currentState == 2)
        {
            slotCount += 1;
        }
        else
        {
            slotCount -= 1;
        }
        break;
    case 2:
        if (currentState == 3)
        {
            slotCount += 1;
        }
        else
        {
            slotCount -= 1;
        }
        break;
    case 3:
        if (currentState == 4)
        {
            slotCount += 1;
        }
        else
        {
            slotCount -= 1;
        }
        break;
    case 4:
        if (currentState == 1)
        {
            slotCount += 1;
        }
        else
        {
            slotCount -= 1;
        }
        break;

    }

    currentAngle = ((slotCount * 360) / 448);
    previousState = currentState;

    GPIOIntClear(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1));
}

//*****************************************************************************
//
// Initialises the yaw FSM. An interrupt is setup to be triggered when either
// GPIO input has a rising or falling edge.
//
//*****************************************************************************
void yawFSMInit(void) {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOIntRegister(GPIO_PORTB_BASE, yawFSM);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);

    uint32_t InitChannelA = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    uint32_t InitChannelB = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    // Calculate the initial state of the machine
    if (InitChannelA == 0 && InitChannelB == 0)
    {
        previousState = 0;
    }
    else if (InitChannelA == 0 && InitChannelB != 0)
    {
        previousState = 1;
    }
    else if (InitChannelA != 0 && InitChannelB == 0)
    {
        previousState = 2;
    }
    else if (InitChannelA != 0 && InitChannelB != 0)
    {
        previousState = 3;
    }
}




void referenceTriggerHandler(void)
{
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_INT_PIN_4 );
    currentAngle = 0;
    slotCount = 0;
    referenceAngleSet = 1;

}

void initReferenceTrigger(void)
{

   SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOC);
   GPIOIntRegister(GPIO_PORTC_BASE, referenceTriggerHandler);
   GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
   GPIOIntTypeSet (GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);
   GPIOIntEnable (GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}


int16_t getCurrentAngle(void)
{
    return currentAngle;
}

int8_t getReferenceAngleSetState(void)
{
    return referenceAngleSet;
}

void setReferenceAngleSetState(int8_t state)
{
    referenceAngleSet = state;
}



