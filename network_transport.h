/* See config.h file for license and warranty
 */

/* 
 * File: datalink  
 * Author: G. K.
 * Comments: implements UART and other datalink layer features
 */

//HEADER GUARDS
#ifndef NETWORK_TRANSPORT_H
#define	NETWORK_TRANSPORT_H

#include "config.h" // include processor files - each processor file is guarded.  


void nt_initialize(void);
void nt_updateIDTable();
void nt_chekData(unsigned char byte); 
void nt_UpdateTimer(); 

#define TIMEOUT 10 //2.5sec
#define REFRESH_ID 400


#endif	/* XC_HEADER_TEMPLATE_H */
