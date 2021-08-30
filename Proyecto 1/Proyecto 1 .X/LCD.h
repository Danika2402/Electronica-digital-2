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

#define RS_PIN PORTEbits.RE0
#define RS_TRIS TRISEbits.TRISE0

#define RW_PIN PORTEbits.RE1
#define RW_TRIS TRISEbits.TRISE1

#define E_PIN  PORTEbits.RE2
#define E_TRIS TRISEbits.TRISE2

#define _XTAL_FREQ 8000000

#endif	/* LCD_16_2_H */