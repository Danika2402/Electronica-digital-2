/* 
 * File:   SPI_master
 * Author: Danika Andrino
 * Comments:PIC16F887
 * Revision history: por electroSome
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __SPI_H
#define	__SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

typedef enum{
    
    MASTER_OSC_div4  = 0b00100000,
    MASTER_OSC_div16 = 0b00100001,
    MASTER_OSC_DIV64 = 0b00100010,
    MASTER_TMR2      = 0b00100011,
    SLAVE_SS_EN      = 0b00100100, 
    SLAVE_SS_DIS     = 0b00100101 
}Type;

typedef enum{
    
    SAMPLE_middle   = 0b00000000,
    SAMPLE_end      = 0b10000000
    
}Data_sample;

typedef enum
{
    IDLE_high  = 0b00010000,
    IDLE_low   = 0b00000000        
}Clock_Idle;
 
typedef enum
{
    IDLE_2_active    = 0b00000000,
    ACTIVE_2_idle    = 0b01000000
            
}Transmit_Edge;

void spiInit(Type, Data_sample, Clock_Idle, Transmit_Edge); 
void spiWrite(char);
unsigned spiDataReady();
char spiRead();

#endif	/* SPI_H */
