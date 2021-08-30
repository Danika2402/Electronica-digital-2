/*
 * File:   LCD.c
 * Author: Danika
 *
 * Created on 24 de agosto de 2021, 12:05 AM
 */
#include <xc.h>
#include <stdint.h>
#include "LCD.h"
#define _XTAL_FREQ 8000000


//Para 8 bits
void LCD_Init(void){
    LCD_PORT = LCD_PORT & 0x00; //limpiar puerto
    LCD_TRIS = 0x00;            //salidas
    RS_TRIS = 0;        //pines como salida
    RW_TRIS = 0;
    E_TRIS = 0;
    RS_PIN = 0;         //estado inicial
    RW_PIN = 0;
    E_PIN = 0;
        
    __delay_ms(16);     
    LCD_comando(0b00110000);    
    __delay_ms(5);               
    LCD_comando(0b00110000);
    __delay_us(100);
    LCD_comando(0b00110000);
    __delay_us(100);            
    
    //configuracion 8 bits
    LCD_comando(0b00111000);
    __delay_us(100);
    LCD_comando(0b00001000);
    __delay_us(100);
    LCD_comando(0b00000001);
    __delay_us(100);
    LCD_comando(0b00000100);     //incremente y no moverse
    __delay_us(100);
    __delay_us(100);
    LCD_comando(0x06);           //display
    LCD_comando(0x0C);           //incremento y no moverse
    return;
}

void LCD_comando(unsigned char dato){
    LCD_PORT = dato;
    RS_PIN = 0;                 //modo comando
    RW_PIN = 0;                 //modo escritura
    __delay_us(10);
    E_PIN = 1;                  //habilito LCD
    __delay_us(10);
    E_PIN = 0;                  //deshabilito LCD
    
    return;
}

void LCD_XY(unsigned char x,unsigned char y){
    if(x>0){
        LCD_comando(0xC0 + y);
    }
    else{
        LCD_comando(0x80 + y);
    }
}

void LCD_Cadena(const char *dato){
    while(*dato){
        __delay_us(100);
        LCD_Data(*dato);
        dato++;
    }
}

void LCD_Data(char data){
    __delay_us(100);
    
    LCD_PORT = data;
    //Configuración
    RW_PIN = 0;     //Modo Escritura
    RS_PIN = 1;     //Modo Caracter
    __delay_us(10); 
    E_PIN = 1;      //Habilita LCD
    __delay_us(10);
    E_PIN = 0;      //Deshabilita LCD
    
    return;
}


