import serial
from serial.tools import list_ports

listofCOMs = []
print("=====\nList of COMS")
for p in list_ports.comports():
    print(p.device)
    listofCOMs.append(p.device)
print("=======================")
port = input('COM Port: ')
try:
    ser = serial.Serial(port, 9600)
    ser.close()
except:
    print("Unknown error")

