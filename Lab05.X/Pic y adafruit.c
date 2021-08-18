/*
 * File:   Pic y adafruit.c
 * Author: Danika
 *
 * Created on 11 de agosto de 2021, 10:14 PM
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
#include <xc.h>
#include "USART.h"

int voltaje, voltaje2;
char entrada[2];
char cont, cont1;
char listo;
char total;
char oldCont, oldCont1;
char centena,decena,unidad;
char centena2,decena2,unidad2;

void setup (void);

void __interrupt() isr(void){
        if(RBIF == 1){
                
            if(RB0 == 0){
                cont++;
            }else if(RB1 == 0){
                cont--;
            }
        }
        RBIF = 0;
}

void main(void) {
    setup();
    while(1){

        centena = (cont / 100);
        decena = ((cont - (centena*100)) / 10);
        unidad = ((cont - (centena*100)) - (decena*10));
        
        centena += 48;
        decena += 48;
        unidad += 48;
        
        if (PIR1bits.RCIF == 1){
        
            listo = USART_Rx();
            cont1 = listo;
                if(listo == 's'){
                    USART_Tx(centena);
                    USART_Tx(decena);
                    USART_Tx(unidad);
                    listo = 0;
            
                }
            PIR1bits.RCIF = 0; 
        }
        PORTD = cont;
        PORTA = cont1;
        
        /*if(oldCont != cont ||  oldCont1!= cont1){
            
            USART_string("+ para aumentar PORTA\n\r");
            USART_string("- para decrementar PORTA\n\r");
            
            USART_string("\n\r\n\rValor del PORTD:\n");
            USART_Tx(centena);
            USART_Tx(decena);
            USART_Tx(unidad);
            USART_string("V\n\r");
            
            USART_string("\n\r\n\rValor del PORTA:\n");
            USART_Tx(centena2);
            USART_Tx(decena2);
            USART_Tx(unidad2);
            USART_string("V\n\r");            
            
        }*/
        /*
        if (PIR1bits.RCIF == 1){
            listo = USART_Rx();
            if(listo == '+'){
                cont1++;
            }else if(listo == '-'){
                cont1--;
            }
        }*/

       
    }
    
    return;
}

void setup(void){
    ANSEL = 0x00;
    ANSELH = 0x00; 
    
    TRISA = 0x00;
    TRISB = 0x03;
    TRISC = 0b10000000; //RX entrada
    TRISD = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
    PORTC = 0x00;
    
    OSCCONbits.IRCF = 0b111;    //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;  
    
        //Config. PULL-UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    
    IOCBbits.IOCB0 = 1; // pines en lo que habra interrupcion por cambio 
    IOCBbits.IOCB1 = 1; 
    
    INTCONbits.GIE = 1;     
    INTCONbits.RBIE = 1;    
    INTCONbits.RBIF = 0;    
    USART_Int();
    cont = 0;
    cont1 = 0;
}