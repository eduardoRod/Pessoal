/*
 * File:   main_Single_Stepper.c
 * Author: Eduardo Rodrigues
 *
 * Created on May 23, 2018, 3:52 PM
 */

/*
 *                             pic24fv16km202
 *                               ___________
 *                     Pin 1 --| 0           |--      AVDD
 *         (Pot) RA0 / Pin 2 ->|             |--      AVSS
 *         (Dir) RA1 / Pin 3 ->|             |-> Pin 26 / RB15 (Motor 1a)
 *              PGD1 / Pin 4 --|             |-> Pin 25 / RB14 (Motor 1b)
 *              PGC1 / Pin 5 --|             |-> Pin 24 / RB13 (Motor 2a)
 *                     Pin 6 --|             |-> Pin 23 / RB12 (Motor 2b)
 *                     Pin 7 --|             |-- Pin 22
 *                  VSS      --|             |-- Pin 21
 *        (Mod0) RA2 / Pin 9 ->|             |-- Pin 20
 *       (Mod1) RA3 / Pin 10 ->|             |-- Pin 19
 *                    Pin 11 --|             |-- Pin 18
 *                    Pin 12 --|             |-- Pin 17 
 *                  VDD      --|             |-- Pin 16
 *                    Pin 14 --| ___________ |-- Pin 15 
 */


#include "xc.h"
#include "config.h"

#define DIR PORTAbits.RA1

typedef struct Stepper{ 
    uint8_t Mode;
    uint8_t Dir;
    int8_t Step;
    uint32_t stepWait;
    uint32_t lastStep;
    uint16_t MaxSpeed;
    uint16_t MinSpeed;
    uint16_t Speed;
    uint8_t stepFlag;
    
}Stepper; 
typedef Stepper *Stepp_handle;

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);
inline void ConfigADC(void);
unsigned int readADC(unsigned int ch);
inline void Stepper_Init(void);
void Stepper_Update(void);
void makeStep(Stepper *Motor, uint8_t *stepTable, uint8_t Max_Step);

uint8_t singlePhaseStep[4] = {0b0001, 0b0010, 0b0100, 0b1000};
uint8_t dualPhaseStep[4] = {0b0011, 0b0110, 0b1100, 0b1001};
uint8_t halfStep[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};

uint32_t time = 0;

Stepper Motor1;

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigADC();
    
    Stepper_Init();
    
    while(1){
        
        Stepper_Update();
        
        if(Motor1.stepFlag){
            
            switch(Motor1.Mode){
                // Single Phase - Full Step
                case 0:
                    makeStep(&Motor1, singlePhaseStep, 4);
                    break;
                
                // Dual Phase - Full Step
                case 1:
                    makeStep(&Motor1, dualPhaseStep, 4);
                    break;
                  
                // Half Step
                case 2:
                    makeStep(&Motor1, halfStep, 8);
                    break;
                
                // Stop - No break
                default:
                    LATB = (PORTB & 0x0fff);
            }
            
            Motor1.stepFlag = 0;
        }
    }
    
    return 0;
}

inline void ConfigCLK(void){
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.RCDIV = 0;
}

inline void ConfigIO(void){
    ANSA = 0;
    ANSAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;   // Potenciometro
    TRISAbits.TRISA1 = 1;   // Dir
    TRISAbits.TRISA2 = 1;   // Mod0
    TRISAbits.TRISA3 = 1;   // Mod1
    
    ANSB = 0;
    TRISBbits.TRISB15 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB12 = 0;
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
    
    if((time - Motor1.lastStep) >= Motor1.stepWait){
        Motor1.lastStep = time;
        Motor1.stepFlag = 1;
    }
    
    _T1IF = 0;
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

inline void Stepper_Init(void){
    Motor1.Dir = 1;
    Motor1.MaxSpeed = 10000; // 1000.0Hz
    Motor1.MinSpeed = 1;
    Motor1.Mode = 0;
    Motor1.Speed = 1;
    Motor1.lastStep = 0;
    Motor1.Step = 0;
    Motor1.stepFlag = 0;
    Motor1.stepWait = 1000000/Motor1.Speed;
}

void Stepper_Update(void){
    Motor1.Dir = PORTAbits.RA1;
    Motor1.Mode = (PORTAbits.RA3 << 1) | PORTAbits.RA2;
    Motor1.Speed = readADC(0)*(Motor1.MaxSpeed/4095);
    //Motor1.Speed = readADC(0)/(4095/Motor1.MaxSpeed);

    // 100mV -> ADC=81.9 -> Speed=99.92
    if(Motor1.Speed <= 100){
        Motor1.Speed = Motor1.MinSpeed;
        Motor1.Mode = 3;
    }
    Motor1.stepWait = 100000/Motor1.Speed;
}

void makeStep(Stepper *Motor, uint8_t *stepTable, uint8_t Max_Step){
    if(Motor->Dir){
        Motor->Step++;
        if(Motor->Step > Max_Step-1){
            Motor->Step = 0;
        }
    }else{
        Motor->Step--;
        if(Motor->Step < 0){
            Motor->Step = Max_Step-1;
        }
    }

    LATB = ((stepTable[Motor->Step] << 12) | (PORTB & 0x0fff));
}
