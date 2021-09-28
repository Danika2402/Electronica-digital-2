
#include <SPI.h>
#include <SD.h>

const int chipSelect = PA_3;
File myfile;
String file;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){
    ;
  }

  SPI.setModule(0);
  delay(2500);
  
  Serial.print("Iniciando carta SD");
  pinMode(PA_3, OUTPUT);

  if(!SD.begin(chipSelect)){
    Serial.println("\nNo se encontro SD");
    return;
  }
  Serial.println("\nSD iniciado");
  Serial.println("\nEscribe el nombre del documento \nque quiere abrir");

//  myfile = SD.open("lab07.txt");

  //if(myfile){
    //Serial.println("lab07.txt:");

    //while(myfile.available()){
      //Serial.write(myfile.read());
    //}
    //myfile.close();
  //}else{
    //Serial.println("No se encontro Archivo");
  //}
}

void loop() {
  // put your main code here, to run repeatedly: 

  while(Serial.available()){
    delay(20);
    char dato = Serial.read();
    file += dato;
  }

  if(file == ("love\n")){
      Serial.println("Abriendo archivo Love\n");
      myfile = SD.open("love.txt");
      
      while(myfile.available()){
        Serial.write(myfile.read());
      }
      myfile.close();
      Serial.println("\nSe cerro archivo");
      
  }else if (file == "persona\n"){
      Serial.println("Abriendo archivo Persona\n");
      myfile = SD.open("Persona.txt");
  
      while(myfile.available()){
        Serial.write(myfile.read());
      }
      myfile.close();
      Serial.println("\nSe cerro archivo");
      
  }else if(file == "zelda\n"){
      Serial.println("Abriendo archivo Zelda\n");
      myfile = SD.open("zelda.txt");
  
      while(myfile.available()){
        Serial.write(myfile.read());
      }
      myfile.close();
      Serial.println("Se cerro archivo");
  }else if(file != ""){
    Serial.println("\nEse archivo no existe oh no es valido");
  }
  file = "";
  delay(1);
  
}
