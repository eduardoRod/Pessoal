/*
 * File:   SI_Cap1_T2_main.c
 * Author: Eduardo Rodrigues
 *
 * Created on September 13, 2018, 6:18 PM
 */

/*
 *                             pic24fv16km202
 *                              _____________
 *                     Pin 1 --| 0           |--      AVDD
 *       BUZZ / RA0 /  Pin 2 --|             |--      AVSS
 *                     Pin 3 --|             |-- Pin 26 / RB15 
 *               RB0 / Pin 4 --|             |-- Pin 25 / RB14 
 *               RB1 / Pin 5 --|             |-- Pin 24 / RB13 
 *               RB2 / Pin 6 --|             |-- Pin 23 
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
#include <p24FV16KM202.h>

#define SW1 PORTBbits.RB10
#define SW2 PORTBbits.RB11
#define ON_OFF PORTBbits.RB12
#define BUZZ LATAbits.LATA0


inline void ConfigCLK(void);
inline void ConfigIO(void);

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    
    while(1){
       
        if(SW2 == 1){
            if((SW1 == 1)){
                BUZZ = 0;
            }
        }else{
            BUZZ = 1;
        }
    }
    return 0;
}

inline void ConfigCLK(void){
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.RCDIV = 0;
}

inline void ConfigIO(void){
    
    TRISB = 0;
    TRISBbits.TRISB10 = 1;
    TRISBbits.TRISB11 = 1;
    TRISBbits.TRISB12 = 1;
    
    TRISAbits.TRISA0 = 0;
}