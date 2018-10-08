/*
 * File:   SI_Cap1_T3_main.c
 * Author: Eduardo Rodrigues
 *
 * Created on September 13, 2018, 6:18 PM
 */

/*
 *                             pic24fv16km202
 *                              _____________
 *                     Pin 1 --| 0           |--      AVDD
 *              RA0 /  Pin 2 --|             |--      AVSS
 *                     Pin 3 --|             |-- Pin 26 / RB15 
 *        LED1 / RB0 / Pin 4 --|             |-- Pin 25 / RB14 
 *        LED2 / RB1 / Pin 5 --|             |-- Pin 24 / RB13 
 *        LED3 / RB2 / Pin 6 --|             |-- Pin 23 / RB12
 *               RB3 / Pin 7 --|             |-- Pin 22 / RB11 / SW2
 *                  VSS      --|             |-- Pin 21 / RB10 / SW1
 *               RA2 / Pin 9 --|             |-- Pin 20
 *              RA3 / Pin 10 --|             |-- Pin 19 / RA7 
 *                    Pin 11 --|             |-- Pin 18 / 
 *                    Pin 12 --|             |-- Pin 17 / RB8 
 *                  VDD      --|             |-- Pin 16
 *                    Pin 14 --|_____________|-- Pin 15 
 */

#include "xc.h"
#include "config.h"
#include <stdlib.h>
#include <libpic30.h>

#define SW1 PORTBbits.RB10
#define SW2 PORTBbits.RB11
#define LED1 LATBbits.LATB0
#define LED2 LATBbits.LATB1
#define LED3 LATBbits.LATB2
#define LEDS LATB


inline void ConfigCLK(void);
inline void ConfigIO(void);

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    
    char sig = 1;
    
    while(1){
       
        if(SW1 == 1){
            
            sig = sig << 1;
            if((sig > 4) || (sig == 0)){
                sig = 1;
            }
        }else if(SW2 == 1){
            
            sig = sig >> 1;
            if(sig < 1){
                sig = 4;
            }
        }
        
        LEDS = sig;
        
        __delay_ms(500);
    }
    return 0;
}

inline void ConfigCLK(void){
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.RCDIV = 0;
}

inline void ConfigIO(void){
    
    ANSB = 0;
    TRISB = 0;
    TRISBbits.TRISB0 = 0;   // LED1
    TRISBbits.TRISB1 = 0;   // LED2
    TRISBbits.TRISB2 = 0;   // LED3
    TRISBbits.TRISB10 = 1;  // SW1
    TRISBbits.TRISB11 = 1;  // SW2
    
}