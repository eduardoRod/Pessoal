/*
 * File:   main_controlador_posicao.c
 * Author: Eduardo Rodrigues
 *
 * Created on April 11, 2018, 2:31 PM
 * 
 * Ligações:
 * PIC24FV          MD10C
 * pin17 (OC1B)     PWM
 * pin2 (RA1)       DIR
 * 
 * pin25 AN10 (RB14)       Potenciometro
 * 
 */

/*
 *                             pic24fv16km202
 *                               ___________
 *                     Pin 1 --| 0           |--      AVDD
 *                     Pin 2 --|             |--      AVSS
 *       (Dir 1) RA1 / Pin 3 <-|             |<- Pin 26 / TCKIA (CCP2 <- encoder 1)
 *                     Pin 4 --|             |<- Pin 25 / AN10 (RB14 -> Potenciometer)
 *                     Pin 5 --|             |-- Pin 24
 *                     Pin 6 --|             |-- Pin 23
 *                     Pin 7 --|             |-- Pin 22
 *                  VSS      --|             |-- Pin 21
 *                     Pin 9 --|             |-- Pin 20
 *                    Pin 10 ->|             |-- Pin 19
 *                    Pin 11 --|             |-- Pin 18
 *                    Pin 12 --|             |-> Pin 17 / OC1B (CCP1 ->PWM 1)
 *                  VDD      --|             |-- Pin 16
 *    Phase 1(Azul) / Pin 14 --| ___________ |-- Pin 15 / Phase 2(Amarelo)
 */

#include "xc.h"
#include "config.h"
#include <stdio.h>

typedef struct Encoder{ 
    int32_t Theta;    // Encoder position
    int16_t dTheta;   // Displacement since last update
    uint16_t cnt;     // Counter last value
}Encoder; 
typedef Encoder *Enc_handle;

#define TControl 10
#define MIN_I_ERROR 100
#define MIN_D_ERROR 200

/*
 * PWM parameters
 */
#define MAXPWM 1000
#define MINPWM -MAXPWM
#define PWMSTEP 10

#define ADCToDeltaStepRatio 4.9 // max_dTheta = 840, max_ADC = 4095
#define ADCToStepRatio 2.8 // max_Theta = 11500, max_ADC = 4095

inline void ConfigCLK(void);
inline void ConfigIO(void);

inline void ConfigADC(void);
inline void UART1Init(unsigned long int baud);
inline void ConfigTMR1(void);
inline void ConfigCN(void);
inline void ConfigCCP1(void);
inline void ConfigCCP2(void);
inline void ConfigCLC1(void);
inline void InitEncoder(void);
void UpdateEncoder(void);
unsigned int readADC(unsigned int ch);

uint32_t time = 0;
uint32_t lastUpdate = 0;

// Controlador
int16_t SpeedError = 0;

float Kp = 2;
float Ki = 1;
float Kd = 1;
int32_t P = 0, I = 0, D = 0, PID = 0;

Encoder encX;

uint8_t control_flag = 0;

// -420:420
#define NumValues 7
int16_t SpeedValues[NumValues] = {0, 30, 50, -20, -50, -20, 0};
int16_t TimeValues[NumValues] = {1, 2, 5, 7, 10, 20, 22};
uint8_t nextStep = 0;
int16_t SpeedRef = 0;

int main(void) {

    ConfigCLK();
    ConfigIO();
    UART1Init(19200);
    ConfigTMR1(); // Config TTick
    ConfigCCP1(); // Config PWM
    ConfigCCP2(); // Confic Counter
    //ConfigCN(); // Change Notification for direction
    ConfigCLC1();
    InitEncoder(); // Config contagem de impulsos
    ConfigADC();

    
    printf("\rControlador PID de velociade\r\n");

    while (1) {
        
        if((TimeValues[nextStep]*1000) == time){
            SpeedRef = SpeedValues[nextStep];
            nextStep++;
            if(nextStep > NumValues-1){
                nextStep = 0;
            }
        }
        
        if (control_flag) {
            int16_t dError;

            dError = SpeedError;
            //SpeedRef = (readADC(10)/ADCToDeltaStepRatio) - (2046/ADCToDeltaStepRatio);
            SpeedError = SpeedRef - encX.dTheta;
            printf("%d,%d\n",SpeedError,SpeedRef);
            dError = (SpeedError - dError);
            
            P = (int16_t)Kp * SpeedError;
            
            if((SpeedError >= -MIN_I_ERROR) && (SpeedError <= MIN_I_ERROR)){
                //totalError += error;// * (0.01);
                I += (int16_t)Ki * SpeedError;
                if(I > MAXPWM)
                    I = MAXPWM;
                else if(I < -MAXPWM)
                    I = -MAXPWM;
            }
            if((dError >= -MIN_D_ERROR) && (dError <= MIN_D_ERROR))
                D = Kd*dError;
            else
                D = 0;
            
            PID = P + I + D;

            if (PID < MINPWM) {
                PID = MINPWM;
            }
            if (PID > MAXPWM) {
                PID = MAXPWM;
            }
            
            if (PID < 0) {
                PID = -PID; // Direcção
                PORTAbits.RA1 = 1; // Direcção
            } else {
                PORTAbits.RA1 = 0; // Direcção
            }
            
            //P = P;///(0xFFFFFFFF/0xFFFF); //int32_t -> int16_t
            CCP1RB = (PID & 0x0000FFFF);

            control_flag = 0;
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
    TRISAbits.TRISA1 = 0; // Dir      (RA1)

    ANSB = 0;
    TRISBbits.TRISB8 = 0; // PWM      (OC1B)
    ANSBbits.ANSB14 = 1;
    TRISBbits.TRISB14 = 1; // Potenciometro
    TRISBbits.TRISB15 = 1; // Encoder  (TCKIA)

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
    T1CON = 0x8010; // enabled, prescaler 1:8, 0.5 us tck @ Fcy=16MHz
    TMR1 = 0;
    PR1 = 2000 - 1; // 1ms TTick

    _T1IF = 0;
    _T1IE = 1; // Activa interrupcoes
}

void _ISRFAST _T1Interrupt(void) {
    time++;

    if (time - lastUpdate >= TControl) {

        UpdateEncoder();
        lastUpdate = time;
        
        control_flag = 1;

    }

    _T1IF = 0;
}

inline void ConfigCN(void) {
    CNEN2bits.CN29IE = 1;
    _CNIF = 0; // Clear the interrupt flag
    _CNIP = 3; // Choose a priority from 1 (low) to 7 (high)
    _CNIE = 1; // enable the Change Notification general interrupt
}

void _ISR _CNInterrupt(void) {
    // Actualiza encoder quando o motor muda de sentido
    UpdateEncoder();
    _CNIF = 0; // clear the change notification interrupt bit
}

inline void ConfigCCP1(void) {
    /*
     * Configura CCP1 em modo PWM
     * Fpwm = Fcy/MAXPWM = 16MHz/4000 = 4kHz
     * Duty = CCP1RB / MAXPWM
     * Saida PWM: OC1B (pin17)
     * Consultar: Section 64 - Capture Compare PWM Timer (MCCP and SCCP)
     */
    // Set MCCP operating mode
    CCP1CON1L = 0;
    // CCP1CON1Lbits.CCSEL = 0; // Set MCCP operating mode (OC mode)
    CCP1CON1Lbits.MOD = 0b0101; // Set mode (Buffered Dual-Compare/PWM mode)
    //Configure MCCP Timebase
    // CCP1CON1Lbits.TMR32 = 0;   // Set timebase width (16-bit)
    // CCP1CON1Lbits.TMRSYNC = 0; // Set timebase synchronization (Synchronized)
    // CCP1CON1Lbits.CLKSEL = 0b000; // Set the clock source (Tcy)
    // CCP1CON1Lbits.TMRPS = 0b00;   // Set the clock pre-scaler (1:1)
    CCP1CON1H = 0;
    // CCP1CON1Hbits.TRIGEN = 0; // Set Sync/Triggered mode (Synchronous)
    // CCP1CON1Hbits.SYNC = 0b00000; // Select Sync/Trigger source (Self-sync)

    //Configure MCCP output for PWM signal
    CCP1CON2Hbits.OCBEN = 1; // Enable output signal (OC1B/RB8/pin17)
    CCP1CON3H = 0;
    // CCP1CON3Hbits.OUTM = 0b000; // Set advanced output modes (Standard output)
    // CCP1CON3Hbits.POLACE = 0;   //Configure output polarity (Active High)
    CCP1TMRL = 0; //Initialize timer prior to enable module.
    CCP1PRL = MAXPWM; //Configure timebase period
    CCP1RA = 0; // Set the rising edge compare value
    CCP1RB = 0; // Set the falling edge compare value
    CCP1CON1Lbits.CCPON = 1; // Turn on MCCP module
}

inline void ConfigCCP2(void) {
    /*
     * Configura CCP2 para contar impulsos do encoder
     * Usa TCKIA/RB15 (pin26)
     */
    CCP2CON1L = 0; // Reset CCP module: 16 bit timer/counter; div 1
    CCP2CON1Lbits.CLKSEL = 0b111; // TCLKIA input
    CCP2PRL = 0xFFFF; // Max
    CCP2TMRL = 0;

    CCP2CON1Lbits.CCPON = 1; // Enable CCP

    _CCT2IF = 0;
    _CCT2IE = 0; // Desactiva interrupcao do CCP Timer
}

inline void ConfigCLC1(void){
    
    CLC1CONLbits.MODE = 0b100;  // 1 input D-FF
    CLC1SELbits.DS1 = 0;        // CLCINA
    CLC1SELbits.DS2 = 0;        // CLCINB
    
    CLC1GLSLbits.G1D1T = 1;     // Data Set 1 -> Gate 1 -> Clock
    CLC1GLSLbits.G2D2T = 1;     // Data Set 2 -> Gate 2 -> Data
    //CLC1GLSHbits.G3D3T = 0;
    //CLC1GLSHbits.G4D4T = 0;
    
    CLC1CONLbits.LCEN = 1;      // CLC1 On
    
    CLC1CONLbits.INTN = 1;      // Interrupt on rising edge
    CLC1CONLbits.INTP = 1;      // Interrupt on falling edge
    
    _CLC1IF = 0;
    _CLC1IE = 1;
    
}

void _ISR _CLC1Interrupt(void){
    // Actualiza encoder quando o motor muda de sentido
    UpdateEncoder();
    _CLC1IF = 0; // clear the change notification interrupt bit
}

inline void InitEncoder(void) {
    // Inicializa a estrutura de suporte ao encoder
    // e os perifericos necessarios
    // Entradas: RA
    encX.Theta = 0; // Posicao inicial do encoder
    //encX.Counter = &CCP2TMRL;
    encX.cnt = CCP2TMRL; // Valor inicial do contador

}

void UpdateEncoder(void) {
    // Conta impulsos com o CCP2    
    // 
    uint16_t cnt;

    cnt = CCP2TMRL; // Valor actual do contador
    if (cnt < encX.cnt) {
        encX.dTheta = (0xffff - encX.cnt) + cnt + 1; // Overflow
    } else {
        encX.dTheta = cnt - encX.cnt;
    }
    if (CLC1CONLbits.LCOUT == 0) { // Reverse direction
        encX.dTheta = -encX.dTheta;
    }
    encX.cnt = cnt; // Guarda valor do contador
    encX.Theta += encX.dTheta; // Actualiza posição
}
