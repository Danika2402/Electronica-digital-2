#Electronica digital
#Proyecto 1
#3/09/2021
#Danika Andrino

from Adafruit_IO import Client, RequestError, Feed
import serial
import time

ADAFRUIT_I0_KEY = "aio_wTCW15faPQA7osaqqW8r6cDKZnoG"
ADAFRUIT_I0_USERNAME = "Danika"
aio = Client(ADAFRUIT_I0_USERNAME,ADAFRUIT_I0_KEY)

proteus = serial.Serial("COM2",9600)
proteus.timeout = 3
time.sleep(1)

foto1 = 0
foto2 = 0

def menu():
    print("*************************")
    print("** 1: Correr programa  **")
    print("** 2: Terminar programa**")
    print("*************************")

while(True):
    with proteus:
        menu()
        Decidir = input("Elige una opcion:")

        if(Decidir == '1'):

            proteus.write(b'f')

            digital_feed= aio.feeds('fotoresistencia1')        
            foto1 = (proteus.readline(6).decode('ascii'))
            foto1 = (str(foto1))
            foto1 = int(foto1)
            aio.send_data(digital_feed.key, (foto1))

            proteus.write(b'd')

            digital_feed2= aio.feeds('fotoresistencia2')        
            foto2 = (proteus.readline(6).decode('ascii'))
            foto2 = int(str(foto2))
            aio.send_data(digital_feed2.key, (foto2))

        elif(Decidir == '2'):
            print("Fin del programa")
            break
        
        
        
