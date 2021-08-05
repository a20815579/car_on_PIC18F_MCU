/*
 * File:   lab9.c
 * Author: huang
 *
 * Created on 2019?12?2?, ?? 3:47
 */

#include <xc.h>
#include <pic18f4520.h>
#define _XTAL_FREQ 4000000
#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit 
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON   // Brown-out Reset Enable bit
#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
int mode = 0;

void __interrupt(high_priority) ISR(void) {

    switch (mode) {
        case 0: //straight
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            LATDbits.LATD2 = 1;
            LATDbits.LATD3 = 0;
            break;
        case 1://right
            LATDbits.LATD0 = 1;
            LATDbits.LATD1 = 0;
            LATDbits.LATD2 = 0;
            LATDbits.LATD3 = 0;
            break;
        case 2://left
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 0;
            LATDbits.LATD2 = 1;
            LATDbits.LATD3 = 0;
            break;
        case 3:
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 0;
            LATDbits.LATD2 = 0;
            LATDbits.LATD3 = 0;
            break;
    }
    mode += 1;
    if (mode == 3) {
        mode = 0;
    }

    __delay_ms(200);
    INTCONbits.INT0IF = 0;
    return;
}

void initPWM(double freq, int duty) { //freq duty% channl
    int TMR2Pre = 1;
    if (T2CONbits.T2CKPS == 1) {
        TMR2Pre = 4;
    } else if (T2CONbits.T2CKPS > 1) {
        TMR2Pre = 16;
    }
    PR2 = _XTAL_FREQ / (4 * freq * TMR2Pre) - 1;
    int ccpr1l_ccp1con;
    int dc_max = _XTAL_FREQ / (1023 * TMR2Pre);
    int dc = 100 * freq / duty;

    if (channel == 1) {
        ccpr1l_ccp1con = _XTAL_FREQ / dc * TMR2Pre;
        CCPR1L = (ccpr1l_ccp1con & 0b0000111111111100) >> 2;
        CCP1CONbits.DC1B1 = ccpr1l_ccp1con & 2;
        CCP1CONbits.DC1B0 = ccpr1l_ccp1con & 1;
    }
}

void startPWM(int channel) {
    if (channel == 1) {
        TRISC = 0;
        CCP1CONbits.CCP1M3 = 1;
        CCP1CONbits.CCP1M2 = 1;
    }
    if (channel == 2) {
        TRISC = 0;
        CCP2CONbits.CCP2M3 = 1;
        CCP2CONbits.CCP2M2 = 1;
    }
    T2CON = 3;
    T2CONbits.TMR2ON = 1;
}

void main(void) {
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.INT0IE = 1;
    //mode change
    TRISB = 1;
    LATB = 0;
    TRISD = 0;
//    TRISC = 0;
    LATA = 0;
    LATD = 0b00001111;
    //    LATD = 0;
    //    LATC = 1;
    //        LATD = 0;
    //    TRISDbits.TRISD0 = 0;
    //    TRISDbits.TRISD1 = 0;
    //    TRISDbits.TRISD2 = 0;
    //    TRISDbits.TRISD3 = 0;
    //   initPWM(500, 50, 1);
    //        startPWM(1);
    while (1) {
//        __delay_ms(200);
    }
    return;
}
// period = (PR2+1)*4*Tosc*(TMR2 prescaler) = (0x9b + 1) * 4 * 8µs * 4 = 0.019968s ~= 20ms
//(0x0b*4//8bit shift to left 2bit on the right id two bit)
// duty cycle = (CCPR1L:CCP1CON<5:4>)*Tosc*(TMR2 prescaler) = (0x0b*4 + 0b01) * 8µs * 4 = 0.00144s ~= 1450µs
