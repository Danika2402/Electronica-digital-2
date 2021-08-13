/*
 * File:   interrupciones y librerias.c
 * Author: Danika
 *
 * Created on 15 de julio de 2021, 03:00 PM
 */
//******************************************************************************
/*contador con botones, ADC con multiplexor
 * Usando librerias
 */
//******************************************************************************
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

#include "ADC.h"
#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ  4000000
char display;
char unidad, decena, adc, referencia;

void setup(void);

const char tabla[]={
        0x3f,   //0
        0x06,   //1
        0x5b,   //2
        0x4f,   //3
        0x66,   //4
        0x6d,   //5
        0x7d,   //6
        0x07,   //7
        0x7f,   //8
        0x67,   //9
        0x77,   //A
        0x7c,   //b
        0x39,   //c
        0x5e,   //d
        0x79,   //e
        0x71    //f
};

void __interrupt() isr (void){
    if (RBIF==1){
        
        if (RB0==0){
            
            PORTA++;
            
        } else if (RB1==0){
            
            PORTA--;
        }
        INTCONbits.RBIF=0;
    }
    if (T0IF == 1){
        PORTD = 0;
    
        if(display==0){
            RD0 = 1;
            PORTC = tabla[decena];
            display = 1;
            
        }else if (display == 1){
            RD1 = 1;
            PORTC = tabla[unidad];
            
            display = 0;
        }
        
        T0IF = 0;
        TMR0 = 236;
    }
    return;
}

void main(void) {
    setup();
    
     
    while(1){
        referencia = PORTA;
    
        if (ADCON0bits.GO == 0){
            adc = ADRESH;
            __delay_us(50);
            
            ADCON0bits.GO = 1;
        }
    
        unidad = display_unidad(adc);
        decena = display_decena(adc);
        
        if(adc < referencia){
            RE1 = 0;
        }else if(adc >= referencia){
            RE1 = 1;
        }
    
    }
}

void setup(void){
    ANSEL = 0b00100000;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0b00000011;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0b001;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //oscilador a 4M Hz
    OSCCONbits.IRCF2 =1;    
    OSCCONbits.IRCF1 =1;
    OSCCONbits.IRCF0 =0;
    OSCCONbits.SCS   =1;
    
    //configuracion de timer0, prescaler 1:256
    OPTION_REGbits.T0CS =0;
    OPTION_REGbits.T0SE =0;
    OPTION_REGbits.PSA =0;
    OPTION_REGbits.PS2 =1;
    OPTION_REGbits.PS1 =1;
    OPTION_REGbits.PS0 =1;
    TMR0 = 236; //5ms
    
    //Config. PULL UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    
    IOCBbits.IOCB0 = 1;         //pines 1 y 2 se realizaran la interrupcion
    IOCBbits.IOCB1 = 1;
    
    //Config. interrupciones
    INTCONbits.GIE  = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.RBIE = 1;    //interrupcion pull up 
    INTCONbits.RBIF = 0;
    
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
    
    display = 0;
    unidad = 0; 
    decena = 0;
    adc = 0; 
    referencia = 0;
    return;
}
