/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <pic16f887.h>

void USART_Int(void);
void USART_Tx(char data);
char USART_Rx(void);
void USART_string(char *c);

#endif	/* USART_H */

