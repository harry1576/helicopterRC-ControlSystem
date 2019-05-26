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

#endif /*UART_COMMS_H_*/
