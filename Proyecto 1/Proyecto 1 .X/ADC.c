/*
 * File:   ADC.c
 * Author: Danika
 *
 * Created on 5 de agosto de 2021, 11:29 PM
 */
#include <stdint.h>
#include <xc.h>
#include "ADC.h"

void ADC_Int(void){
        //Config. ADC
    
    ADCON1bits.VCFG0 = 0;   //voltajes de referencia
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS0 = 0;   //fosc/8, Tad = 2us
    ADCON0bits.ADCS1 = 1;

    INTCONbits.PEIE  =1;    
    PIR1bits.ADIF   = 0;    //bajando bandera
    PIE1bits.ADIE   = 1;    //habilitando bandera
    
    ADCON1bits.ADFM =0; //izquierda
    
    ADCON0bits.ADON = 1;    //enciende modulo y permite conversion 
    __delay_us(50);
    ADCON0bits.GO_nDONE = 1; 
}

void canal (char c){

    switch(c){
        case 0:
            ADCON0bits.CHS3 = 0;    //canal 0
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 1:
            ADCON0bits.CHS3 = 0;    //canal 1
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 2:
            ADCON0bits.CHS3 = 0;    //canal 2
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 3:
            ADCON0bits.CHS3 = 0;    //canal 3
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 4:
            ADCON0bits.CHS3 = 0;    //canal 4
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 5:
            ADCON0bits.CHS3 = 0;    //canal 5
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 6:
            ADCON0bits.CHS3 = 0;    //canal 6
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 7:
            ADCON0bits.CHS3 = 0;    //canal 7
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 8:
            ADCON0bits.CHS3 = 1;    //canal 8
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 9:
            ADCON0bits.CHS3 = 1;    //canal 9
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 10:
            ADCON0bits.CHS3 = 1;    //canal 10
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 11:
            ADCON0bits.CHS3 = 1;    //canal 11
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 12:
            ADCON0bits.CHS3 = 1;    //canal 12
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 13:
            ADCON0bits.CHS3 = 1;    //canal 13
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        default:
            ADCON0bits.CHS3 = 0;    //canal 0
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
    }
    
}
/*
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
 */