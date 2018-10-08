/*
 * File:   main_SI_Cap2_T2.c
 * Author: Eduardo Rodrigues
 *
 * Created on September 20, 2018, 7:13 PM
 */


#include "xc.h"
#include "config.h"
#include <libpic30.h>

inline void ConfigCLK(void);
inline void ConfigIO(void);

const char digit[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    
int main(void) {
    
    ConfigCLK();
    ConfigIO();
    
    unsigned char* digitOut = (unsigned char*) &LATB;
    digitOut++;
    
    int i;
    while(1){
        for(i = 0; i < 16; i++){
            *digitOut = digit[i];
            __delay_ms(500);
        }
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
    
}