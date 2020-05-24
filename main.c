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
//                    if (get_value > 64)
//            {
//                LATBbits.LATB2 = 1;
//            }
//            if (get_value > 128)
//            {
//                LATBbits.LATB3 = 1;
//            }
//            if (get_value > 192)
//            {
//                LATBbits.LATB4 = 1;
//            } 
        
        
        //UARTSendByte(0b11101110);
        for (int a =0; a < 55; a++) //DELAY
        {
            int c = 0;
        }
        LATBbits.LATB1 = 0;
        if (trpBufferSendIndex > 0)
        {
            //LATBbits.LATB1 = 1;
            trpBufferSendIndex--;
            char sendByte = trpBufferSend[trpBufferSendIndex]; //Send data in buffer
            dl_UARTSendByte(sendByte);
            nt_UpdateTimer(); //works only in this specific case
            //UARTSendByte(0b11101110);
            
//            if (sendByte > 64)
//            {
//                LATBbits.LATB2 = 1;
//            }
//            if (sendByte > 128)
//            {
//                LATBbits.LATB3 = 1;
//            }
//            if (sendByte > 192)
//            {
//                LATBbits.LATB4 = 1;
//            } 
            
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








//////////TEST WERKT//////////////
//
//#include "config.h"
//#include "datalink.h"
//#include "analog_in_app.h"
//#include "analog_out_app.h"
//
////#define _XTAL_FREQ 20000000
////#define BaudRate 9600
//
//unsigned char trpBuffer[8];
//unsigned char trpBufferId[8];
//unsigned char trpBufferIndex;
//
//void __interrupt (high_priority) high_ISR(void);   //high priority interrupt routine
//
//////***Initializing UART module for PIC16F877A***//
////void Initialize_UART(void)
////{
////    //****Setting I/O pins for UART****//
////    TRISCbits.TRISC6 = 1; // TX Pin set as input (output)
////    TRISCbits.TRISC7 = 1; // RX Pin set as input
////    
////    /**Initialize SPBRG register for required 
////    baud rate and set BRGH for fast baud_rate**/
////    TXSTA1bits.BRGH  = 1;  // select divider: 4
////    BAUDCON1bits.BRG16 = 1; //select divider: 4
////    char divider = 4;
////    SPBRG = ((_XTAL_FREQ/divider)/BaudRate) - 1; //25
////    //0.16% error for 9600baud 1MHz (25.04-25)/25
////
////    
////    //****Enable Asynchronous serial port*******//
////    TXSTA1bits.SYNC  = 0;    // asynchronous mode
////    RCSTA1bits.SPEN  = 1; // enable serial port
////
////    //**Lets prepare for transmission & reception**//
////    TXSTA1bits.TXEN  = 0;    // disable transmitter
////    RCSTA1bits.CREN  = 1;    // enable reciever
////    PIE1bits.TXIE = 0; //disable transmitter interupts 
////    
////    //**Select 8-bit mode**//  
////    TXSTA1bits.TX9   = 1;    // disable 9 bit mode
////    RCSTA1bits.RX9   = 1;    // disable 9 bit mode 
////    RCSTA1bits.ADDEN = 0; //disable adress mode
////    
////    ANSELC = 0x00;
////}
////
////calculate odd parity bit
////unsigned char calcOddParity(unsigned char p)
////{
////    p = p ^ (p >> 4 | p << 4);
////    p = p ^ (p >> 2);
////    p = p ^ (p >> 1);
////    return p & 1;
////}
//
////**Function to send one byte of date to UART**//
////void UART_send_char(char byte)  
////{
////    CONNECT TRANSMITTER TO RC6
////    TXSTA1bits.TXEN  = 1;    // enable transmitter
////    TRISCbits.TRISC6 = 0; // TX Pin set as output //not neccesary
////   
////    for (int b =0; b < 30; b++) //wait for transmitter to startup
////    {
////            Nop(); //wait 1 cycle
////    }
////    
////    CALCULATE ODD PARITY BIT
////	TXSTA1bits.TX9D = calcOddParity(byte); //error correction bit
////    if(calcOddParity(byte))
////    {
////        LATBbits.LATB5 = 1; 
////    }
////    else
////    {
////        LATBbits.LATB6 = 1; 
////    }
////
////
////    
////    
////    TXREG = byte; //Load the transmitter buffer with the received value
////    while(!PIR1bits.TXIF);  // wait till TX buffer is free
////    DISCONNECT TRANSMITTER FROM RC6
////    TRISCbits.TRISC6 = 1; // TX Pin set as high impedance
////    TXSTA1bits.TXEN  = 0;    // disable transmitter
////    
////}
//
////**Function to get one byte of date from UART**//
//char UART_get_char()   
//{
//    if(RCSTAbits.FERR) //framing error
//    {
//        //LATBbits.LATB6 = 1; 
//        SPEN = 0;
//        SPEN = 1; //If error -> Reset 
//
//    }
//    else if(RCSTAbits.OERR) //overrun error
//    {
//        //LATBbits.LATB5 = 1; 
//        CREN = 0; 
//        CREN = 1; //If error -> Reset 
//    }
//    else if(PIR1bits.RCIF)
//    {
//        //LATBbits.LATB3 = 1; 
//        if(RCSTA1bits.RX9D)
//        {
//            LATBbits.LATB4 = 1; 
//        }
//        else
//        {
//            LATBbits.LATB3 = 1; 
//        }
//        PIR1bits.RCIF = 0;
//        return RCREG; //receive the value and send it to main function
//    }
//    
//    return 0;
//}
//
//
//
//void main(void)
//{
//    int get_value;
//
//    TRISB = 0x00; //Initialize PortB as output
//    LATB = 0x00;
//    //Initialize_UART();    //Initialize UART module                    
//    dl_initialize();
//    initializeAnalogIn();
//    initializeAnalogOut();
//    startSamplingSensor(); 
//    INTCONbits.GIE = 1; //verhindert uart_out
//    while(1) //Infinite loop
//    {
//
//      //get_value = UART_get_char(); 
//        //UART_send_char(0b10001110);
//        UARTSendByte(0b10001110);
//        LATBbits.LATB1 = 1;   
//        for (int a =0; a < 255; a++)
//        {
//            int c = 0;
//        }
//        LATB = 0b00000000;
//                
//        LATBbits.LATB1 = 0; 
//        //LATB = RCREG;
//        //get_value = UART_get_char();
//        get_value = UARTRecieveByte();
//        if (get_value > 1)
//        {
//            LATBbits.LATB7 = 1; 
//        }
//        //LATB = get_value;     
//    }
//}
//
//void __interrupt (high_priority) high_ISR(void)
//{
//    INTCONbits.GIE = 0;	// Turn Off global interrupt
//    analogInInterupt();
//    //LATBbits.LATB6 = 0;
//    UARTInterupt();
//    INTCONbits.GIE = 1;	// Enable global interrupt
//}