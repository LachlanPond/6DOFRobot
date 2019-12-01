import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 115200)
print(ser.name)
while(1):
	ser.write('hello\n')
	print(ser.read())
	time.sleep(0.1)