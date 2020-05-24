#include "analog_out_app.h"


void aoa_initialize(void)
{
    //CONFIGURE I/O PORTS
    LATB = 0x00; //Initial PORTB
    TRISB = 0x00; //Define PORTB as output
    ANSELB = 0xFF; //Disable digital input buffer

}

//void analogOutInterupt(void)
//{
//    
//}

void aoa_processEntry()
{
    LATBbits.LATB1 = 1;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    if (trpBufferRecieveIndex > 0)
    {
        trpBufferRecieveIndex--; //points to top of stack/buffer
        //LATBbits.LATB7 = 1;

        //OUTPUT the analog value
        char sample_value = trpBufferRecieve[trpBufferRecieveIndex];

        if (sample_value > 64)
        {
            LATBbits.LATB2 = 1;
        }
        if (sample_value > 128)
        {
            LATBbits.LATB3 = 1;
        }
        if (sample_value > 192)
        {
            LATBbits.LATB4 = 1;
        }   
    
//    //OUTPUT the ID
//    char ID_value = trpBufferId[trpBufferIndex];
//    LATBbits.LATB5 = 0;
//    LATBbits.LATB6 = 0;
//    LATBbits.LATB7 = 0;
//    if (ID_value == 1)
//    {
//        LATBbits.LATB5 = 1;
//        LATBbits.LATB6 = 0;
//        LATBbits.LATB7 = 0;
//    }
//    else if (ID_value == 2)
//    {
//        LATBbits.LATB5 = 0;
//        LATBbits.LATB6 = 1;
//        LATBbits.LATB7 = 0;
//    }
//    else if (ID_value == 3)
//    {
//        LATBbits.LATB5 = 1;
//        LATBbits.LATB6 = 1;
//        LATBbits.LATB7 = 0;
//    }
//    else if (ID_value > 3)
//    {
//        LATBbits.LATB5 = 0;
//        LATBbits.LATB6 = 0;
//        LATBbits.LATB7 = 1;
//    }
    }
}