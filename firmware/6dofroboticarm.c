#ifndef __AVR_ATmega328P__
	#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "defines.h"
#include "globals.h"

int micro_steps = 1;
int radians_per_second_x1000 = 100;
int packet_complete = 0;
int packet_byte_counter = 0;
unsigned char packet[32];
double J1_degrees = 0;
double J2_degrees = 0;
double J3_degrees = 0;
double J4_degrees = 0;
double J5_degrees = 0;
double J6_degrees = 0;
unsigned int J1_degrees_setpoint_x100 = 0;
unsigned int J2_degrees_setpoint_x100 = 0;
unsigned int J3_degrees_setpoint_x100 = 0;
unsigned int J4_degrees_setpoint_x100 = 0;
unsigned int J5_degrees_setpoint_x100 = 0;
unsigned int J6_degrees_setpoint_x100 = 0;

#include "functions.h"

int main(void){

	int joint1_delay = Set_Joint_Speed(radians_per_second_x1000, 1, J1_GEAR_RATIO, J_GEAR_RATIO, micro_steps);
	int joint_delay = Set_Joint_Speed(radians_per_second_x1000, 0, J1_GEAR_RATIO, J_GEAR_RATIO, micro_steps);
	int joint1_delay_count = 0;
	int joint_delay_count = 0;

	Setup();
	while(1) {
		// Introduce a soft delay to effectively set the speed of each joint
		if (joint1_delay_count == joint1_delay) {
			joint1_delay_count = 0;
		}
		if (joint_delay_count == joint_delay) {
			joint_delay_count = 0;
		}

		// If a new packet comes through, update
		Check_Packet();

		joint1_delay_count++;
		joint_delay_count++;
		_delay_ms(1);
	}
	return 0;
}

ISR(USART_RX_vect) {
	// Return received byte from the buffer
	unsigned char byte = UDR0;
	packet[packet_byte_counter] = byte;
	packet_byte_counter++;
	if (byte == '\n') {
		packet_complete = 1;
		packet_byte_counter = 0;
	}
}