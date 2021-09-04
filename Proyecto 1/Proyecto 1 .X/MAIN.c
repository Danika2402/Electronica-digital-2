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
int TEMP;
char foto1, foto2;
char servo1,servo2;
char cont;
char decmil1,mil1,cen1, dec1, uni1;
char decmil2,mil2,cen2, dec2, uni2;
unsigned short read_rtc(unsigned short address);

char time[]= "TIME:  :  :  ";
char calendario[]= "DATE:  /  /20  ";
unsigned long fotoresistencia1;
unsigned long fotoresistencia2;

int seg,min,dia,hora,year,mes;

char seg1,seg2;
char listo;

//char cen3, dec3, uni3;

void valores(char decenasdemiles,char miles,char centenas, char decenas, char unidades);
void valores2(char decenasdemiles,char miles,char centenas, char decenas, char unidades);

void __interrupt() isr(void){
        if(RBIF == 1){
                
            if(RB2 == 0){
                cont++;
            }
        }
        RBIF = 0;
}

void main(void) {
    
    setup();
    while(1){
        
        if(ADCON0bits.GO == 0){
            if (ADCON0bits.CHS  == 12){     //cuando este en el canal 12 
                foto1   = ADRESH;           //El potenciometro esta conectado a ese canal
                ADCON0bits.CHS  = 10;       //pot se convierte y se muestre en el puerto


            } else if (ADCON0bits.CHS  == 10){
                PORTA = ADRESH;                                //Despues de eso se cambia al canal 10 
                foto2   = ADRESH;               //donde hay un segundo potenciometro
                ADCON0bits.CHS  = 12;       //este manda sus valores a convertirlos
                        //y los guarda en la variable p
            }
            __delay_us(50); 
            ADCON0bits.GO = 1;       
        }
        
        
        decmil1 = foto1/10000;
        mil1 = (foto1 - (decmil1*10000))/1000;
        cen1  = (foto1 - (decmil1*10000) - (mil1*1000))/100;
        dec1 = ((foto1 - (decmil1*10000) - (mil1*1000)-(cen1*100)) / 10);
        uni1 = ((foto1 - (decmil1*10000) - (mil1*1000)-(cen1*100)) - (dec1*10));
                
        decmil2 = foto2/10000;
        mil2 = (foto2 - (decmil2*10000))/1000;
        cen2  = (foto2 - (decmil2*10000) - (mil2*1000))/100;
        dec2 = ((foto2 - (decmil2*10000) - (mil2*1000)-(cen2*100)) / 10);
        uni2 = ((foto2 - (decmil2*10000) - (mil2*1000)- (cen2*100)) - (dec2*10));

        seg = read_rtc(0x00);
        min = read_rtc(0x01);
        hora = read_rtc(0x02);
        dia = read_rtc(0x04);
        mes = read_rtc(0x05);
        year = read_rtc(0x06);
        
        seg = (seg >>4)*10 + (seg & 0x0f);
        min = (min >>4)*10 +(min & 0x0f);
        hora = (hora>>4)*10 + (hora & 0x0f);
        dia = (dia>>4)*10 + (dia & 0x0f);
        mes = (mes>>4)*10 + (mes & 0x0f);
        year=(year >>4)*10 + (year & 0x0f);
        
        time[12] = (seg % 10) + 48;
        time[11] = (seg/10) + 48;
        time[9] = (min % 10) + 48;
        time[8] = (min/10) + 48;
        time[6] = (hora % 10) + 48;
        time[5] = (hora/10) + 48;
        
        calendario[14]=(year % 10) + 48;
        calendario[13] = (year/10) + 48;
        calendario[9] = (mes % 10) + 48;
        calendario[8] = (mes/10) + 48;
        calendario[6] = (dia % 10) + 48;
        calendario[5] = (dia/10) + 48;
                
                
        fotoresistencia1 = (decmil1*10000) + (mil1*1000) + (cen1*100) + (dec1*10) + (uni1);
        fotoresistencia2 = (decmil2*10000) + (mil2*1000) + (cen2*100) + (dec2*10) + (uni2);
        
        if(cont ==0){
            LCD_XY(0,0);
            LCD_Cadena("Foto1     Foto2");
            valores(decmil1,mil1,cen1,dec1,uni1);
            valores2(decmil2,mil2,cen2,dec2,uni2);
            
        }else if (cont == 1){
            LCD_XY(0,0);
            LCD_Cadena(time);
            LCD_XY(2,0);
            LCD_Cadena(calendario);
        }else if (cont == 2){
            LCD_Cadena("           ");
            LCD_Cadena("           ");
            cont = 0;
        }
        
        if((fotoresistencia2 <= 100) && (fotoresistencia1 <= 100)){
            
                CCPR1L  = 187;              //1.5ms 0 grados
                CCP1CONbits.DC1B0 = 1;      //bits menos significativos CCP1
                CCP1CONbits.DC1B1 = 0;    
            
        }else if((fotoresistencia2 >=9000)  && (fotoresistencia1 >= 9000)){
                CCPR1L  = 187;              //1.5ms 0 grados
                CCP1CONbits.DC1B0 = 1;      //bits menos significativos CCP1
                CCP1CONbits.DC1B1 = 0; 
                
        }else if(fotoresistencia2 > fotoresistencia1){
                CCPR1L  = 157;              //1.25ms 45 grados
                CCP1CONbits.DC1B0 = 1;      //bits menos significativos CCP1
                CCP1CONbits.DC1B1 = 1;
        }else if(fotoresistencia2 < fotoresistencia1){
                CCPR1L  = 218;              //1.75ms 135 grados
                CCP1CONbits.DC1B0 = 1;      //bits menos significativos CCP1
                CCP1CONbits.DC1B1 = 1;
        }
        
        if (PIR1bits.RCIF == 1){
        
            listo = USART_Rx();

                if(listo == 'f'){
                    USART_Tx(decmil1 + 48);
                    USART_Tx(mil1 + 48);
                    USART_Tx(cen1 + 48);
                    USART_Tx(dec1 + 48);
                    USART_Tx(uni1 + 48);
            
                }else if(listo == 'd'){
                    USART_Tx(decmil2 + 48);
                    USART_Tx(mil2 + 48);
                    USART_Tx(cen2 + 48);
                    USART_Tx(dec2 + 48);
                    USART_Tx(uni2 + 48);
                    listo = 0;
                }
            PIR1bits.RCIF = 0; 
        }
        
    }  
}

void setup(void){
    ANSEL =  0x00;
    ANSELH = 0b00010100;    //canal 12 y 10
    
    TRISA = 0x00;
    TRISE = 0x01;
    TRISB = 0x07;       //pines 0, 1 y 2 entrada   
    TRISC = 0b10000000; //RX entrada
    TRISD = 0;
    
    PORTA = 0x00;
    PORTE = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    //Configuracion PWM
    //1.
    TRISCbits.TRISC1 = 1;       //puerto C1 como entrada

    //2. Setear periodo con PR2
    PR2 = 250;          //2ms 
    
    //3. Configuración CCPCON PWM
    CCP1CONbits.P1M = 0b00;         //unica salida
    CCP1CONbits.CCP1M = 0b00001100; //modo PWM CCP1
    
    //4. Cargar el valor de CCPRxL
    CCPR1L  = 125;              //1ms
    CCP1CONbits.DC1B0 = 0;      //bits menos significativos CCP1
    CCP1CONbits.DC1B1 = 0;       
    
    //5. Configurar el TmR2 
    //tmr2
    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS = 0b11;    //prescaler 1:16
    T2CONbits.TMR2ON = 1;
    
    //6.
    while(!PIR1bits.TMR2IF);    //espera completar tmr2
    PIR1bits.TMR2IF = 0;
    
    //7.      
    TRISCbits.TRISC1 = 0;   //salida de PWM
    
    OSCCONbits.IRCF = 0b111;    //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;  
    
            //Config. PULL-UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB2 = 1;
    IOCBbits.IOCB2 = 1; // pines en lo que habra interrupcion por cambio 
    
    INTCONbits.GIE = 1;     
    INTCONbits.RBIE = 1;    
    INTCONbits.RBIF = 0; 
    
    cont = 0;
    
    canal(12);   
    ADC_Int();

    USART_Int();
    LCD_Init();
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

unsigned short read_rtc(unsigned short address){
    
    unsigned short data;
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(address);
    I2C_Master_Stop();
    __delay_ms(200);
    
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    data = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(200);
    return data;
    
}

void valores(char decenasdemiles,char miles,char centenas, char decenas, char unidades){
    
    decenasdemiles +=48;
    miles += 48;
    centenas += 48;
    decenas += 48;
    unidades += 48;
    
    LCD_XY(2,1);
    LCD_Data(decenasdemiles);
    LCD_Data(miles);
    LCD_Data(centenas);
    LCD_Data(decenas);
    LCD_Data(unidades);
    
}

void valores2(char decenasdemiles,char miles,char centenas, char decenas, char unidades){
    
    decenasdemiles +=48;
    miles += 48;
    centenas += 48;
    decenas += 48;
    unidades += 48;
    
    LCD_XY(2,9);
    LCD_Data(decenasdemiles);
    LCD_Data(miles);
    LCD_Data(centenas);
    LCD_Data(decenas);
    LCD_Data(unidades);
    
}