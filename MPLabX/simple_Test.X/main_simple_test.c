/*
 * File:   main_simple_test.c
 * Author: Eduardo Rodrigues
 *
 * Created on May 17, 2018, 7:02 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <p24FV16KM202.h>
#include "xc.h"
#include "config.h"
#include <libpic30.h>

#define MIN_DEBOUNCE_TIME 5
#define X_MIN_END PORTBbits.RB15
#define X_MAX_END PORTBbits.RB3

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);
inline void ConfigCCP1(void);
inline void ConfigADC(void);
unsigned int readADC(unsigned int ch);

uint32_t time = 0, time_ms = 0;
uint32_t last_ms = 0;
uint32_t last_s = 0, time_s = 0;
uint8_t X_min_cnt = 0, X_max_cnt = 0;
uint8_t X_min = 0, X_max = 0;


int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigCCP1();
    ConfigADC();
    
    uint16_t val;
    
    while(1){
        
        
        val = readADC(0);
        __delay_us(10);
        
    }
    
    return 0;
}
 
inline void ConfigCLK(void){
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.RCDIV = 0;
}

inline void ConfigIO(void){
    ANSA = 0;
    ANSB = 0;
    
    ANSAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB3 = 1;   // X2
    CNPU1bits.CN7PUE = 1;   // Pull-Up
    TRISBbits.TRISB15 = 1;  // X1
    CNPU1bits.CN11PUE = 1;  // Pull-Up
}

inline void ConfigTMR1(void){
    T1CON = 0x8000; // enabled, prescaler 1:1, 62.5 ns tck @ Fcy=16MHz
    TMR1 = 0;
    PR1 = 160 - 1; // 10us TTick

    _T1IF = 0;
    _T1IE = 1; // Activa interrupcoes
}

void _ISRFAST _T1Interrupt(void) {
    time++;
    
    if(time >= last_ms + 100){
        last_ms = time;
        time_ms++;
        
        if(!X_MIN_END){
            if(++X_min_cnt > MIN_DEBOUNCE_TIME){
                X_min_cnt--;
            }
        }else{
            X_min_cnt = 0;
        }
        
        if(!X_MAX_END){
            if(++X_max_cnt > MIN_DEBOUNCE_TIME){
                X_max_cnt--;
            }
        }else{
            X_max_cnt = 0;
        }
        
        X_min = (X_min_cnt >= MIN_DEBOUNCE_TIME);
        X_max = (X_max_cnt >= MIN_DEBOUNCE_TIME);
        
        if(X_min && !PORTBbits.RB1){
            LATBbits.LATB1 = 1;
        }
        
        if(X_max && PORTBbits.RB1){
            LATBbits.LATB1 = 0;
        }
    }
    
    if(time_ms >=  last_s + 1000){
        last_s = time_ms;
        time_s++;
    }
    
    _T1IF = 0;
}

inline void ConfigCCP1(void){
    
    // CCP1CON1L
    CCP1CON1L = 0;
    CCP1CON1Lbits.CCPON = 1;
    CCP1CON1Lbits.TMRPS = 0b00; // 1:1 16 MHz
    CCP1CON1Lbits.MOD = 0b0101; // Dual Edge Compare Mode
    
    CCP1CON1H = 0;
    CCP1CON2L = 0;
    CCP1CON2H = 0;
    CCP1CON2Hbits.OCBEN = 1; // Pin 17 -> RB8 -> OC1B
    CCP1CON3L = 0;
    CCP1CON3H = 0;
    
    CCP1TMRL = 0;
    CCP1PRL = 4000; // 4 kHz / 20mS
    CCP1RA = 0;
    CCP1RB = 2000;     // 1 mS
}

inline void ConfigADC(void) {
    /*
     * Configura o ADC para funcionar comandado por SW
     * Consultar: Section 51 - 12-Bit AD Converter with Threshold Detect
     * e Ch 19 de PIC24FV16KM204 FAMILY Data Sheet
     */

    AD1CON1 = 0x0470; // 12-bit A/D operation (MODE12=1)
    // SSRC<3:0> = 0111 Internal counter ends sampling
    // and starts conversion (auto-convert)
    AD1CON2 = 0x0000; // Configure A/D voltage reference and buffer fill modes.
    // Vr+ and Vr- from AVdd and AVss(PVCFG<1:0>=00, NVCFG=0),
    // Inputs are not scanned,
    // Buffer in FIFO mode (BUFM=0)
    AD1CON3 = 0x1003; // Sample time = 16Tad, Tad = 4Tcy (= 250ns)

    AD1CSSL = 0; // No inputs are scanned.

    _AD1IF = 0; // Clear A/D conversion interrupt.
    _AD1IE = 0; // Disable A/D conversion interrupt
    AD1CON1bits.ADON = 1; // Turn on A/D
}

unsigned int readADC(unsigned int ch) {
    AD1CHS = ch; // Select analog input channel
    AD1CON1bits.SAMP = 1; // start sampling, then go to conversion

    while (!AD1CON1bits.DONE); // conversion done?
    return (ADC1BUF0); // yes then get ADC value
}

uint8_t UART1_Has_Data(void){

    return U1STAbits.URXDA;
}

uint8_t putChar1(uint8_t c){
    while ( U1STAbits.UTXBF);   // wait while Tx buffer full
    U1TXREG = c;
    return c;
}

uint8_t getChar1(void){
    
    return U1RXREG;             // read from the receive buffer
}

uint8_t hasChar(uint8_t *str, uint8_t character, uint8_t size){

    uint8_t i;

    for(i = 0; i < size; i++){
        if(str[i] == character){
            return 1;
        }
    }

    return 0;
}
