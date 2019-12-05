import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.1)
print(ser.name)

J1_step = 1000
J2_step = 200
J3_step = 3
J4_step = 80
J5_step = 1200
J6_step = 12200

J1_dir = 1
J2_dir = 0
J3_dir = 0
J4_dir = 1
J5_dir = 1
J6_dir = 1

while(1):
	packet = bytearray()
	packet.append(100)
	packet.append(200)
	packet.append(J1_step >> 8)
	packet.append(J1_step & 0xFF)
	packet.append(J2_step >> 8)
	packet.append(J2_step & 0xFF)
	packet.append(J3_step >> 8)
	packet.append(J3_step & 0xFF)
	packet.append(J4_step >> 8)
	packet.append(J4_step & 0xFF)
	packet.append(J5_step >> 8)
	packet.append(J5_step & 0xFF)
	packet.append(J6_step >> 8)
	packet.append(J6_step & 0xFF)
	packet.append(J1_dir)
	packet.append(J2_dir)
	packet.append(J3_dir)
	packet.append(J4_dir)
	packet.append(J5_dir)
	packet.append(J6_dir)
	packet.append(ord('.'))
	ser.write(packet)
	time.sleep(1)
	print(ser.read(11))