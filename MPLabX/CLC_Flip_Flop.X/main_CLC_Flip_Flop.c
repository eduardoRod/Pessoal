/*
 * File:   main_CLC_Flip_Flop.c
 * Author: Eduardo Rodrigues
 *
 * Created on April 26, 2018, 11:39 PM
 */


/*
 *                             pic24fv16km202
 *                               ___________
 *                     Pin 1 --| 0           |--      AVDD
 *                     Pin 2 <-|             |--      AVSS
 *       (Dir 1) RA1 / Pin 3 --|             |<- Pin 26 / TCKIA (CCP2 <- encoder 1)
 *              PGD1 / Pin 4 --|             |<- Pin 25 / AN10 (RB14 -> Potenciometer)
 *              PGC1 / Pin 5 --|             |-- Pin 24
 *                     Pin 6 --|             |-- Pin 23
 *                     Pin 7 --|             |-- Pin 22
 *                  VSS      --|             |-- Pin 21
 *                     Pin 9 --|             |-- Pin 20
 *      (Dir 1) RA3 / Pin 10 ->|             |-- Pin 19
 *                    Pin 11 --|             |-- Pin 18
 *                    Pin 12 --|             |-> Pin 17 / OC1B (CCP1 ->PWM 1)
 *                  VDD      --|             |-- Pin 16
 *           CLCINA / Pin 14 --| ___________ |-- Pin 15 / CLCINB
 */


#include "xc.h"
#include "config.h"
#include <stdio.h>

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);

inline void UART1Init(unsigned long int baud);
inline void ConfigCLC1(void);

uint16_t active = 0, CLC1_out;

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    UART1Init(19200);
    ConfigCLC1();
    
    while(1){
        if(active){
            
            CLC1_out = CLC1CONLbits.LCOUT;
            printf("phase = %d\n\r", CLC1_out);
            
            active = 0;
        }
    }
    
    return 0;
}

inline void ConfigCLK(void) {
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.RCDIV = 0;
}

inline void ConfigIO(void) {
    ANSA = 0;

    ANSB = 0;

}

inline void UART1Init(unsigned long int baud) {
    U1BRG = (FCY / (16 * baud)) - 1; //BAUDRATEREG1;
    U1MODE = 0;
    U1MODEbits.BRGH = 0;
    U1STA = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXISEL = 2;
    U1MODEbits.UEN = 3;
}

inline void ConfigTMR1(void) {
    T1CONbits.TON = 1;
    T1CONbits.TCKPS = 0b11; // prescaler 1:256
    
    TMR1 = 0;
    PR1 = 62500 - 1; // 1s TTick

    _T1IF = 0;
    _T1IE = 1; // Activa interrupcoes
}

void _ISRFAST _T1Interrupt(void) {
    active = 1;
    _T1IF = 0;
}

inline void ConfigCLC1(void){
    
    CLC1CONLbits.MODE = 0b100;  // 1 input D-FF
    CLC1SELbits.DS1 = 0;        // CLCINA
    CLC1SELbits.DS2 = 0;        // CLCINB
    
    CLC1GLSLbits.G1D1T = 1;     // Data Set 1 -> Gate 1 -> Clock
    CLC1GLSLbits.G2D2T = 1;     // Data Set 2 -> Gate 2 -> Data
    //CLC1GLSHbits.G3D3T = 0;
    //CLC1GLSHbits.G4D4T = 0;
    
    CLC1CONLbits.LCEN = 1;
    
}