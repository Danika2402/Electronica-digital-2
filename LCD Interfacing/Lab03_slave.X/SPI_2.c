/*
 * File:   SPI.c
 * Author: Danika
 *
 * Created on 29 de julio de 2021, 09:14 PM
 */
//******************************************************************************

#include "SPI.h"

void spiInit(Type sType, Data_sample sDataSample, Clock_Idle sClockIdle, Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) //if de Esclavo
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else              //If de Maestro
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF ); // esperar por recivir el dato
}

void spiWrite(char dat)  //Write data to SPI bus
{
    SSPBUF = dat;
}

unsigned spiDataReady() //Check whether the data is ready to read
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead() //REad the received data
{
    spiReceiveWait();        // wait until the all bits receive
    return(SSPBUF); // read the received data from the buffer
}