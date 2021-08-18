#Electronica digital
#Codigo adafruit - lab05
#13/08/2021
#Danika Andrino

from Adafruit_IO import Client, RequestError, Feed
import serial
import time

ada = 0
portA = 0

ADAFRUIT_I0_KEY = "aio_bFvR07Ph7b9jDZjrhs9HoDuV4xMW"
ADAFRUIT_I0_USERNAME = "Danika"
aio = Client(ADAFRUIT_I0_USERNAME,ADAFRUIT_I0_KEY)

proteus = serial.Serial("COM2",9600)
proteus.timeout = 3
time.sleep(1)

def menu():
    print("*************************")
    print("** 1: Correr programa  **")
    print("** 2: Terminar programa**")
    print("*************************")

    
while(True):
    with proteus:
        menu()
        Decidir = input("Elige una opcion: ")

        if(Decidir == '1'):

            proteus.write(b's')
            portA = (proteus.readline(3).decode('ascii')) #Recivir contador del pic
            digital_feed = aio.feeds('contador')    #Feed del contador
            portA = str(portA)
            portA = int((portA))
            aio.send_data(digital_feed.key, (portA))

            digital_ada = aio.feeds('adafruit')    #Feed de adafruit
            ada = int((aio.receive(digital_ada.key)).value)

            proteus.write(str(ada).encode('ascii'))

            print("**************")
            print("Valor del puerto D:", portA)
            print("Valor del puerto A:", ada)
            print("**************")
            
        elif(Decidir == '2'):
            print("Se termino el programa")
            break
