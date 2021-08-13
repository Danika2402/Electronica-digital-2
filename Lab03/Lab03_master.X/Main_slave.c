/*
 * File:   Main_slave.c
 * Author: Danika
 *
 * Created on 30 de julio de 2021, 04:41 PM
 */

#pragma config  FOSC    = INTRC_NOCLKOUT
#pragma config  WDTE    = OFF
#pragma config  PWRTE   = OFF
#pragma config  MCLRE   = OFF
#pragma config  CP      = OFF
#pragma config  CPD     = OFF
#pragma config  BOREN   = OFF
#pragma config  IESO    = OFF
#pragma config  FCMEN   = OFF
#pragma config  LVP     = OFF

#pragma config  BOR4V   = BOR40V
#pragma config  WRT     = OFF

#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "ADC.h"

#define _XTAL_FREQ 4000000

void setup(void);
char pot1,pot2;
char potenciometro;


void __interrupt() isr(void){
    
    if(SSPIF == 1){
        potenciometro = spiRead();
        if(potenciometro == 1){
            spiWrite(pot2);
        }else {
            spiWrite(pot1);
        }
    }
    SSPIF = 0;
}

void main(void) {
    
    setup();
    while(1){
        
        if(ADCON0bits.GO == 0){
            
            if(ADCON0bits.CHS == 5){
                pot1 = ADRESH;
                ADCON0bits.CHS = 6;
                
            }else if(ADCON0bits.CHS == 6){
                pot2 = ADRESH;
                ADCON0bits.CHS = 5;
            }
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
        
    }
    return;
}

void setup (void){
    ANSEL = 0b01100000;
    ANSELH = 0x00;
    
    TRISE = 0xff;
    TRISAbits.TRISA5 = 1;
    
    PORTE = 0x00;
    PORTA = 0x00;
    
     //oscilador a 4M Hz
    OSCCONbits.IRCF2 =1;    
    OSCCONbits.IRCF1 =1;
    OSCCONbits.IRCF0 =0;
    OSCCONbits.SCS   =1;
    
    //Config. ADC
    canal(5);
    
    ADCON1bits.VCFG0 = 0;   //voltajes de referencia
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS0 = 0;   //fosc/8, Tad = 2us
    ADCON0bits.ADCS1 = 1;

//    INTCONbits.PEIE  =1;
    
//    PIR1bits.ADIF   = 0;    //bajando bandera
//    PIE1bits.ADIE   = 1;    //habilitando bandera
    
    ADCON1bits.ADFM =0; //izquierda
    
    ADCON0bits.ADON = 1;    //enciende modulo y permite conversion 
    __delay_us(50);
    ADCON0bits.GO_nDONE = 1; 
    
    pot1 = 0;
    pot2 = 0;
    
    //Interrupciones 
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
    