#include <xc.h>
#include <stdint.h>        // include stdint header
#include <pic18f4520.h>

#define _XTAL_FREQ 4000000

#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
//#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF          // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)
         
__bit nec_ok;
uint8_t nec_state, bit_n, mode;
uint16_t timer_value;
uint32_t nec_code;

void GetIRCode(){
    if(nec_state != 0)
    {
        timer_value = (TMR1H << 8) | TMR1L;  // store Timer1 value
        TMR1H = TMR1L = 0;     // reset Timer1
    }
    switch(nec_state)
    {
        case 0 :              // start receiving IR data (we're at the beginning of 9ms pulse)
            TMR1H = TMR1L = 0;  // reset Timer1
            TMR1ON = 1;         // enable Timer1
            nec_state = 1;      // next state: end of 9ms pulse (start of 4.5ms space)
            bit_n = 0;
            break;
        case 1 :                                       
            if((timer_value > 13000) && (timer_value < 14000)) // 13.5ms
            { 
                nec_state = 2; 
            }
            else{  // invalid interval
                nec_state = 0;  // reset decoding process
                TMR1ON = 0;     // disable Timer1
            }              
            break;
        case 2 :
            if((timer_value > 2600) || (timer_value < 900))
            {  // invalid interval ==> stop decoding and reset
                TMR1ON = 0;     // disable Timer1
                nec_state = 0;  // reset decoding process
            }
            else
            {
                if( timer_value > 1750)  // 1.125 ms
                  nec_code |=   (uint32_t)1 << (31 - bit_n);
                else    // 2.25ms
                  nec_code &= ~((uint32_t)1 << (31 - bit_n));

                bit_n++;

                if(bit_n > 31)
                {
                  nec_ok = 1;   // decoding process OK
                  INT0IE = 0;
                }
                break;
            } 
    }
    return;
}

void initPWM(double freq, int duty) { //freq duty% channl
    int TMR2Pre = 16;
    PR2 = 8000000 / (4 * freq * TMR2Pre) - 1;
    int ccpr1l_ccp1con;
    int dc_max = 8000000 / (1023 * TMR2Pre);
    int dc = 100 * freq / duty;

    ccpr1l_ccp1con = 8000000 / dc * TMR2Pre;
    CCPR1L = (ccpr1l_ccp1con & 0b0000111111111100) >> 2;
    CCP1CONbits.DC1B1 = ccpr1l_ccp1con & 2;
    CCP1CONbits.DC1B0 = ccpr1l_ccp1con & 1;
    return;
}

void Backward(){
    LATC4 = 1;
    LATC5 = 0;
    LATC6 = 1;
    LATC7 = 0;
    
    LATD3 = 0;
    LATD2 = 1;
    LATD1 = 1;
    LATD0 = 0;
    return;
}

void Forward(){
    LATC4 = 0;
    LATC5 = 1;
    LATC6 = 0;
    LATC7 = 1; 
    
    LATD3 = 0;
    LATD2 = 1;
    LATD1 = 0;
    LATD0 = 1;
    return;
}

void Stop(){
    LATC4 = 0;
    LATC5 = 0;
    LATC6 = 0;
    LATC7 = 0;
    
    LATD3 = 0;
    LATD2 = 0;
    LATD1 = 0;
    LATD0 = 0;
    return;
}

void Left(){
    LATC4 = 0;
    LATC5 = 1;
    LATC6 = 0;
    LATC7 = 0;
    
    LATD3 = 0;
    LATD2 = 0;
    LATD1 = 1;
    LATD0 = 1;
    return;
}

void Right(){
    LATC4 = 0;
    LATC5 = 0;
    LATC6 = 0;
    LATC7 = 1;
    
    LATD3 = 0;
    LATD2 = 1;
    LATD1 = 0;
    LATD0 = 0;
    return;
}
 
void __interrupt() MY_ISR(void)
{
    //receive IR
    if (INT0IF)
    {
        INT0IF = 0;
        GetIRCode();
    } 
    if (TMR1IF)         // Timer1 ISR
    {
      TMR1IF    = 0;   // clear Timer1 overflow flag bit
      nec_state = 0;   // reset decoding process
      TMR1ON    = 0;   // disable Timer1
    }
    return;
} 

short Arrive(){
    TMR3H = 0;                  //Sets the Initial Value of Timer  //0????
    TMR3L = 0;                  //Sets the Initial Value of Timer
    LATD4 = 1;               //TRIGGER HIGH    
    __delay_us(10);               //10uS Delay //?10us?????    
    LATD4 = 0;               //TRIGGER LOW
    while(!PORTCbits.RC0);           //Waiting for Echo //???????? //????1
    TMR3ON = 1;               //Timer Starts
    while(PORTCbits.RC0);            //Waiting for Echo goes LOW //????0
    TMR3ON = 0;               //Timer Stops
    
    if(TMR3H < 2){
        // < 7cm
        LATD0 = ~LATD0;
        __delay_ms(100); 
        return 1;
    }
    else{
        LATD1 = ~LATD1;
        __delay_ms(100); 
        return 0;
    }
    return 0;
}

void main(void)
{
    OSCCON = 0x70;   // set internal oscillator to 8MHz
    
    // for IR
    TRISB0 = 1; // set RB0 as input       
    TMR1IF = 0;     // clear Timer1 overflow interrupt flag bit
    TMR1IE = 1;     // enable Timer1 overflow interrupt
    T1CON  = 0x10;  // set Timer1 clock source to internal with 1:2 prescaler (Timer1 clock = 1MHz)
    INT0IF = 0;
    INTCON = 0b11010000;  // enable global, peripheral and INT0 interrupts
    INTEDG0 = 0;
    nec_ok = 0;
    nec_state = 0;
    uint8_t command;
    
    // for test
    TRISD0 = 0;
    TRISD1 = 0;
    LATD0 = 1;
    
    // for tyre
    IPEN = 1;
    TRISC4 = 0;
    TRISC5 = 0;
    TRISC6 = 0;
    TRISC7 = 0;
    LATC4 = 0;
    LATC5 = 0;
    LATC6 = 0;
    LATC7 = 0;
    
    LATC = 0x00;
    T2CON = 0x07;
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    initPWM(1000, 50);
    
    //for track
    TRISD5 = 1;
    TRISD6 = 1;
    TRISD7 = 1;
    PORTDbits.RD5 = 0;
    PORTDbits.RD6 = 0;
    PORTDbits.RD7 = 0;
    
    // for control other pic18
    TRISD0 = 0;
    TRISD1 = 0;
    TRISD2 = 0;
    TRISD3 = 0;
    LATD0 = 0;
    LATD1 = 0;
    LATD2 = 0;
    LATD3 = 0;
    
    // for ultra
    T3CON = 0x10;
    TRISC0 = 1;
    TRISD4 = 0;
    LATD4 = 0;
    
    // for buzzer
    TRISC3 = 0;    
    mode = 0;
    
    while(1)
    {
        while (!nec_ok){    // wait until NEC code receiver 
            // tracking
            if(mode == 1){
                if(PORTDbits.RD5 == 1 || PORTDbits.RD6 == 1 || PORTDbits.RD7 == 1){ //black
                    LATC3 = 1;
                    LATD3 = 0;
                    LATD2 = 0;
                    LATD1 = 1;
                    LATD0 = 0;
                }    
                else{
                    LATC3 = 0;
                }
                if(Arrive()){
                    Stop();
                    LATD3 = 0;
                    LATD2 = 0;
                    LATD1 = 0;
                    LATD0 = 1;
                }
            }
            if(mode == 2){
                if(PORTDbits.RD5 == 1 && PORTDbits.RD7 == 0){ //left black, right white
                    Left();
                }   
                else if(PORTDbits.RD5 == 0 && PORTDbits.RD7 == 1){ 
                    Right();
                }   
                else{
                    Forward();
                }
            }                    
            
        }     
        nec_ok    = 0;   // reset decoding process
        nec_state = 0;
        TMR1ON    = 0;   // disable Timer1
        command = nec_code >> 8;
        
        if(command == 0b00011000){   // press 2 -> forward
            Forward();
        }
        else if(command == 0b00010000){  // press 4 -> left
            Left();
        }
        else if(command == 0b00111000){  // press 5 -> stop
            Stop();
        }
        else if(command == 0b01011010){  // press 6 -> right
            Right();
        }
        else if(command == 0b01001010){  // press 8 -> back
            Backward();
        }
        else if(command == 0b00110000){  // press 1 -> start
            mode = 1;
            LATD3 = 1;
            LATD2 = 1;
            LATD1 = 1;
            LATD0 = 0;
        }
        else if(command == 0b01111010){  // click 3 -> tracking
            mode = 2;
            LATD3 = 1;
            LATD2 = 1;
            LATD1 = 1;
            LATD0 = 1;
        }
        else if(command == 0b01000010){  // click 7 -> 7 color LED
            mode = 3;
            Stop();
            LATD3 = 1;
            LATD2 = 0;
            LATD1 = 1;
            LATD0 = 0;
        }
        else if(command == 0b01010010){  // click 9 -> breath LED
            mode = 4;
            Stop();
            LATD3 = 1;
            LATD2 = 0;
            LATD1 = 1;
            LATD0 = 1;
        }   
        INT0IE = 1;
    }
}
