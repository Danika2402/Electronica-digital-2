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

byte j1 = 0;
byte j2 = 0;
byte contador1 = 0;
byte contador2 = 0;

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

}

void loop() {

   push1 = digitalRead(button1);
   push2 = digitalRead(button2);
   
   semaforo(1,0,0);
   semaforo(0,1,0);
   semaforo(0,0,1);
   semaforo(0,0,0);

     if(push1 == 0){
        j1++;
        delay(1);
     }else if (push2 == 0){
        j2++;
        delay(1);
     }

     if(j1 == 8){

        
        
                   
     }
    
   
   
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
