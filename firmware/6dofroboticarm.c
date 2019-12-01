#ifndef __AVR_ATmega328P__
	#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "defines.h"

volatile int packet_complete = 0;
volatile int packet_byte_counter = 0;
volatile unsigned char packet[32];

#include "functions.h"

int main(){

	int micro_steps = 1;
	int radians_per_second_x1000 = 100;

	int J1_delay = 0;
	int J_delay = 0;
	int J1_delay_count = 0;
	int J_delay_count = 0;

	unsigned int J1_steps;
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

	J1_delay = Set_Joint_Speed(radians_per_second_x1000, 1, J1_GEAR_RATIO, J_GEAR_RATIO, micro_steps);
	J_delay = Set_Joint_Speed(radians_per_second_x1000, 0, J1_GEAR_RATIO, J_GEAR_RATIO, micro_steps);

	J1_delay = 30000;
	J1_steps = 30000;

	Setup();
	USART_Transmit_Msg("It's being reset!");
	while(1) {
		//Check_Packet(&packet_complete, packet);
		// if (J1_steps > 0) {
		// 	J1_steps--;
		// }
		// if (J1_steps == 2) {
		// 	USART_Transmit_Byte('@');
		// }
	}
	return 0;
}

ISR(USART_RX_vect) {
	unsigned char byte = UDR0;
	packet[packet_byte_counter] = byte;
	packet_byte_counter++;
	if (byte == '.') {
		packet_complete = 1;
		packet_byte_counter = 0;
	}
}