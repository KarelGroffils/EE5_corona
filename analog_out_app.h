/* See config.h file for license and warranty
 */

/* 
 * File: analog out app
 * Author: G. K.
 * Comments: implements an analog output application it can display a sensor ID or an 8bit value depending on the operation mode
 */

//header guard
#ifndef ANALOG_OUT_APP_H
#define	ANALOG_OUT_APP_H

#include "config.h" //defines global parameters  


//prototypes
void initializeAnalogOut(void);
//void analogOutInterupt(void);
void processEntry(void);

#endif	

