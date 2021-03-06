/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_16_2_H
#define	LCD_16_2_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//para 8 pines
void LCD_Init(void);       
void LCD_comando(unsigned char dato); 
void LCD_XY(unsigned char x,unsigned char y);  
void LCD_Cadena(const char *dato); 
void LCD_Data(char data);  

#define LCD_PORT PORTD
#define LCD_TRIS TRISD

#define RS_PIN PORTCbits.RC5
#define RS_TRIS TRISCbits.TRISC5

#define RW_PIN PORTCbits.RC6
#define RW_TRIS TRISCbits.TRISC6

#define E_PIN  PORTCbits.RC7
#define E_TRIS TRISCbits.TRISC7

#define _XTAL_FREQ 8000000

#endif	/* LCD_16_2_H */