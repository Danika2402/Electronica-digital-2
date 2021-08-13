/*
 * File:   Master.c
 * Author: Danika
 *
 * Created on 5 de agosto de 2021, 10:50 PM
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
#include "LCD.h"
#include <xc.h>
#define _XTAL_FREQ 8000000

int ADC, TEMP, CONT;
char centena, decena, unidad;
char centena1, decena1, unidad1;
char centena2, decena2, unidad2;
void setup (void);

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
        ADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x41);
        CONT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3); //temperatura
        I2C_Master_Stop();
        __delay_ms(200);
        I2C_Master_Start();
        I2C_Master_Write(0x81);//nos manda 2 bytes
        TEMP = ((I2C_Master_Read(0))<<8);
        TEMP += I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        TEMP &= ~0x03;
        TEMP = ((175.72 * TEMP)/65536) - 46.85; //datasheet
        
        ADC = ADC * 1.97; //500/255 = 1.9607
        CONT = CONT * 33.33;//500/15
        
        centena = (ADC /100) + 48;
        decena = ((ADC - (centena*100)) / 10) + 48;
        unidad = ((ADC - (centena*100)) - (decena*10)) + 48;
        
        centena1 = (CONT /100) + 48;
        decena1 = ((CONT - (centena1*100)) / 10) + 48;
        unidad1 = ((CONT - (centena1*100)) - (decena1*10)) +48;
        
        centena2 = (TEMP /100) + 48;
        decena2 = ((TEMP - (centena2*100)) / 10) + 48;
        unidad2 = ((TEMP - (centena2*100)) - (decena2*10)) + 48;
        
        
        LCD_XY(0,0);
        LCD_Cadena("S1    S2     S3");
        
        LCD_XY(2,0);
        LCD_Data(centena);
        LCD_Data('.');
        LCD_Data(decena);
        LCD_Data(unidad);
        
        LCD_XY(2,5);
        LCD_Data(centena1);
        LCD_Data('.');
        LCD_Data(decena1);
        LCD_Data(unidad1);
        
        LCD_XY(2,11);
        LCD_Data(centena2);
        LCD_Data(decena2);
        LCD_Data(unidad2);
        
        if(TEMP < 0){
            TEMP *= -1;
            LCD_XY(2,10);
            LCD_Data('-');            
        }else{
            LCD_XY(2,10);
            LCD_Data(' ');            
        } 
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
            //oscilador a 8M Hz
    OSCCONbits.IRCF2 =1;     
    OSCCONbits.IRCF1 =1; 
    OSCCONbits.IRCF0 =1; 
    OSCCONbits.SCS   =1;
    
    TEMP = 0;
    CONT = 0;
    ADC = 0;
    centena = 0;
    decena = 0;
    unidad = 0;
    
    centena1 = 0;
    decena1 = 0;
    unidad1 = 0;
    
    centena2 = 0;
    decena2 = 0;
    unidad2 = 0;
    
    LCD_Init();
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}