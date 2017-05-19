/*
***************************************************************************************************
* Project:  USART
* Filename: USART.c
*
* Created: 18.05.2017
* Author:  M. Schuepbach
*
* Description: This is a USART Driver.
*
***************************************************************************************************
*/

#include "USART.h"


/*
***************************************************************************************************
**                                  GLOBAL VARIABLES AND ARRAYS
***************************************************************************************************
*/
// Initialize global variables or arrays here


/*
***************************************************************************************************
**                                           FUNCTIONS
***************************************************************************************************
*/

/*
***************************************************************************************************
* Function: Add function name here
* --------------------------------
*   Initializes USART0
*   Settings: Asynchronous, 8 data bits, none parity, 1 stop bit
*
***************************************************************************************************
*/
void USART0_init(void)
{
    UBRR0H = (unsigned char)(BAUDRATE>>8);          /* Set baud rate */
    UBRR0L = (unsigned char)BAUDRATE;
    
    UCSR0A = (1<<RXC0);                             /* Set Receive Complete flag, needed for interrupt */
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);     /* Enable Receive Complete Interrupt, receiver and transmitter */
    
    sei();
}

/*
***************************************************************************************************
* Function: Add function name here
* --------------------------------
*   Send byte via USART0
*
*   byte_to_send: Byte that should me sent via USART0
*
***************************************************************************************************
*/
void USART0_send_byte(unsigned char byte_to_send)
{
    while( !(UCSR0A & (1<<UDRE0)) );    /* Wait for empty buffer */
    
    UDR0 = byte_to_send;	/* Transmission starts */
}
