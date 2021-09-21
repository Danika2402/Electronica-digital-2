#define LED0_1  PB_3
#define LED1_1  PC_4
#define LED2_1  PC_5
#define LED3_1  PC_6
#define LED4_1  PC_7
#define LED5_1  PD_6
#define LED6_1  PA_3
#define LED7_1  PA_2

#define LED0_2  PB_5
#define LED1_2  PB_0
#define LED2_2  PB_1
#define LED3_2  PE_4
#define LED4_2  PE_5
#define LED5_2  PB_4
#define LED6_2  PA_5
#define LED7_2  PA_6

#define red PF_1    //red led
#define green PF_3  //led verde
#define blue PF_2   //led azul

#define ganador1 PD_0 //si el j1 gana
#define ganador2 PD_1 //si el j2 gana

#define button1  PF_4 //sw1
#define button2  PF_0 //sw2

byte jugador1 = 0;
byte jugador2 = 0;
byte contador1 = 0;
byte contador2 = 0;
byte push1 = 0;
byte push2 = 0;

bool inicio = 0;

void setup() {
  pinMode(LED0_1, OUTPUT);
  pinMode(LED1_1, OUTPUT);
  pinMode(LED2_1, OUTPUT);
  pinMode(LED3_1, OUTPUT);
  pinMode(LED4_1, OUTPUT);
  pinMode(LED5_1, OUTPUT);
  pinMode(LED6_1, OUTPUT);
  pinMode(LED7_1, OUTPUT);

  pinMode(LED0_2, OUTPUT);
  pinMode(LED1_2, OUTPUT);
  pinMode(LED2_2, OUTPUT);
  pinMode(LED3_2, OUTPUT);
  pinMode(LED4_2, OUTPUT);
  pinMode(LED5_2, OUTPUT);
  pinMode(LED6_2, OUTPUT);
  pinMode(LED7_2, OUTPUT);

  pinMode(red,OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  pinMode(ganador1, OUTPUT);
  pinMode(ganador2, OUTPUT);

}

void loop() {

   push1 = digitalRead(button1);
   push2 = digitalRead(button2);

   if(push1 == 0){
     semaforo(1,0,0);
     semaforo(0,1,0);
     semaforo(0,0,1);
     semaforo(0,0,0);
    
     inicio = 1; 
   }

   if(inicio == 1){
   
     if(push1 == 0){
        jugador1++;
        delay(1);
     }else if (push2 == 0){
        jugador2++;
        delay(1);
     }

     if(jugador1 == 8){

        jugador2=0;
        digitalWrite(ganador1, 1);
        digitalWrite(ganador2, 0);
        delay(1500);
        inicio = 0;   
                   
     }else if(jugador2 == 8){
        jugador1=0;
        digitalWrite(ganador1, 0);
        digitalWrite(ganador2, 1);
        delay(1500);
        inicio = 0;
    }else {
        digitalWrite(ganador1, 0);
        digitalWrite(ganador2, 0);
    }
   }
   
    contador1 = leds(jugador1);
    contador2 = leds(jugador2);

    digitalWrite(LED0_1,bitRead(contador1,0));
    digitalWrite(LED1_1,bitRead(contador1,1));
    digitalWrite(LED2_1,bitRead(contador1,2));
    digitalWrite(LED3_1,bitRead(contador1,3));
    digitalWrite(LED4_1,bitRead(contador1,4));
    digitalWrite(LED5_1,bitRead(contador1,5));
    digitalWrite(LED6_1,bitRead(contador1,6));
    digitalWrite(LED7_1,bitRead(contador1,7));

    digitalWrite(LED0_2,bitRead(contador2,0));
    digitalWrite(LED1_2,bitRead(contador2,1));
    digitalWrite(LED2_2,bitRead(contador2,2));
    digitalWrite(LED3_2,bitRead(contador2,3));
    digitalWrite(LED4_2,bitRead(contador2,4));
    digitalWrite(LED5_2,bitRead(contador2,5));
    digitalWrite(LED6_2,bitRead(contador2,6));
    digitalWrite(LED7_2,bitRead(contador2,7));
}

void semaforo(bool r,bool b, bool g ){

    digitalWrite(red,r);
    digitalWrite(blue,b);
    digitalWrite(green,g);
    delay(1000);
}

byte leds(byte c){

    switch(c){

      case 0:
        c = 0;
        break;
      
      case 1:
        c = 1;
        break;

      case 2:
        c = 3;
        break;
        
      case 3:
        c = 7;
        break;

      case 4:
        c = 15;
        break;

      case 5:
        c = 31;
        break;

      case 6:
        c = 63;
        break;

      case 7:
        c = 127;
        break;

      case 8:
        c = 255;
        break;
    }
    return c;
   
}
