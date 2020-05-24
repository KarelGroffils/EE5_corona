/* See config.h file for license and warranty
 */

/* 
 * File: datalink  
 * Author: G. K.
 * Comments: implements UART and other datalink layer features
 */

//HEADER GUARDS
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include "config.h" // include processor files - each processor file is guarded.  


void dl_initialize(void);
void UARTSendByte(char byte);
char UARTRecieveByte(void); 
void UARTInterupt(void);
//unsigned char calcOddParity(unsigned char byte);

#endif	/* XC_HEADER_TEMPLATE_H */

