#include "datalink.h"


void dl_initialize(void)
{
   //BAUDRATE SETTINGS 
   //BAUDCONx.ABDOVF; //OVERFLOW
   //BAUDCONx.RCIDL = 0; //1 = receiver idle 0 start bit detected
   //BAUDCON1bits.RXDTP = 1; //inverted data active low !!!!!!!
   //BAUDCON1bits.TXCKP = 1; //idle state for transmit is low 
   //BAUDCON1bits.BRG16 = 0; //no 16bit baud generator (used for low baud rates with high clock frequency)
   ////BAUDCON1bits.WUE = 1; //wake up enable bit -> RCIF will be set
   ////BAUDCON1bits.ABDEN = 1; //auto baud detection -> goes to zero when completed
   
   //TRANSMISSION SETTINGS 
   //TXSTAx.CSRC; in asynchronous mode not important
   //TXSTA1bits.TX9 = 0; //disable 9 bit mode
   //TXSTA1bits.TXEN = 1; //enable transmitter ///////////////disabled
   //TXSTA1bits.SYNC = 0; //asynchronous mode
   //TXSTA1bits.SENDB = 1; //send break character !!!!!!!
   //TXSTA1bits.BRGH = 0; //start in low speed mode
   //TXSTAx.TRMT = 0; //transmit shift register status bit
   //TXSTAx.TX9D = 0; //adress/data bit or parity bit
   
   //RECIEVER SETTINGS 
   //RCSTA1bits.SPEN = 1; //enable serial port
   //RCSTA1bits.RX9 = 0; //disable 9 bit mode
   //RCSTAx.SREN; /in asynchronous mode not important
   //RCSTA1bits.CREN = 1; //enable reciever
   //RCSTA1bits.ADDEN = 0; //disable adress mode !!!!!!!!misschien 1
   //RCSTAx.FERR; //frame error bit
   //RCSTAx.FERR; //overrun error bit
   //RCSTAx.RCX9D; //nith data bit
    
    //BAUDRATE SETTINGS
    TXSTA1bits.BRGH  = 1;  // select divider: 4
    BAUDCON1bits.BRG16 = 1; //select divider: 4
    char divider = 4;
    SPBRG = ((_XTAL_FREQ/divider)/BaudRate) - 1; //25
    //0.16% error for 9600baud 1MHz (25.04-25)/25
   
   //SET TX AND RX
   TXSTA1bits.SYNC = 0; //asynchronous mode
   TXSTA1bits.TXEN  = 0; // disable transmitter
   RCSTA1bits.CREN  = 1; // enable reciever
   
   //I/O SETTINGS
   TRISCbits.TRISC6 = 1; // transmitter = rc6 set as input 
   TRISCbits.TRISC7 = 1; // Reciever = rc7 set as input
   ANSELC = 0x00; //enable digital input buffer
   RCSTA1bits.SPEN  = 1; // enable serial port
   
   //9-BIT MODE
   TXSTA1bits.TX9   = 1; // disable 9 bit mode
   RCSTA1bits.RX9   = 1; // disable 9 bit mode 
   RCSTA1bits.ADDEN = 0; //disable adress mode
    
   //OSCILLATOR SETTINGS
   //standard 1MHz
   
   //INTERRUPTS
   //PIE1bits.RCIE = 1; //enable UART rx interupt //disables uart sending??
   //IPR1bits.RCIP = 1; //set UART rx high priority
   PIE1bits.TXIE = 0; //disable transmitter interupts 
   //INTCONbits.PEIE = 1; // Enable peripheral interrupts 
}


static unsigned char calcOddParity(unsigned char byte)
{
    byte = byte ^ (byte >> 4 | byte << 4); //^ Bitwise exclusive OR
    byte = byte ^ (byte >> 2);
    byte = byte ^ (byte >> 1);
    return byte & 1; //& bitwise and
}


void UARTSendByte(char byte) 
{
    //CONNECT TRANSMITTER TO RC6
    TXSTA1bits.TXEN  = 1;    // enable transmitter
    TRISCbits.TRISC6 = 0; // TX Pin set as output //not neccesary
   
    for (int b =0; b < 30; b++) //wait for transmitter to startup
    {
        Nop(); //wait 1 cycle
    }
    
    //CALCULATE ODD PARITY BIT
	TXSTA1bits.TX9D = calcOddParity(byte); //error correction bit
//    //DEBUG
//    if(calcOddParity(byte))
//    {
//        LATBbits.LATB5 = 1; 
//    }
//    else
//    {
//        LATBbits.LATB6 = 1; 
//    }
    //////

    //SEND DATA
    
    //while(!PIR1bits.TXIF);  // wait till TX buffer is free
    //LATBbits.LATB4 = 1;
    TXREG = byte; //Load the transmitter buffer with the received value
    //LATBbits.LATB3 = 1;
    //debug
//        for (int b =0; b < 30; b++) //wait for transmitter to startup
//    {
//        Nop(); //wait 1 cycle
//    }    
    //debug

    //LATBbits.LATB6 = 1;
    //DISCONNECT TRANSMITTER FROM RC6
    while(!PIR1bits.TXIF);  // wait till TX buffer is free
    TRISCbits.TRISC6 = 1; // TX Pin set as high impedance
    TXSTA1bits.TXEN  = 0;    // disable transmitter
}


char UARTRecieveByte()   
{
    if(RCSTAbits.FERR) //framing error
    { 
        SPEN = 0;
        SPEN = 1; //If error -> Reset 

    }
    else if(RCSTAbits.OERR) //overrun error
    { 
        CREN = 0; 
        CREN = 1; //If error -> Reset 
    }
    else if(PIR1bits.RCIF)
    {
//        /////DEBUG
//        if(RCSTA1bits.RX9D)
//        {
//            LATBbits.LATB4 = 1; 
//        }
//        else
//        {
//            LATBbits.LATB3 = 1; 
//        }
//        //////
        unsigned char received_byte = RCREG;
        if ( RCSTA1bits.RX9D == calcOddParity(received_byte))
        {
            return received_byte; 
        }
        else
        {
            return CODE_PARITY_ERROR;
        }       
        PIR1bits.RCIF = 0;       
    }
    return CODE_RX_ERROR;
}

void UARTInterupt(void)
{
//    if(PIR1bits.RCIF  == 1) //ADC interupt
//    {        
//        newSample();
//        LATBbits.LATB7 = 1;
//        PIR1bits.RCIF  = 0; //clear interupt flag
//    }  
    //LATBbits.LATB7 = 1;
    char UARTRecieveByte();
}