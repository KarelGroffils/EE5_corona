#include "analog_in_app.h"

unsigned int timer_togle = 0;
//unsigned int sample_value = 0;

void initializeAnalogIn(void)
{ 
    //CONFIGURE I/O PORTS
    LATA = 0x00; //Initial PORTA
    TRISA = 0xFF; //Define PORTA as input
    ANSELA = 0xFF; //Disable digital input buffer
    //ADCON1 = 0x0E; // Turn off ADcon
    
    //0x07; 
    //TURN OFF COMPARATOR
    CM1CON0bits.C1ON  = false; //Turn off Comparator 1
    CM2CON0bits.C2ON  = false; 
    
    //SET VOLTAGE REFERENCE
    VREFCON0bits.FVREN = 1; //enable voltage reference
    VREFCON0bits.FVRS = 0b11; //select 4.096V
    
    //CONFIGURE INTERRUPTS
	INTCONbits.GIE = 0;	// Turn Off global interrupt
    PIE1bits.ADIE = 1; //enable ADC interupt
    IPR1bits.ADIP = 1; //set ADC high priority
    INTCONbits.TMR0IE = 1; //enable timer0 overflow interrupt
    INTCON2bits.TMR0IP = 1; //set timer0 high priority
    RCONbits.IPEN = 1; //enable interupt priority
    
    //CONFIGURE ADC
    ADCON0bits.ADON = 1; //ADC enabled //changed from disabled
    ADCON0bits.CHS = 0b00000; //select channel AN0
    ADCON1bits.PVCFG = 0b10; //select FVR (fixed voltage reference)
    ADCON2bits.ADFM = 0; //left justified (8bit precision) e.g. xxxx xxxx xx00 0000
    ADCON2bits.ACQT = 0b111; //20 cycle acquisition time
    ADCON2bits.ADCS = 0b110; //Fosc/64 conversion time

    //CONFIGURE TIMER   
    T0CONbits.TMR0ON = 0; //timer disabled
    T0CONbits.T08BIT = 1; //8 bit modus
    T0CONbits.T0CS = 0; //internal clock
    T0CONbits.PSA = 0; //enable prescaler
    T0CONbits.T0PS = 0b111; //1:256 prescaler 
    /*************************************************
    Timer freq = Fosc/(4*prescaler*(timer values))
    e.g. 1MHz/(4*256*256) = 4Hz meting elke 250ms
    **************************************************/
    TMR0L = 0x00;    //Initialize the timer value
    
    
    //CONFIGURE OSCILLATOR -> standard 1MHz
    
    //SET VARIABLES
    //trpBufferIndex = 0;
    
}

void startSamplingSensor(void)
{    
    //sprintf("check");
           
    PIR1bits.ADIF = 0; //clear ADC flag
    INTCONbits.TMR0IF=0;  //clear timer flag
    ADCON0bits.ADON = 1;  //start ADC
    T0CONbits.TMR0ON = 1;  //start Timer 0
    
    //INTCONbits.GIE = 1; //enable global interrupt
}

static void newSample(void)
{ 
    char dataByte = ADRESH;
    if(dataByte > 250)
    {
        dataByte = 250;
    }
    trpBufferSend[trpBufferSendIndex] = ADRESH;
    trpBufferSendID[trpBufferSendIndex] = 3;
    trpBufferSendIndex++;
    if(trpBufferSendIndex >= BUFFER_SIZE)
    {
        trpBufferSendIndex = BUFFER_SIZE-1;
    }
    LATBbits.LATB1 = timer_togle;
}

static void timer0Elapsed(void)
{ 
    timer_togle = !timer_togle;
    LATBbits.LATB0 = timer_togle;
    ADCON0bits.GO = 1;  //restart AD
}


void analogInInterupt(void)
{
    if(PIR1bits.ADIF  == 1) //ADC interupt
    {        
        newSample();
        PIR1bits.ADIF  = 0; //clear interupt flag
    } 
    
    if(INTCONbits.TMR0IF == 1) //timer0 interupt
     {
        TMR0L = 0x00;
        timer0Elapsed();
        INTCONbits.TMR0IF=0;     //clear interrupt flag
     }  
}
