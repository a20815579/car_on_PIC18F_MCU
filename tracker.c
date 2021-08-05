/*
 * File:   tracker.c
 * Author: User
 *
 * Created on 2021?1?10?, ?? 1:50
 */


#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit 
//#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON   // Brown-out Reset Enable bit
#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
#define _XTAL_FREQ 4000000


void main(void) {
    int a;
    LATD = 0x00;
    TRISD = 0x00;
    TRISC = 0x03;   //set RC0,RC1 to be input
    PORTC = 0x00;

    while(1){
        if(PORTCbits.RC0){
            LATDbits.LATD0 = 1;
             __delay_ms(10);
        }
        else    LATDbits.LATD0 = 0;
        
        if(PORTCbits.RC1){
            LATDbits.LATD1 = 1;
            __delay_ms(10);
        }
        else    LATDbits.LATD1 = 0;
    }
    return;
}