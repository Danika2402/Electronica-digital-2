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
#include "USART.h"  //Librerias
#include "LCD.h"
#include "I2C.h"
#include "ADC.h"
#define _XTAL_FREQ 8000000

void setup(void);   //variables y funciones 
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
                
            if(RB2 == 0){   //interrupcion push button pull up
                cont++;     //en solo el pin RB2, que incrementa al variable
            }               //cont
        }
        RBIF = 0;           //se baja la bandera
}

void main(void) {
    
    setup();
    while(1){
        
        if(ADCON0bits.GO == 0){
            if (ADCON0bits.CHS  == 12){     //La primera fotoresistencia esta
                foto1   = ADRESH;           //en el canal 12, se guarda el
                ADCON0bits.CHS  = 10;       //ADRESH en la variable "foto1"


            } else if (ADCON0bits.CHS  == 10){  //Se cambio de canal al 10
                PORTA = ADRESH;                 //donde esta la segunda  
                foto2   = ADRESH;               //fotoresistencia
                ADCON0bits.CHS  = 12;            
                        
            }
            __delay_us(50); 
            ADCON0bits.GO = 1;       //se baja la bandera
        }
        
        
        decmil1 = foto1/10000;  //las fotoresistencias reales pueden llegar 
        mil1 = (foto1 - (decmil1*10000))/1000;//hasta 20,000
        cen1  = (foto1 - (decmil1*10000) - (mil1*1000))/100;
        dec1 = ((foto1 - (decmil1*10000) - (mil1*1000)-(cen1*100)) / 10);
        uni1 = ((foto1 - (decmil1*10000) - (mil1*1000)-(cen1*100)) - (dec1*10));
                
        decmil2 = foto2/10000;//las resistencias son poco a poco 
        mil2 = (foto2 - (decmil2*10000))/1000;//divididas entre miles, cent, dec,etc
        cen2  = (foto2 - (decmil2*10000) - (mil2*1000))/100;
        dec2 = ((foto2 - (decmil2*10000) - (mil2*1000)-(cen2*100)) / 10);
        uni2 = ((foto2 - (decmil2*10000) - (mil2*1000)- (cen2*100)) - (dec2*10));

        seg = read_rtc(0x00);   //Aqui llamamos al RTC
        min = read_rtc(0x01);   //llamamos a la funcion y le enviamos un dato
        hora = read_rtc(0x02);  //en hexadecimal, segun el datasheet
        dia = read_rtc(0x04);
        mes = read_rtc(0x05);
        year = read_rtc(0x06);
        
        seg = (seg >>4)*10 + (seg & 0x0f);  //convertir BDC a decimal
        min = (min >>4)*10 +(min & 0x0f);
        hora = (hora>>4)*10 + (hora & 0x0f);
        dia = (dia>>4)*10 + (dia & 0x0f);
        mes = (mes>>4)*10 + (mes & 0x0f);
        year=(year >>4)*10 + (year & 0x0f);
        
        time[12] = (seg % 10) + 48; //En esta variable guardamos las otras en
        time[11] = (seg/10) + 48;   //forma de un caracter
        time[9] = (min % 10) + 48;  
        time[8] = (min/10) + 48;
        time[6] = (hora % 10) + 48;
        time[5] = (hora/10) + 48;
        
        calendario[14]=(year % 10) + 48;    //igual con esta variable
        calendario[13] = (year/10) + 48;
        calendario[9] = (mes % 10) + 48;
        calendario[8] = (mes/10) + 48;
        calendario[6] = (dia % 10) + 48;
        calendario[5] = (dia/10) + 48;
                
        //se suman las variables para asi compararlas entre si 
        fotoresistencia1 = (decmil1*10000) + (mil1*1000) + (cen1*100) + (dec1*10) + (uni1);
        fotoresistencia2 = (decmil2*10000) + (mil2*1000) + (cen2*100) + (dec2*10) + (uni2);
        
        if(cont ==0){
            LCD_XY(0,0);
            LCD_Cadena("Foto1     Foto2");          //con la variable cont que
            valores(decmil1,mil1,cen1,dec1,uni1);   //es incrementada por el boton
            valores2(decmil2,mil2,cen2,dec2,uni2);  //podemos controlar lo que
                                                //el LCD muesta
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
            //aqui comparamos las fotoresistencias para controlar el servo
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
        
        if (PIR1bits.RCIF == 1){    //aqui empezamos la comunicacion serial 
                                    //la variable listo recibe un caracter de
            listo = USART_Rx();     //python, dependiendo de este
                                    //entonces el pic manda los datos de 
                if(listo == 'f'){           //la foto1 oh la foto2
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
    
    INTCONbits.GIE = 1;     //habilitar interrupciones para puerto B pull up
    INTCONbits.RBIE = 1;    
    INTCONbits.RBIF = 0; 
    
    cont = 0;
    
    canal(12);              //llamamos al canal y configuracion ADC
    ADC_Int();

    USART_Int();            //configuracion USART
    LCD_Init();             //configuracion del LCD
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

unsigned short read_rtc(unsigned short address){
    
    unsigned short data;        //el adress es el que controla que dato
    I2C_Master_Start();         //queremos del rtc, segundos, minutos o meses
    I2C_Master_Write(0xD0);
    I2C_Master_Write(address);
    I2C_Master_Stop();
    __delay_ms(200);
    
    I2C_Master_Start();         //mandamos a que el rtc enviara los datos al 
    I2C_Master_Write(0xD1);     //master y lo guardamos en una variable
    data = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(200);
    return data;
    
}

void valores(char decenasdemiles,char miles,char centenas, char decenas, char unidades){
    
    decenasdemiles +=48;
    miles += 48;                //aqui organizamos las variables de cada
    centenas += 48;             //fotoresistencia para que aparesca en el LCD
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
    centenas += 48;         //misma funcion pero para la segunda fotoresistencia
    decenas += 48;
    unidades += 48;
    
    LCD_XY(2,9);
    LCD_Data(decenasdemiles);
    LCD_Data(miles);
    LCD_Data(centenas);
    LCD_Data(decenas);
    LCD_Data(unidades);
    
}