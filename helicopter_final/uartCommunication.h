#ifndef UART_COMMS_H_
#define UART_COMMS_H_


//********************************************************
// initialiseUSB_UART - 8 bits, 1 stop bit, no parity
//********************************************************
#define MAX_STR_LEN 16
void initialiseUSB_UART (void);
extern char statusStr[MAX_STR_LEN + 1];


//**********************************************************************
// Transmit a string via UART0
//**********************************************************************
void UARTSend (char *pucBuffer);

//*****************************************************************************
//
// @Description This function takes the parameters that need to be displayed
// via UART and then logically strucutres them for printing
// @Param desiredAngle the angle the user wishes the helicopter to be at
// @Param currentAngle the current angle of the helicopter
// @Param mainDutyCycle the duty cycle of the main rotor
// @Param tailDutyCylce the duty cycle of the tail rotor
// @Param maxHeight the maximum height of the helicopeter as a 12bit number
// @Param currentHeight the current height of the helicopeter as a 12bit number
// @Param groundReference the ground height of the helicopeter as a 12bit number
// @Param flightMode the flight mode of the helicopter specified as an integer
// @Return none
// @Authors (student ID): Harry Dobbs (89030703), Sam Purdy (48538646), Sam Dunshea (26500850)
//*****************************************************************************
void updateUARTOutput(int32_t desiredAngle,int32_t currentAngle,int8_t mainDutyCycle,int8_t tailDutyCycle,
                      int16_t maxHeight,int16_t currentHeight,int16_t groundReference, int8_t flightMode );

#endif /*UART_COMMS_H_*/
