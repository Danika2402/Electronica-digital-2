/*
 * File:   Slave_cont.c
 * Author: Danika
 *
 * Created on 9 de agosto de 2021, 12:40 PM
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
#include "I2C.h"

#define _XTAL_FREQ 8000000 

uint8_t z;

char boton1, boton2;

void setup(void);

void __interrupt()isr(void){
    
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
            PORTA = SSPBUF;        }     // Guardar en el PORTA el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;         
            BF = 0;
            SSPBUF = PORTD;     
            SSPCONbits.CKP = 1; 
            __delay_us(250);
            while(SSPSTATbits.BF); 
        }
       
        PIR1bits.SSPIF = 0;    
    }
    
}



void main(void) {
    setup();
    
    while (1){
        if(RB0 == 0 && boton1 != RB0){
            PORTD++;
        }
        if(RB1 == 0 && boton2 != RB1){
            PORTD--;
        }
        boton1 = RB0;
        boton2 = RB1;
        PORTD &= 0x0F;
    }
    return;
}
void setup(void){
    ANSEL = 0x00;
    ANSELH = 0x00;     
    
    TRISA = 0x00;
    TRISD = 0x00;       
    TRISB = 0x03; 
    
    PORTD = 0x00;
    PORTB = 0x00;    
    PORTA = 0x00;
    
    OSCCONbits.IRCF = 0b111;    //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;         
    
    //Config. PULL-UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;                                          
        
    boton1 = 0;
    boton2 = 0;
    I2C_Slave_Init(0x40);   
    
}