#define _XTAL_FREQ 4000000

#include <xc.h>
#include <pic18f4520.h>

// BEGIN CONFIG
#pragma config OSC = INTIO67 // Oscillator Selection bits (HS oscillator)
#pragma config WDT = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
//END CONFIG
void main()
{
  OSCCON = 0x70;
  TRISC0 = 1;
    TRISD4 = 0;
    LATD4 = 0;
  TRISD0 = 0;
  LATD0 = 1;
  TRISD1 = 0;
  LATD1 = 1;
  
  T3CON = 0x10;
  TMR3H = 0;                  //Sets the Initial Value of Timer  //0????
  TMR3L = 0;                  //Sets the Initial Value of Timer
  while(1)
  {
    TMR3H = 0;                  //Sets the Initial Value of Timer  //0????
    TMR3L = 0;                  //Sets the Initial Value of Timer
    LATD4 = 1;               //TRIGGER HIGH
    __delay_us(10);               //10uS Delay //?10us?????
    LATD4 = 0;               //TRIGGER LOW
    while(!PORTCbits.RC0);           //Waiting for Echo //???????? //????1
    TMR3ON = 1;               //Timer Starts
    while(PORTCbits.RC0);            //Waiting for Echo goes LOW //????0
    TMR3ON = 0;               //Timer Stops

    if(TMR3H < 1){
        // < 7cm
        LATD0 = ~LATD0;
    }
//      LATD0 = 0;
    __delay_ms(100); 
  }
}