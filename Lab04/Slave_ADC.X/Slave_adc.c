/*
 * File:   Slave_adc.c
 * Author: Danika
 *
 * Created on 6 de agosto de 2021, 09:23 AM
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

#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "ADC.h"
#include <xc.h>

#define _XTAL_FREQ 8000000
uint8_t z;
uint8_t dato;
char recepcion; 

void setup(void);

void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0; 
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF){     // Esperar a que la recepción se complete
            PORTD = SSPBUF;        }     // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = recepcion;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
    setup();

    while(1){
        
        if (ADCON0bits.GO == 0){
            recepcion = ADRESH;
            __delay_us(50);
            
            ADCON0bits.GO = 1;
        }
       /* 
       __delay_ms(500); 
       
        PORTB = ~PORTB;
       __delay_ms(500);*/
    }
    return;
}
void setup(void){
    ANSEL = 0b00100000;
    ANSELH = 0;
    
    TRISE = 0xff;
    TRISB = 0x00;
    TRISD = 0x00;
    
    PORTB = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
  
        //oscilador a 8M Hz
    OSCCONbits.IRCF2 =1;    
    OSCCONbits.IRCF1 =1;
    OSCCONbits.IRCF0 =1;
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
 
    I2C_Slave_Init(0x50);  
    
}