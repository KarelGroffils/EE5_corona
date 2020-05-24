/* See config.h file for license and warranty
 */

/* 
 * File: analog in app
 * Author: G. K.
 * Comments: implements an analog input sampling application @1Hz compatible with 0-5v analog sensors
 */

//header guard
#ifndef ANALOG_IN_APP_H
#define	ANALOG_IN_APP_H

#include "config.h" //defines global parameters  


//prototypes
void initializeAnalogIn(void);
void analogInInterupt(void);
//void newSample(void);
//void timer0Elapsed(void);
void startSamplingSensor(void);

#endif	







