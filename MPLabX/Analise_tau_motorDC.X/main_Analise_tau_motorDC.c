/*
 * File:   main_Analise_tau_motorDC.c
 * Author: Eduardo Rodrigues
 *
 * Created on April 28, 2018, 4:13 PM
 */

/*
 *                             pic24fv16km202
 *                               ___________
 *                     Pin 1 --| 0           |--      AVDD
 *                     Pin 2 --|             |--      AVSS
 *               RA3 / Pin 3 <-|             |<- Pin 26 / TCKIA (CCP2 <- encoder 1)
 *                     Pin 4 --|             |<- Pin 25 / AN10 (RB14 -> Corrente)
 *                     Pin 5 --|             |-- Pin 24
 *                     Pin 6 --|             |-- Pin 23
 *                     Pin 7 --|             |-- Pin 22
 *                  VSS      --|             |-- Pin 21
 *                     Pin 9 --|             |-- Pin 20
 *                    Pin 10 --|             |-- Pin 19
 *                    Pin 11 --|             |-- Pin 18
 *                    Pin 12 --|             |-- Pin 17 
 *                  VDD      --|             |-- Pin 16
 *                    Pin 14 --| ___________ |-- Pin 15 
 */

#include "xc.h"
#include "config.h"

typedef struct Encoder{ 
    int32_t Theta;    // Encoder position
    int16_t dTheta;   // Displacement since last update
    uint16_t cnt;     // Counter last value
    
    //uint16_t *Counter;
}Encoder; 
typedef Encoder *Enc_handle;

#define NSamples 800
#define TToPrint NSamples+1
#define TToStart 5

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);

inline void ConfigADC(void);
unsigned int readADC(unsigned int ch);

inline void UART1Init(unsigned long int baud);
uint8_t putChar1(uint8_t c);
uint8_t getChar1(void);

uint32_t time = 0;
uint8_t print_flag = 0;

uint16_t values[NSamples];

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigADC();
    UART1Init(19200);
    
    while(1){
        if(print_flag){
            _T1IE = 0;  // stop interrups during print
            
            uint16_t i;
            for(i = 0; i < NSamples; i++){
                putChar1((values[i] & 0xF0) >> 8);
                putChar1(values[i] & 0x0F);
            }
            
            print_flag = 0;
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
    TRISAbits.TRISA3 = 0;
    PORTA = 0;
    
    ANSB = 0;
    ANSBbits.ANSB14 = 1;
    TRISBbits.TRISB14 = 1; // Potenciometro
}

inline void ConfigTMR1(void) {
    T1CON = 0x8000; // enabled, prescaler 1:1, 62.5 ns tck @ Fcy=16MHz
    TMR1 = 0;
    PR1 = 240 - 1; // 15us TTick

    _T1IF = 0;
    _T1IE = 1; // Activa interrupcoes
}

void _ISRFAST _T1Interrupt(void) {
    time++;
    
    if(time == TToStart)
        LATAbits.LATA3 = 1;
    
    if(time == TToPrint){
        LATAbits.LATA3 = 0;
        print_flag = 1;
    }
    
    values[time] = readADC(10);
    
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

uint8_t putChar1(uint8_t c){
    while ( U1STAbits.UTXBF);   // wait while Tx buffer full
    U1TXREG = c;
    return c;
}

uint8_t getChar1(void){
    while ( !U1STAbits.URXDA);  // wait
    return U1RXREG;             // read from the receive buffer
}
