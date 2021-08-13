/*
 * File:   juego de carreras.c
 * Author: Danika
 *
 * Created on 11 de julio de 2021, 12:46 PM
 */
//******************************************************************************
/*Uso de la terminal con el pic
 *Se usa la terminal para mandar caracteres al pic
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

#include <stdint.h>
#include <xc.h>
#define _XTAL_FREQ  4000000

const char tabla[] = {0xfc, 0x60, 0xda, 0xf2, 0x66,
                      0xb6, 0xbe, 0xe0, 0xfe, 0xf6};
char s,c, i, a, j1, j2;
char cont1, cont2;

void setup(void);
char indicador (char analizar);

void __interrupt() isr (void){
    
    if(RBIF == 1){
        if(RB0 == 0){
            i++;
            
        }else if (RB1 ==0 && a == 1){
            cont1++;
            if(cont1 == 10){
                j1++;
                cont1 = 0;
            }
        }else if (RB2 == 0 && a == 1){
            cont2++;
            if(cont2 == 10){
                j2++;
                cont2 = 0;
            }
        }
        RBIF = 0;
    }
    
    else if(T0IF == 1){
        
        c++;
        if(c == 4){
            s++;
            if(s == 255){
                s = 0;
            }
        }
        
        T0IF = 0;
        TMR0 = 60; //25 ms
    }
}

void main(void) {
    setup();
    
    while(1){
        if(i == 1){
            /*
             if(s==1){
             encender una led}
             if (segundos){
             *  encender otra led}
             */
            RB7 = 1;  //rojo
            RB6 = 0;  
            RB5 = 0;
            PORTA = tabla[3];
            __delay_ms(600);
            PORTA = tabla[2];
            __delay_ms(1000);
            
            RB7 = 0;  //amarillo
            RB6 = 1;  
            RB5 = 0;
            PORTA = tabla[1];
            __delay_ms(600);
            
            RB7 = 0;  //verde
            RB6 = 0;  
            RB5 = 1;
            PORTA = tabla[0];
            __delay_ms(1000);
            
            RB7 = 0;  //apagado
            RB6 = 0;  
            RB5 = 0;
            PORTA = 0x00;
            
            i = 0;
            a = 1;
        }
        if (a == 1){
            
            PORTC = indicador(j1);
            PORTD = indicador(j2);
            
        }
            __delay_ms(50);
            if (j1 == 8){
                PORTA = tabla[1];
                RE0 = 1;
                RE1 = 0;
                i = 0;
                a = 0;
                __delay_ms(2000);
                PORTA = 0x00;
                PORTE = 0x00;
            }else if (j2 == 8){
                PORTA = tabla[2];
                RE0 = 0;
                RE1 = 1;
                i = 0;
                a = 0;
                __delay_ms(2000);
                PORTA = 0x00;
                PORTE = 0x00;
            }
    }
}

void setup(void){
    ANSEL = 0x00;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0b00000111;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Config. TMR0
    OSCCONbits.IRCF = 0b111; //8 M Hz
    OSCCONbits.SCS = 1;
    
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b111;  // 1:256
    TMR0 = 60;                  //Reinicio 25ms
    
    //Config. PULL UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    WPUBbits.WPUB2 = 1;
    
    IOCBbits.IOCB0 = 0;
    IOCBbits.IOCB1 = 0;
    IOCBbits.IOCB2 = 0;
    RBIF = 0;
    
    
    //Config. interrupciones
    INTCONbits.RBIE = 1;    //interrupcion pull up 
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;    //interrupcion tmr0
    INTCONbits.T0IF = 0;
    
    s = 0;
    c = 0;
    i = 0;
    a = 0;
    j1 = 0;
    j2 = 0;
    cont1 = 0;
    cont2 = 0;
    
    return;
}

char indicador (char analizar){
    char salida;
    
    switch(analizar){
        case(0):
            salida = 0b00000000;
            break;
        case(1):
            salida = 0b00000001;
            break;
        case(2):
            salida = 0b00000011;
            break;
        case(3):
            salida = 0b00000111;
            break;
        case(4):
            salida = 0b00001111;
            break;
        case(5):
            salida = 0b00011111;
            break;
        case(6):
            salida = 0b00111111;
            break;
        case(7):
            salida = 0b01111111;
            break;
        case(8):
            salida = 0xff;
            break;
        
    }
    return salida;
}