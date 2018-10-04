/*
 * File:   main_Analise_tau_Mec_motorDC.c
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

#define NSamples 800-1
#define TToPrint NSamples+1
#define TToStart 5

inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void ConfigTMR1(void);
inline void UART1Init(unsigned long int baud);
uint8_t putChar1(uint8_t c);
uint8_t getChar1(void);
inline void ConfigCCP2(void);
inline void InitEncoder(void);
void UpdateEncoder(void);

Encoder encX;
uint32_t time = 0;
uint32_t lastUpdate = 0;
uint8_t print_flag = 0;

uint16_t values[NSamples];

int main(void) {
    
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigCCP2();
    InitEncoder();
    UART1Init(19200);
    
    while(1){
        if(print_flag){
            _T1IE = 0;  // stop interrups during print
            
            uint16_t i;
            for(i = 0; i <= NSamples; i++){
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
    TRISBbits.TRISB15 = 1; // Encoder  (TCKIA)
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
    
    values[time] = encX.dTheta;
    
    _T1IF = 0;
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
    
    encX.cnt = cnt; // Guarda valor do contador
    encX.Theta += encX.dTheta; // Actualiza posição
}
