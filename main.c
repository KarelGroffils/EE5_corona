/*
 * File:   main.c
 * Author: Groffils Karel
 *
 * Created on 12 april 2020, 5:35
 */

#include "config.h"
#include "analog_in_app.h"
#include "analog_out_app.h"
#include "datalink.h"
#include "network_transport.h"

/*
 * Variables
 */
unsigned char trpBufferSend[8];
unsigned char trpBufferRecieve[8];
unsigned char trpBufferSendID[8];
unsigned char trpBufferSendIndex = 0;
unsigned char trpBufferRecieveIndex = 0;
unsigned char testvalue = 0;
/*
 * Prototypes
 */
void __interrupt (high_priority) high_ISR(void);   //high priority interrupt routine
void __interrupt (low_priority) low_ISR(void);  //low priority interrupt routine (not used))
/*
 * Functions
 */
 /*************************************************
			Main
**************************************************/
void main(void)
{
    //INITIALIZE
    aia_initialize();
    aoa_initialize();
    dl_initialize();
	aia_startSamplingSensor();
    INTCONbits.GIE = 1;	// Enable global interrupt
    
    //SOFTWARE POLLING
    while(true)    
    {
        int get_value = dl_UARTRecieveByte();
        if (get_value > 1)
        {
            trpBufferRecieve[trpBufferRecieveIndex] = get_value; //Store incomming data in buffer
            trpBufferRecieveIndex++;
            LATBbits.LATB7 = 1; 
        }

        LATBbits.LATB1 = 0;
        if (trpBufferSendIndex > 0)
        {
            trpBufferSendIndex--;
            char sendByte = trpBufferSend[trpBufferSendIndex]; //Send data in buffer
            dl_UARTSendByte(sendByte);
            nt_UpdateTimer(); //works only in this specific case
              
            aoa_processEntry();

        }
    }
}

//INTERUPTS
void __interrupt (high_priority) high_ISR(void)
{
    INTCONbits.GIE = 0;	// Turn Off global interrupt
    aia_Interupt();
    dl_UARTInterupt();
    INTCONbits.GIE = 1;	// Enable global interrupt
}
