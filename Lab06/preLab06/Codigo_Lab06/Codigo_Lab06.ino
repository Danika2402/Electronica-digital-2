#define LED0  PB_3
#define LED1  PC_4
#define LED2  PC_5
#define LED3  PC_6
#define LED4  PC_7
#define LED5  PD_6
#define LED6  PB_7
#define LED7  PB_6

const int button = PUSH1;

int push = 0;
char contador = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  push = digitalRead(button);

   if (push = HIGH){
      contador = contador + 1;
   }else {
    contador = contador;
   }

  digitalWrite(LED0,bitRead(contador,0));
  digitalWrite(LED1,bitRead(contador,1));
  digitalWrite(LED2,bitRead(contador,2));
  digitalWrite(LED3,bitRead(contador,3));
  digitalWrite(LED4,bitRead(contador,4));
  digitalWrite(LED5,bitRead(contador,5));
  digitalWrite(LED6,bitRead(contador,6));
  digitalWrite(LED7,bitRead(contador,7));
  
}
