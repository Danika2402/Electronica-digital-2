/*
 * File:   USART.c
 * Author: Danika
 *
 * Created on 11 de agosto de 2021, 11:04 PM
 */
#include <xc.h> 
#include "USART.h"
#include <pic16f887.h>

void USART_Int(void){
                               
    TXSTAbits.SYNC = 0;     //asincrono
    TXSTAbits.BRGH = 1;     //alta velocidad
    BAUDCTLbits.BRG16 = 1;  //uso 16 bits
   
    SPBRG = 207;   //baud rate                      
    SPBRGH = 0;             
    
    RCSTAbits.SPEN = 1;     //enciendo el modulo
    RCSTAbits.RX9 = 0;      
    RCSTAbits.CREN = 1;     //activo recepción
    TXSTAbits.TXEN = 1;     //activo transmision 
}


void USART_Tx(char data){       //caracter a la terminal
    while(TXSTAbits.TRMT == 0);
    TXREG = data;
}

char USART_Rx(){                //comunicacion serial
    return RCREG; 
   }

void USART_string(char *cadena){   //envio string
    while(*cadena != '\0'){
        USART_Tx(*cadena);
        cadena++;
    }
}
