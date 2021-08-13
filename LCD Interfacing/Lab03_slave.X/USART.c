/*
 * File:   USART.c
 * Author: Danika
 *
 * Created on 29 de julio de 2021, 10:22 PM
 */

#include <xc.h>   
#include <stdint.h>
#include <pic16f887.h>
#include "USART.h"

void USART_Int(void){
                                
    TXSTAbits.SYNC = 0;     //asincrono
    TXSTAbits.BRGH = 1;     //alta velocidad
    BAUDCTLbits.BRG16 = 1;  //uso los 16 bits
   
    SPBRG = 207;   //tabla baud rates                       
    SPBRGH = 0;            
    
    RCSTAbits.SPEN = 1;     //se enciende el modulo
    RCSTAbits.RX9 = 0;      //no trabajare a 9 bits
    RCSTAbits.CREN = 1;     //se activa la recepcion
    TXSTAbits.TXEN = 1;     //se activa la transmicion
}


void USART_Tx(char data){       //envio de caracteres
    while(TXSTAbits.TRMT == 0); //a tx y a la computadora
    TXREG = data;
}

char USART_Rx(){                //comunicacion serial
    return RCREG; 
   }

void USART_String(char *cadena){   //transformo un dato     
    while(*cadena != '\0'){        //y lo envio como caracter
        USART_Tx(*cadena);
        cadena++;
    }
}