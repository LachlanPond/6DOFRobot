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
volatile int step_generating_compare_match_toggle_J1 = 0;
volatile int step_generating_compare_match_toggle_J = 0;

volatile unsigned int J1_steps = 0;
volatile unsigned int J2_steps = 0;
volatile unsigned int J3_steps = 0;
volatile unsigned int J4_steps = 0;
volatile unsigned int J5_steps = 0;
volatile unsigned int J6_steps = 0;

#include "functions.h"

int main(){

	int micro_steps = 1;
	int radians_per_second_x1000 = 100;

	unsigned char J1_direction = 0;
	unsigned char J2_direction = 0;
	unsigned char J3_direction = 0;
	unsigned char J4_direction = 0;
	unsigned char J5_direction = 0;
	unsigned char J6_direction = 0;

	Setup();
	while(1) {
		if (packet_complete) {
			// Set global variables with packet data
			//OCR0A = packet[0];
			//OCR2A = packet[1];
			J1_steps = (packet[2] << 8) | (packet[3]);
			J2_steps = (packet[4] << 8) | (packet[5]);
			J3_steps = (packet[6] << 8) | (packet[7]);
			J4_steps = (packet[8] << 8) | (packet[9]);
			J5_steps = (packet[10] << 8) | (packet[11]);
			J6_steps = (packet[12] << 8) | (packet[13]);
			J1_direction = (packet[14] == 'P') ? 1 : 0;
			J2_direction = (packet[15] == 'P') ? 1 : 0;
			J3_direction = (packet[16] == 'P') ? 1 : 0;
			J4_direction = (packet[17] == 'P') ? 1 : 0;
			J5_direction = (packet[18] == 'P') ? 1 : 0;
			J6_direction = (packet[19] == 'P') ? 1 : 0;
			if (J6_steps == 12200) {
				USART_Transmit_Msg("testing 123");
			}
			packet_complete = 0;
		}
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

ISR(TIMER0_COMPA_vect) {
	if (step_generating_compare_match_toggle_J1) {
		if (J1_steps > -1) {
			PORTD |= (1<<MTR1_STEP);
			J1_steps--;
		}
	}
	else {
		PORTD &= ~(1<<MTR1_STEP);
	}
	step_generating_compare_match_toggle_J1 = !step_generating_compare_match_toggle_J1;
}

ISR(TIMER2_COMPA_vect) {
	if (step_generating_compare_match_toggle_J) {
		if (J2_steps > -1) {
			PORTD |= (1<<MTR2_STEP);
			J2_steps--;
		}
		if (J3_steps > -1) {
			PORTC |= (1<<MTR3_STEP);
			J3_steps--;
		}
		if (J4_steps > -1) {
			PORTC |= (1<<MTR4_STEP);
			J4_steps--;
		}
		if (J5_steps > -1) {
			PORTC |= (1<<MTR5_STEP);
			J5_steps--;
		}
		if (J6_steps > -1) {
			PORTC |= (1<<MTR6_STEP);
			J6_steps--;
		}
	}
	else {
		PORTD &= ~(1<<MTR2_STEP);
		PORTC &= ~(1<<MTR3_STEP) | ~(1<<MTR4_STEP) | ~(MTR5_STEP) | ~(MTR6_STEP);
	}
	step_generating_compare_match_toggle_J = !step_generating_compare_match_toggle_J;
}