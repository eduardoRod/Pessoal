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
 *                     Pin 3 --|             |-- Pin 26 / RB0 / CN11 / X1 
 *                     Pin 4 --|             |-- Pin 25 / RB14 / AN10 / PotX 
 *      Y-Step / RB1 / Pin 5 --|             |-- Pin 24 / RB13 / AN11 / PotY 
 *                     Pin 6 --|             |-- Pin 23 
 *    X2 / RB1 / CN5 / Pin 7 --|             |-- Pin 22 / RB11 / X-Step
 *                  VSS      --|             |-- Pin 21 / RB10 / X-Dir
 *   Y1 / RA2 / CN30 / Pin 9 --|             |-- Pin 20
 *  Y2 / RA3 / CN29 / Pin 10 --|             |-- Pin 19 / RA7 / Y-Dir
 *                    Pin 11 --|             |-- Pin 18 / 
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

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);
inline void ConfigCCP1(void);
inline void UART1Init(unsigned long int);
uint8_t putChar1(uint8_t);
uint8_t getChar1(void);
uint8_t UART1_Has_Data(void);
uint8_t hasChar(uint8_t*, uint8_t, uint8_t);
inline void Command_Init(void);
void Command_Do(uint8_t *, uint8_t);
float Command_Get_Param(uint8_t *, uint8_t, uint8_t);
inline void Stepper_Init(void);
inline void Plotter_Init(void);
void Plotter_Go_Home(void);
void Plotter_Go_To(uint32_t, uint32_t);
inline void ConfigADC(void);
unsigned int readADC(unsigned int ch);
/*
 * Timming and Debounce
 */
uint32_t time = 0, time_ms = 0;
uint32_t last_ms = 0;
uint8_t X_min_cnt = 0, X_max_cnt = 0, Y_min_cnt = 0, Y_max_cnt = 0;
uint8_t X_min = 0, X_max = 0, Y_min = 0, Y_max = 0;

/*
 * G-Code Commands
 */
uint8_t command[MAX_COMMAND];
uint8_t received = 0;
uint8_t command_count;

Stepper MotorX, MotorY;

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigCCP1();
    UART1Init(19200);
    
    ConfigADC();
    
    Command_Init();
    Stepper_Init();
    Plotter_Init();
    
    
    while(1){
        
        /*
         G-Code
         */
        if(UART1_Has_Data()){
            
            received = getChar1();
            
            if(received != '\n'){
                command[command_count] = received;
                command_count++;
            }
        }else{
            
            if((received == '\n') && command_count){
                
                Command_Do(command, command_count);
                Command_Init();
                
            }
            
        }
        
        /*
         Update Steppers
         */
        if(Stepper_Step(&MotorX, X_min, X_max, time)){
            Stepper_Calc_New_Speed(&MotorX, 0, 0);
        }
        if(Stepper_Step(&MotorY, Y_min, Y_max, time)){
            Stepper_Calc_New_Speed(&MotorY, 0, 0);
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
    //TRISBbits.TRISB8 = 0;   // Servo
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
        
        X_min = (X_min_cnt >= MIN_DEBOUNCE_TIME);
        X_max = (X_max_cnt >= MIN_DEBOUNCE_TIME);
        Y_min = (Y_min_cnt >= MIN_DEBOUNCE_TIME);
        Y_max = (Y_max_cnt >= MIN_DEBOUNCE_TIME);
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

inline void Command_Init(void){
    uint8_t i;
    
    for(i = 0; i < MAX_COMMAND; i++){
        command[i] = 0;
    }
    command_count = 0;
}

void Command_Do(uint8_t *command, uint8_t size){
    
    if(hasChar(command, 'G', size))
    {

        int8_t code = (int)Command_Get_Param(command, 'G', size);
        float X_Pos, Y_Pos, Z_Pos;

        switch(code){
            case 0: // G0
            case 1: // G1
                
                X_Pos = Command_Get_Param(command, 'X', size);
                Y_Pos = Command_Get_Param(command, 'Y', size);
                
                if(hasChar(command, 'Z', size)){
                    Z_Pos = (int16_t)(Command_Get_Param(command, 'Z', size));
                    if(Z_Pos == 0){
                        CCP1RB = 300;
                    }else{
                        CCP1RB = 200;
                    }
                }
                
                if(X_Pos < 0){
                    X_Pos = MotorX.CurrentPos;
                }else{
                    X_Pos = X_Pos * (float)MM_TO_STEP;
                }
                if(Y_Pos < 0){
                    Y_Pos = MotorY.CurrentPos;
                }else{
                    Y_Pos = Y_Pos * (float)MM_TO_STEP;
                }
                
                Plotter_Go_To((uint32_t)X_Pos, (uint32_t)Y_Pos);
                break;
                
            case 28:
                Plotter_Go_Home();
                break;

            default:
                putChar1('\n');
                putChar1('?');
                putChar1('\n');
                break;

        }
    }
}

float Command_Get_Param(uint8_t *command, uint8_t key, uint8_t size){
    uint8_t i, k;
    char temp[10] = "";
    
    for(i = 0; i < size; i++){
        
        if(command[i] == key){
            
            for(k = 0, i++; (i < size) && (k < 10); k++, i++){
                
                if((command[i] == '\n') || (command[i] == ' '))
                    break;
                
                temp[k] = command[i];
            }
            
            return atof(temp);
        }
    }
    
    return -1;
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

inline void Plotter_Init(void){

    Plotter_Go_Home();
}

void Plotter_Go_Home(void){
    float X_Aux_Speed, Y_Aux_Speed;
    
    X_Aux_Speed = MotorX.MaxSpeed;
    Y_Aux_Speed = MotorY.MaxSpeed;
    
    Stepper_Move(&MotorX, (-60000)); // Move -60000 Steps
    Stepper_Move_At(&MotorX, (float)(X_Aux_Speed/2));
    Stepper_Calc_New_Speed(&MotorX, 0, 0);
    Stepper_Move(&MotorY, (-60000)); // Move -60000 Steps
    Stepper_Move_At(&MotorY, (float)(Y_Aux_Speed/2));
    Stepper_Calc_New_Speed(&MotorY, 0, 0);
    
    while(!X_min){
        if(Stepper_Step(&MotorX, X_min, X_max, time)){
            Stepper_Calc_New_Speed(&MotorX, 0, 0);
        }
    }
    while(!Y_min){
        if(Stepper_Step(&MotorY, Y_min, Y_max, time)){
            Stepper_Calc_New_Speed(&MotorY, 0, 0);
        }
    }
    
    MotorX.CurrentPos = 0;
    MotorY.CurrentPos = 0;
    
    Stepper_Move(&MotorX, 600);
    Stepper_Move_At(&MotorX, (float)(X_Aux_Speed/5));
    Stepper_Calc_New_Speed(&MotorX, 0, 0);
    Stepper_Move(&MotorY, 600);
    Stepper_Move_At(&MotorY, (float)(Y_Aux_Speed/5));
    Stepper_Calc_New_Speed(&MotorY, 0, 0);
    
    while((MotorX.CurrentPos != 600) || (MotorY.CurrentPos != 600)){
        if(Stepper_Step(&MotorX, X_min, X_max, time)){
            Stepper_Calc_New_Speed(&MotorX, 0, 0);
        }
        if(Stepper_Step(&MotorY, Y_min, Y_max, time)){
            Stepper_Calc_New_Speed(&MotorY, 0, 0);
        }
    }
    
    Stepper_Move(&MotorX, (-800));
    Stepper_Calc_New_Speed(&MotorX, 0, 0);
    Stepper_Move(&MotorY, (-800));
    Stepper_Calc_New_Speed(&MotorY, 0, 0);
    
    while(!X_min){
        if(Stepper_Step(&MotorX, X_min, X_max, time)){
            Stepper_Calc_New_Speed(&MotorX, 0, 0);
        }
    }
    while(!Y_min){
        if(Stepper_Step(&MotorY, Y_min, Y_max, time)){
            Stepper_Calc_New_Speed(&MotorY, 0, 0);
        }
    }
    
    MotorX.CurrentPos = 0;
    MotorX.SetPos = 0;
    MotorY.CurrentPos = 0;
    MotorY.SetPos = 0;
    
    Stepper_Move_At(&MotorX, (float)X_Aux_Speed);
    Stepper_Calc_New_Speed(&MotorX, 0, 0);
    Stepper_Move_At(&MotorY, (float)Y_Aux_Speed);
    Stepper_Calc_New_Speed(&MotorY, 0, 0);
}

void Plotter_Go_To(uint32_t X_Pos, uint32_t Y_Pos){
    
    int32_t dX_steps, dY_steps;
    float diferential, X_New = 0, Y_New = 0, X_Inc, Y_Inc;
    
    dX_steps = X_Pos - MotorX.CurrentPos;
    dY_steps = Y_Pos - MotorY.CurrentPos;
    
    if(dX_steps == 0){
        Stepper_Move_To(&MotorY, ((int32_t)(Y_Pos)));
        Stepper_Calc_New_Speed(&MotorY, 0, 0);
        return;
    }else if(dY_steps == 0){
        Stepper_Move_To(&MotorX, ((int32_t)(X_Pos)));
        Stepper_Calc_New_Speed(&MotorX, 0, 0);
        return;
    }else{
        
        diferential = ((float)dY_steps/(float)dX_steps);
        
        if((diferential == 1) || (diferential == -1)){
            Stepper_Move_To(&MotorX, ((int32_t)(X_Pos)));
            Stepper_Calc_New_Speed(&MotorX, 0, 0);
            Stepper_Move_To(&MotorY, ((int32_t)(Y_Pos)));
            Stepper_Calc_New_Speed(&MotorY, 0, 0);
            return;
        }else{
            
            if((diferential > -1) && (diferential < 1)){
                if(dX_steps > 0){
                    X_Inc = 1;
                    Y_Inc = diferential;
                }else{
                    X_Inc = -1;
                    Y_Inc = -diferential;
                }
            }else if(dY_steps > 0){
                X_Inc = 1/diferential;
                Y_Inc = 1;
            }else{
                X_Inc = -1/diferential;
                Y_Inc = -1;
            }
            
//            if(diferential > 0){
//                if(diferential < 1){
//                    X_Inc = 1;
//                    Y_Inc = diferential;
//                    
//                }else{
//                    X_Inc = 1/diferential;
//                    Y_Inc = 1;
//                    
//                }
//            }else{
//                if(diferential > -1){
//                    X_Inc = 1;
//                    Y_Inc = diferential;
//                    
//                }else{
//                    X_Inc = -1/diferential;
//                    Y_Inc = -1;
//                    
//                }
//            }
        }
    }
    
    X_New = MotorX.CurrentPos;
    Y_New = MotorY.CurrentPos;
    
    while((MotorX.CurrentPos != X_Pos) || (MotorY.CurrentPos != Y_Pos)){
        
        if(MotorX.CurrentPos != X_Pos){
            X_New += X_Inc;
        }
        if(MotorY.CurrentPos != Y_Pos){
            Y_New += Y_Inc;
        }
        
        if(Stepper_Move_To(&MotorX, (int32_t)(X_New))){
            Stepper_Calc_New_Speed(&MotorX, 1, (X_Pos - MotorX.CurrentPos));
            // Wait for the step
            while(!Stepper_Step(&MotorX, X_min, X_max, time));
        }
        
        if(Stepper_Move_To(&MotorY, (int32_t)(Y_New))){
            Stepper_Calc_New_Speed(&MotorY, 1, (Y_Pos - MotorY.CurrentPos));
            // Wait for the step
            while(!Stepper_Step(&MotorY, Y_min, Y_max, time));
        }
    }
    
    
//    /*
//     * Bresenham's line algorithm
//     */
//    error = dX_steps - dY_steps;
//    
//    do{
//        if(X_Updated && Y_Updated){
//            X_Updated = 0;
//            Y_Updated = 0;
//            
//            error2 = 2 * error;
//        
//            if(error2 > (-dY_steps)){
//                error -=dY_steps;
//                Stepper_Move(&MotorX, sx);
//            }
//
//            if(error2 < dX_steps){
//                error += dX_steps;
//                Stepper_Move(&MotorY, sy);
//            }
//        }
//        
//        /*
//         Update Steppers
//         */
//        if(Stepper_Step(&MotorX, X_min, X_max, time))
//            X_Updated = 1;
//        if(Stepper_Step(&MotorY, Y_min, Y_max, time))
//            Y_Updated = 1;
//        
//    }while((MotorX.CurrentPos != dX_steps) && (MotorY.CurrentPos != Y_Pos));
    
}