/*
 * File:   buzzer.c
 * Author: User
 *
 * Created on 2021?1?12?, ?? 12:12
 */

#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit 
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON   // Brown-out Reset Enable bit
#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
#define _XTAL_FREQ 4000000


void main(void) {
    int a;
  // ??TIMER2?prescaler=16?postscale=1:1
    T2CON = 0b00000110;

    // ??OSC???Fosc=8M 
    OSCCONbits.IRCF = 0b111;
    
    // ??CCP1?PWM
    CCP1CONbits.CCP1M = 0b1100;
    
    // ?RC2??????????(??CCP1?RC2????port)
    TRISC = 0;
    LATC = 0;
    TRISD = 0;  //RD??????
    LATD = 0;
    
     //set timer interrupt bit in PIE1 register
    INTCON = 0b11010000;
    INTCON2 = 0b01110001;
    PIR1bits.TMR2IF = 1;
    RCONbits.IPEN=0x01;   
    int arrEnd[24] = {238,189,158,118,94,79,94,
                    238,200,149,118,99,74,99,
                    212,178,133,105,88,66,66,66,66,59};
    
    int arr2[38] = {158, 158, 189, 238, 158, 158, 189, 238,
                    212, 189, 178, 178, 189, 178, 158, 158, 
                    158, 189, 158, 189, 212, 189, 238,
                    178, 212, 212, 212, 189, 238, 238, 238,
                    212, 189, 178, 212, 238, 255 , 238
                    };
    int arrWorking[7] = {94,94,94,118,94,79,158};  //{189,189,189,238,189,158,255}
    for(int i = 0;i < 24 ;i++){
        PR2 = arrEnd[i];
        CCPR1 = PR2>>1;
        LATD = 0x00;
        
        if(i==0 || i==7 || i==14){
            LATDbits.LATD0 = 1;
        }
        else if(i==1 || i==8 || i==15){
            LATDbits.LATD1 = 1;
        }
        else if(i==2 || i==9 || i==16){
            LATDbits.LATD2 = 1;
        }
        else if(i==3 || i==10 || i==17){
            LATDbits.LATD3 = 1;
        }
        else if(i==4 || i==11 || i==18){
            LATDbits.LATD4 = 1;
        }
        else if(i==5 || i==12 || i==19){
            LATDbits.LATD5 = 1;
        }
        else if(i==6 || i==13 || i==20){
            LATDbits.LATD6 = 1;
        }
        if(i < 5 || (i>6 && i<12) || (i>13 && i<19)){
            __delay_ms(200);
            CCPR1 = 0;
            __delay_ms(20);
        }
            
        else if(i==5 || i==6 || i==12 || i==13) {
            __delay_ms(600);
            CCPR1 = 0;
            __delay_ms(50);
        }            
        else if(i==19){
            CCPR1 = 0;
            __delay_ms(40);
            CCPR1 = PR2>>1;
            __delay_ms(600);
            CCPR1 = 0;
            __delay_ms(40);
        } 
        else if (i == 20){
             __delay_ms(200);
            CCPR1 = 0;
            __delay_ms(40);
        }
        else if(i == 21 || i == 22){
            LATD = 0xFF;
            __delay_ms(200);
            CCPR1 = 0;
            LATD = 0x00;
            __delay_ms(40);
        } 
        else if(i == 23){
            LATD = 0xFF;
            __delay_ms(1200);
        } 
    } 
    LATD = 0x00;
    CCPR1 = 0;
    while(1);
   
    return;
}

