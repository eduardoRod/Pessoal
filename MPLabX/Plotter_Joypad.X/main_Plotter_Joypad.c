/*
 * File:   main_PlotterXY.c
 * Author: Eduardo Rodrigues
 *
 * Created on May 24, 2018, 3:42 PM
 */

/*
 *                             pic24fv16km202
 *                              _____________
 *                     Pin 1 --| 0           |--      AVDD
 *                     Pin 2 --|             |--      AVSS
 *                     Pin 3 --|             |-- Pin 26 / RB15 / CN11 / X1 
 *                     Pin 4 --|             |-- Pin 25 / RB14 / AN10 / PotX 
 *      Y-Step / RB1 / Pin 5 --|             |-- Pin 24 / RB13 / AN11 / PotY 
 *                     Pin 6 --|             |-- Pin 23 
 *    X2 / RB3 / CN5 / Pin 7 --|             |-- Pin 22 / RB11 / X-Step
 *                  VSS      --|             |-- Pin 21 / RB10 / X-Dir
 *   Y1 / RA2 / CN30 / Pin 9 --|             |-- Pin 20
 *  Y2 / RA3 / CN29 / Pin 10 --|             |-- Pin 19 / RA7 / Y-Dir
 *                    Pin 11 --|             |-- Pin 18 / RB9 / S1
 *                    Pin 12 --|             |-- Pin 17 / RB8 / Servo
 *                  VDD      --|             |-- Pin 16
 *                    Pin 14 --|_____________|-- Pin 15 
 */

#include "xc.h"
#include <stdlib.h>
#include <math.h>
#include "config.h"
#include "stepper.h"

#define MM_TO_STEP 43.745 // ((200*8)/36.576)
#define MICRO_STEP 8
#define MAX_SPEED (7500)
#define MAX_COMMAND 128

#define MIN_DEBOUNCE_TIME 5
#define X_MIN_END PORTBbits.RB15
#define X_MAX_END PORTBbits.RB3
#define Y_MIN_END PORTAbits.RA2
#define Y_MAX_END PORTAbits.RA3
#define S_BUTTON PORTBbits.RB9

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);
inline void ConfigCCP1(void);
inline void Stepper_Init(void);
inline void ConfigADC(void);
unsigned int readADC(unsigned int ch);

/*
 * Timming and Debounce
 */
uint32_t time = 0, time_ms = 0;
uint32_t last_ms = 0;
uint8_t X_min_cnt = 0, X_max_cnt = 0, Y_min_cnt = 0, Y_max_cnt = 0, S_btn_cnt = 0;
uint8_t X_min = 0, X_max = 0, Y_min = 0, Y_max = 0, S_btn = 0;

Stepper MotorX, MotorY;
int32_t VelX, VelY;

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigCCP1();
    
    ConfigADC();
    
    Stepper_Init();
    
    while(1){
        
        VelX = readADC(10)/409; // 0 - 10
        VelY = readADC(11)/409; // 0 - 10
        
        VelX = (VelX - 5) * 12000; // -60000 - 60000
        VelY = (VelY - 5) * 12000; // -60000 - 60000
        
        if(VelX != 0){
            
            MotorX.Speed = (float)VelX;
            
            if(VelX > 0){
                MotorX.StepWait = 1000000 / MotorX.Speed;
            }else{
                MotorX.StepWait = 1000000 / (-MotorX.Speed);
            }
        }
        
        if(VelY != 0){
            
            MotorY.Speed = (float)VelY;
            
            if(VelY > 0){
                MotorY.StepWait = 1000000 / MotorY.Speed;
            }else{
                MotorY.StepWait = 1000000 / (-MotorY.Speed);
            }
        }
        
        if(S_btn){
            CCP1RB = 300;
        }else{
            CCP1RB = 200;
        }
        
        /*
         Update Steppers
         */
        Stepper_Step(&MotorX, X_min, X_max, time);
        Stepper_Step(&MotorY, Y_min, Y_max, time);
        
    }
    
    return 0;
}

inline void ConfigCLK(void){
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.RCDIV = 0;
}

inline void ConfigIO(void){
    ANSA = 0;
    TRISAbits.TRISA2 = 1;   // Y1
    CNPU2bits.CN30PUE = 1;  // Pull-Up
    TRISAbits.TRISA3 = 1;   // Y2
    CNPU2bits.CN29PUE = 1;  // Pull-Up
    TRISAbits.TRISA7 = 0;   // Y-Dir
    
    ANSB = 0;
    ANSBbits.ANSB13 = 1;    // Potenc. Y
    ANSBbits.ANSB14 = 1;    // Potenc. X
    
    TRISBbits.TRISB1 = 0;   // Y-Step  
    TRISBbits.TRISB3 = 1;   // X2
    CNPU1bits.CN7PUE = 1;   // Pull-Up
    TRISBbits.TRISB9 = 1;   // Servo Button
    CNPU2bits.CN21PUE = 1;  // Pull-Up
    TRISBbits.TRISB10 = 0;  // X-Dir
    TRISBbits.TRISB11 = 0;  // X-Step
    TRISBbits.TRISB13 = 1;  // Potenc. Y
    TRISBbits.TRISB14 = 1;  // Potenc. X
    TRISBbits.TRISB15 = 1;  // X1
    CNPU1bits.CN11PUE = 1;  // Pull-Up
}

inline void ConfigTMR1(void){
    T1CON = 0x8000; // enabled, prescaler 1:1, 62.5 ns tck @ Fcy=16MHz
    TMR1 = 0;
    PR1 = 160-1; // 10us TTick

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
        
        if(!Y_MIN_END){
            if(++Y_min_cnt > MIN_DEBOUNCE_TIME){
                Y_min_cnt--;
            }
        }else{
            Y_min_cnt = 0;
        }
        
        if(!X_MAX_END){
            if(++X_max_cnt > MIN_DEBOUNCE_TIME){
                X_max_cnt--;
            }
        }else{
            X_max_cnt = 0;
        }
        
        if(!Y_MAX_END){
            if(++Y_max_cnt > MIN_DEBOUNCE_TIME){
                Y_max_cnt--;
            }
        }else{
            Y_max_cnt = 0;
        }
        
        if(!S_BUTTON){
            if(++S_btn_cnt > MIN_DEBOUNCE_TIME){
                S_btn_cnt--;
            }
        }else{
            S_btn_cnt = 0;
        }
        
        X_min = (X_min_cnt >= MIN_DEBOUNCE_TIME);
        X_max = (X_max_cnt >= MIN_DEBOUNCE_TIME);
        Y_min = (Y_min_cnt >= MIN_DEBOUNCE_TIME);
        Y_max = (Y_max_cnt >= MIN_DEBOUNCE_TIME);
        S_btn = (S_btn_cnt >= MIN_DEBOUNCE_TIME);
    }
    
    _T1IF = 0;
}

inline void ConfigCCP1(void){
    
    // CCP1CON1L
    CCP1CON1L = 0;
    CCP1CON1Lbits.CCPON = 1;
    CCP1CON1Lbits.TMRPS = 0b11; // 1:64 250 kHz
    CCP1CON1Lbits.MOD = 0b0101; // Dual Edge Compare Mode
    
    CCP1CON1H = 0;
    CCP1CON2L = 0;
    CCP1CON2H = 0;
    CCP1CON2Hbits.OCBEN = 1; // Pin 17 -> RB8 -> OC1B
    CCP1CON3L = 0;
    CCP1CON3H = 0;
    
    CCP1TMRL = 0;
    CCP1PRL = 5000; // 50 Hz / 20mS
    CCP1RA = 0;
    CCP1RB = 200;     // 1 mS
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
    MotorX.MaxSpeed = ((float)MAX_SPEED*(float)MICRO_STEP); // 750.0 steps/seg. * 8 microStep
    MotorX.StepMin = 1000000 / (float)MotorX.MaxSpeed;
    MotorX.Accel = 90000;
    MotorX.FirstStep = 470; //sqrt(2 / MotorX.Accel) * 100000
    MotorX.Speed = 0;
    MotorX.LastStepTime = 0;
    MotorX.StepWait = 0;
    MotorX.CurrentPos = 0;
    MotorX.DirPort = &LATB;
    MotorX.DirPin = 10;
    MotorX.StepPort = &LATB;
    MotorX.StepPin = 11;
    MotorX.PositiveDir = 1;
    
    MotorY.MaxSpeed = ((float)MAX_SPEED*(float)MICRO_STEP); // 750 steps/seg. * 8 microStep
    MotorY.StepMin = 1000000 / (float)MotorY.MaxSpeed;
    MotorY.Accel = 90000;
    MotorY.FirstStep = 470; //sqrt(2 / MotorX.Accel) * 100000
    MotorY.Speed = 0;
    MotorY.LastStepTime = 0;
    MotorY.StepWait = 0;
    MotorY.CurrentPos = 0;
    MotorY.DirPort = &LATA;
    MotorY.DirPin = 7;
    MotorY.StepPort = &LATB;
    MotorY.StepPin = 1;
    MotorY.PositiveDir = 0;
}
