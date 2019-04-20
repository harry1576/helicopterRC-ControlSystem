//*****************************************************************************
//
// yawFiniteStateMachine.c - Program that utilises a FSM to calculate the yaw
// angle of the helicopter.
//
// Author:  Harry Dobbs, Sam Purdy, Sam Dunshea
// Last modified:   25.4.2019
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


uint8_t currentState = 1;
uint8_t previousState = 0;
extern volatile uint32_t currentAngle = 0;



void yawFSM(void)
{

    uint32_t ChannelA = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0);
    uint32_t ChannelB = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0);

    if (ChannelA == 0 && ChannelB == 0)
    {
        currentState = 0;
    }
    else if(ChannelA == 0 && ChannelB != 0)
    {
        currentState = 1;
    }
    else if(ChannelA != 0 && ChannelB == 0)
    {
        currentState = 2;
    }
    else if(ChannelA != 0 && ChannelB != 0)
    {
        currentState = 3;
    }

    switch(previousState) {

          case 0 :
             if(currentState == 1)
             {
                 currentAngle --;
             }
             else
             {
                 currentAngle ++;
             }
             break;
          case 1 :
              if(currentState == 2)
              {
                  currentAngle --;
              }
              else
              {
                  currentAngle ++;
              }
              break;
          case 2 :
              if(currentState == 3)
              {
                  currentAngle --;
              }
              else
              {
                  currentAngle ++;
              }
              break;
          case 3 :
              if(currentState == 0)
              {
                  currentAngle --;
              }
              else
              {
                  currentAngle ++;
              }
              break;

    }

    GPIOIntClear(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1));

}



void yawFSMInit(void)
{


    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);

    GPIOIntRegister(GPIO_PORTB_BASE, yawFSM);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOIntTypeSet (GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    GPIOIntEnable (GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);


    uint32_t InitChannelA = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0);
    uint32_t InitChannelB = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0);

    if (InitChannelA == 0 && InitChannelB == 0)
    {
        previousState = 0;
    }
    else if(InitChannelA == 0 && InitChannelB != 0)
    {
        previousState = 1;
    }
    else if(InitChannelA != 0 && InitChannelB == 0)
    {
        previousState = 2;
    }
    else if(InitChannelA != 0 && InitChannelB != 0)
    {
        previousState = 3;
    }


}
