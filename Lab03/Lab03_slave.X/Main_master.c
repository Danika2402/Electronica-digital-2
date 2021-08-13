/*
 * File:   Main_master.c
 * Author: Danika
 *
 * Created on 30 de julio de 2021, 04:56 PM
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
#include "USART.h"

#define _XTAL_FREQ 4000000

void setup(void);
void spi(void);
void voltajes(char c, char d, char u);
char centena(int c);
char decena(int d);
char unidad(int u);
void terminal(void);
void usart(void);

char pot1,pot2;
int v1,v2,v3;
char contador;
char c1,d1, u1; //voltaje en pot1
char c2,d2,u2;  //voltaje en pot2
char c3,d3,u3;  //voltaje en puerto A
char dato_viejo1, dato_viejo2;

char dato;
char entrada[3];
char turno;
int voltaje;

void main(void) {
    
    setup();
    while(1){
        dato_viejo1 = pot1;
        dato_viejo2 = pot2;
        
        spi();
        
        v1 = pot1 * 1.96;  //(500/255) = 1.96
        v2 = pot2 * 1.96;
        v3 = PORTA * 1.96;
        
        c1 = centena(v1);
        c2 = centena(v2);   
        c3 = centena(v3);
        
        d1 = decena(v1);
        d2 = decena(v2);
        d3 = decena(v3);
        
        u1 = unidad(v1);
        u2 = unidad(v2);
        u3 = unidad(v3);
        
        terminal();
        
        contador = PORTA;
        dato = USART_Rx();
        usart();
        
        if(dato == '+'){
            PORTA++;
        }else if(dato == '-'){
            PORTA--;
        }
        
    } 
    return;
}
void setup (void){
    ANSEL = 0x00; 
    ANSELH = 0x00;

    TRISA = 0x00;
    TRISC = 0b10010000;  //entradas solo M_SDI y RX
    TRISCbits.TRISC2 = 1;
    
    PORTC = 0000000100;
    PORTA = 0x00;
    
     //oscilador a 4M Hz
    OSCCONbits.IRCF2 =1;    
    OSCCONbits.IRCF1 =1;
    OSCCONbits.IRCF0 =0;
    OSCCONbits.SCS   =1;
    
    USART_Int();        
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}
void spi(void){
    
        RC2 = 0;    //slave select
        __delay_ms(1);
        spiWrite(0);
        pot1 = spiRead();
        __delay_ms(1);
        RC2 = 1;    //slave deselect
        __delay_ms(1);
        
        RC2 = 0;    //slave select
        __delay_ms(1);
        spiWrite(1);
        pot2 = spiRead(); 
        __delay_ms(1);
        RC2 = 1;    //slave deselect
        __delay_ms(1);
}

void voltajes(char c, char d, char u){
    
    c += 48;
    d += 48;
    u += 48;
    
    USART_Tx(c);
    USART_Tx('.');
    USART_Tx(d);
    USART_Tx(u);
    USART_Tx('V');
   
    return;
}

void terminal(void){
    
    if(dato_viejo1 != pot1 || dato_viejo2 !=pot2 || contador !=PORTA) {
        USART_string("\n\r\n\r + para incrementar PORTA");
        USART_string("\n\r\n\r - para decrementar PORTA");
        
        USART_string("\n\r\n\r Votlaje del POT V1: ");
        voltajes(c1,d1,u1);
        
        USART_string("\n\r\n\r Votlaje del POT V2: ");
        voltajes(c2,d2,u2);
        
        USART_string("\n\r\n\r Votlaje del PORTA: ");
        voltajes(c3,d3,u3);
        
        USART_string("\n\r\n\r");
    }
}

void usart(void){
    if(RCIF ==1){
        if(dato > 47 && dato < 58){
            entrada[turno] = dato;
            turno++;
            
            if(turno >2){
                
                turno = 0;
                voltaje = (entrada[0] - 48) * 100;
                voltaje += (entrada[1] - 48) * 10;
                voltaje += (entrada[2] - 48);
                PORTA = voltaje/1.958;
                
            }
        }
    }
}

char centena(int c){
    char dato;
    dato = dato/100;
    return dato;
}
char decena(int d){
    char dato;
    dato = (dato %100)/10;
    return dato;
}
char unidad(int u){
    char dato;
    dato = (dato %100)%10;
    return dato;
}