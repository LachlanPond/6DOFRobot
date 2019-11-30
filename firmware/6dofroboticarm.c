#ifndef __AVR_ATmega328P__
	#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "defines.h"
#include "globals.h"

int micro_steps = 1;
int radians_per_second_x1000 = 100;
int packet_complete = 0;
int packet_byte_counter = 0;
int J1_delay = 0;
int J_delay = 0;
int J1_delay_count = 0;
int J_delay_count = 0;
unsigned char packet[32];
unsigned int J1_steps = 0;
unsigned int J2_steps = 0;
unsigned int J3_steps = 0;
unsigned int J4_steps = 0;
unsigned int J5_steps = 0;
unsigned int J6_steps = 0;
unsigned char J1_direction = 0;
unsigned char J2_direction = 0;
unsigned char J3_direction = 0;
unsigned char J4_direction = 0;
unsigned char J5_direction = 0;
unsigned char J6_direction = 0;

#include "functions.h"

int main(void){

	J1_delay = Set_Joint_Speed(radians_per_second_x1000, 1, J1_GEAR_RATIO, J_GEAR_RATIO, micro_steps);
	J_delay = Set_Joint_Speed(radians_per_second_x1000, 0, J1_GEAR_RATIO, J_GEAR_RATIO, micro_steps);

	Setup();
	while(1) {
		Motors();
		// If a new packet comes through, update
		Check_Packet();

		J1_delay_count++;
		J_delay_count++;
		_delay_ms(1);
	}
	return 0;
}

ISR(USART_RX_vect) {
	unsigned char byte = UDR0;
	packet[packet_byte_counter] = byte;
	packet_byte_counter++;
	if (byte == '\n') {
		packet_complete = 1;
		packet_byte_counter = 0;
	}
}