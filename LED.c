/*
 * File:   LEDs.c
 * Author: User
 *
 * Created on 2021?1?13?, ?? 8:46
 */


#include <xc.h>
#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit 
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON   // Brown-out Reset Enable bit
#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
#define _XTAL_FREQ 4000000

void init_pwm();
void delayms(unsigned char x);
void light_control();
unsigned char flag=0;
unsigned int temp=0;
void main(void) {
    TRISD=0X00;
    LATD=0X00;
    init_pwm();
//    while(1)            light_control();
    while(1)
    {
        light_control();
        for(int i = 0;i < 7;i++){
            switch(i){
                case 0:
                    LATDbits.LATD0 = 1;
                    LATDbits.LATD1 = 0;
                    LATDbits.LATD2 = 0;
                    break;
                case 1:
                    LATDbits.LATD0 = 0;
                    LATDbits.LATD1 = 1;
                    LATDbits.LATD2 = 0;
                    break;
                case 2:
                    LATDbits.LATD0 = 0;
                    LATDbits.LATD1 = 0;
                    LATDbits.LATD2 = 1;
                    break;
                case 3:
                    LATDbits.LATD0 = 1;
                    LATDbits.LATD1 = 1;
                    LATDbits.LATD2 = 0;
                    break;
                case 4:
                    LATDbits.LATD0 = 1;
                    LATDbits.LATD1 = 0;
                    LATDbits.LATD2 = 1;
                    break;    
                case 5:
                    LATDbits.LATD0 = 0;
                    LATDbits.LATD1 = 1;
                    LATDbits.LATD2 = 1;
                    break;
                case 6:
                    LATDbits.LATD0 = 0;
                    LATDbits.LATD1 = 0;
                    LATDbits.LATD2 = 0;
                    break;                    
            }
           __delay_ms(800);
        }

    }
    return;
}
void init_pwm()
{
    T2CON=0X04;  //turn tme2 on. prescale = 1?1
    CCP1CON=0X3C; // ??CCP1?PWM
    CCP2CON=0X3c;
    OSCCONbits.IRCF = 0b111; // ??OSC???Fosc=8M Hz
    CCPR1L=0X00; 
    CCPR2L=0X00;
    PR2=199;
    TRISC=0XFF;
    TMR2IF=0;
    while(!TMR2IF);
    TRISC=0;

}
void light_control()
{   __delay_ms(15);
    if(flag==0)
    {
        CCPR1L++;
        CCPR2L = CCPR1L;
    }
    else
    {
        CCPR1L--;
        CCPR2L = CCPR1L;
    }
    temp=CCPR1L;
    if(temp==0X64){
        flag=1;
    }
    if(temp==0X00)
        flag=0;
}