// *******************************************************
// 
// buttons4.c
//
// Support for a set of FOUR specific buttons on the Tiva/Orbit.
// ENCE361 sample code.
// The buttons are:  UP and DOWN (on the Orbit daughterboard) plus
// LEFT and RIGHT on the Tiva.
//
// Note that pin PF0 (the pin for the RIGHT pushbutton - SW2 on
//  the Tiva board) needs special treatment - See PhilsNotesOnTiva.rtf.
//
// P.J. Bones UCECE
// Last modified:  7.2.2018
// 
// ********************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"  // Board specific defines (for PF0)
#include "buttons4.h"
#include "yawFiniteStateMachine.h"


// *******************************************************
// Globals to module
// *******************************************************
static bool but_state[NUM_BUTS];    // Corresponds to the electrical state
static uint8_t but_count[NUM_BUTS];
static bool but_flag[NUM_BUTS];
static bool but_normal[NUM_BUTS];   // Corresponds to the electrical state


uint32_t switchChannel;     //Variable to hold the current value of switch position

int16_t desiredHeightPercentage = 0;
int16_t desiredAngle = 0;
uint32_t variableTest = 1;
int8_t flightMode = 2;
//int8_t referenceAngleSet = 0;
volatile int16_t referenceAngle = 0;
int8_t testVariable = 0;



// *******************************************************
// initButtons: Initialise the variables associated with the set of buttons
// defined by the constants in the buttons2.h header file.

void
initButtons (void)
{
    int i;

    // UP button (active HIGH)
    SysCtlPeripheralEnable (UP_BUT_PERIPH);
    GPIOPinTypeGPIOInput (UP_BUT_PORT_BASE, UP_BUT_PIN);
    GPIOPadConfigSet (UP_BUT_PORT_BASE, UP_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPD);
    but_normal[UP] = UP_BUT_NORMAL;
    // DOWN button (active HIGH)
    SysCtlPeripheralEnable (DOWN_BUT_PERIPH);
    GPIOPinTypeGPIOInput (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN);
    GPIOPadConfigSet (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPD);
    but_normal[DOWN] = DOWN_BUT_NORMAL;
    // LEFT button (active LOW)
    SysCtlPeripheralEnable (LEFT_BUT_PERIPH);
    GPIOPinTypeGPIOInput (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN);
    GPIOPadConfigSet (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPU);
    but_normal[LEFT] = LEFT_BUT_NORMAL;
    // RIGHT button (active LOW)
      // Note that PF0 is one of a handful of GPIO pins that need to be
      // "unlocked" before they can be reconfigured.  This also requires
      //      #include "inc/tm4c123gh6pm.h"
    SysCtlPeripheralEnable (RIGHT_BUT_PERIPH);
    //---Unlock PF0 for the right button:
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= GPIO_PIN_0; //PF0 unlocked
    GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    GPIOPinTypeGPIOInput (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN);
    GPIOPadConfigSet (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN, GPIO_STRENGTH_2MA,
       GPIO_PIN_TYPE_STD_WPU);
    but_normal[RIGHT] = RIGHT_BUT_NORMAL;

    for (i = 0; i < NUM_BUTS; i++)
    {
        but_state[i] = but_normal[i];
        but_count[i] = 0;
        but_flag[i] = false;
    }
}

// *******************************************************
// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_BUT_POLLS according to the polling rate.
void
updateButtons (void)
{
    bool but_value[NUM_BUTS];
    int i;

    // Read the pins; true means HIGH, false means LOW
    but_value[UP] = (GPIOPinRead (UP_BUT_PORT_BASE, UP_BUT_PIN) == UP_BUT_PIN);
    but_value[DOWN] = (GPIOPinRead (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN) == DOWN_BUT_PIN);
    but_value[LEFT] = (GPIOPinRead (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN) == LEFT_BUT_PIN);
    but_value[RIGHT] = (GPIOPinRead (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN) == RIGHT_BUT_PIN);
    // Iterate through the buttons, updating button variables as required
    for (i = 0; i < NUM_BUTS; i++)
    {
        if (but_value[i] != but_state[i])
        {
            but_count[i]++;
            if (but_count[i] >= NUM_BUT_POLLS)
            {
                but_state[i] = but_value[i];
                but_flag[i] = true;    // Reset by call to checkButton()
                but_count[i] = 0;
            }
        }
        else
            but_count[i] = 0;
    }
}

// *******************************************************
// checkButton: Function returns the new button logical state if the button
// logical state (PUSHED or RELEASED) has changed since the last call,
// otherwise returns NO_CHANGE.
enum butStates checkButton (uint8_t butName)
{
    if (but_flag[butName])
    {
        but_flag[butName] = false;
        if (but_state[butName] == but_normal[butName])
            return RELEASED;
        else
            return PUSHED;
    }
    return NO_CHANGE;
}


// *******************************************************
// altAndYawValue: Function that checks the buttons and alters
// the desired height and yaw accordingly
void updateDesiredAltAndYawValue(void)
{
    //change these values to the correct place and format, and init them
        uint8_t butState;


        updateButtons ();


        butState = checkButton (UP);
        if (butState == PUSHED)
        {   if(desiredHeightPercentage < 100)
            {desiredHeightPercentage += 10;}
        }

        butState = checkButton (DOWN);
        if (butState == PUSHED)
        {
            desiredHeightPercentage -= 10;
        }

        butState = checkButton (RIGHT);
        if (butState == PUSHED)
        {

            desiredAngle += 15;

        }

        butState = checkButton (LEFT);
        if (butState == PUSHED)
        {

            desiredAngle -= 15;

        }
}





void switchMode (void)      // to check the position ofthe switch and chang variabels as so
{
    switchChannel = GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_7);

    if (switchChannel == 0) // switch is in land position
    {
        flightMode = LANDING;

    } else if (switchChannel != 0) // switch is in take off position
    {
        flightMode = FLYING;
       // referenceAngleSet = 0;
    }

    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_7);
}



void initSwitch(void)
{
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
    GPIOIntRegister(GPIO_PORTA_BASE, switchMode);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOIntTypeSet (GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable (GPIO_PORTA_BASE, GPIO_INT_PIN_7);


}

void resetCheck (void)
{
    SysCtlReset();
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_6);
}



void initReset(void)
{
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
    GPIOIntRegister(GPIO_PORTA_BASE, resetCheck);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOIntTypeSet (GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_BOTH_EDGES);
    GPIOIntEnable (GPIO_PORTA_BASE, GPIO_INT_PIN_6);
}


/*void referenceTriggerHandler(void)
{
    referenceAngle = currentAngle;
   // referenceAngleSet = 1;
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_INT_PIN_4 );


}

void initReferenceTrigger(void)
{

   SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOC);
   GPIOIntRegister(GPIO_PORTC_BASE, referenceTriggerHandler);
   GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
   GPIOIntTypeSet (GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);
   GPIOIntEnable (GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}*/



