/*
 * File:   MAIN.c
 * Author: Danika
 *
 * Created on 24 de agosto de 2021, 12:01 AM
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
#include "LCD.h"
#include "I2C.h"
#include "ADC.h"
#define _XTAL_FREQ 8000000

void setup(void);

void main(void) {
    
    setup();
    while(1){
        
        I2C_Master_Start();
        /*
        I2C_Master_Write(0x50);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();*/
        
        I2C_Master_Write(0x51);
        I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
    }
    
}

void setup(void){
    ANSEL =  0x00;
    ANSELH = 0b00010000;    //canal 12
    
    TRISE = 0x01;
    TRISB = 0x01;   //RB0 como entrada
    TRISC = 0b10000000; //RX entrada
    TRISD = 0;
    
    PORTE = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    ADC_Int();
    canal(12);
    
    OSCCONbits.IRCF = 0b111;    //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;
    
    USART_Int();
    LCD_Init();
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}