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

void updateUARTOutput(int32_t desiredAngle,int32_t currentAngle,int8_t mainDutyCycle,int8_t tailDutyCycle,
                      int16_t maxHeight,int16_t currentHeight,int16_t groundReference);

#endif /*UART_COMMS_H_*/
