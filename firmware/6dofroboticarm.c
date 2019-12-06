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

struct Joint {
	volatile unsigned int steps;
	volatile unsigned char dir;
};

volatile unsigned int J1_steps = 0;
volatile unsigned int J2_steps = 0;
volatile unsigned int J3_steps = 0;
volatile unsigned int J4_steps = 0;
volatile unsigned int J5_steps = 0;
volatile unsigned int J6_steps = 0;

struct Joint J1 = {0,0};
struct Joint J2 = {0,0};
struct Joint J3 = {0,0};
struct Joint J4 = {0,0};
struct Joint J5 = {0,0};
struct Joint J6 = {0,0};

#include "functions.h"

int main(){

	Setup();
	while(1) {
		if (packet_complete) {
			// Set global variables with packet data
			//OCR0A = packet[0];
			//OCR2A = packet[1];
			J1.steps = (packet[2] << 8) | (packet[3]);
			J2.steps = (packet[4] << 8) | (packet[5]);
			J3.steps = (packet[6] << 8) | (packet[7]);
			J4.steps = (packet[8] << 8) | (packet[9]);
			J5.steps = (packet[10] << 8) | (packet[11]);
			J6.steps = (packet[12] << 8) | (packet[13]);
			J1.dir = (packet[14] == 'P') ? 1 : 0;
			J2.dir = (packet[15] == 'P') ? 1 : 0;
			J3.dir = (packet[16] == 'P') ? 1 : 0;
			J4.dir = (packet[17] == 'P') ? 1 : 0;
			J5.dir = (packet[18] == 'P') ? 1 : 0;
			J6.dir = (packet[19] == 'P') ? 1 : 0;
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
		if (J1.steps > -1) {
			PORTD |= (1<<MTR1_STEP);
			J1.steps--;
		}
	}
	else {
		PORTD &= ~(1<<MTR1_STEP);
	}
	step_generating_compare_match_toggle_J1 = !step_generating_compare_match_toggle_J1;
}

ISR(TIMER2_COMPA_vect) {
	if (step_generating_compare_match_toggle_J) {
		if (J2.steps > -1) {
			PORTD |= (1<<MTR2_STEP);
			J2.steps--;
		}
		if (J3.steps > -1) {
			PORTC |= (1<<MTR3_STEP);
			J3.steps--;
		}
		if (J4.steps > -1) {
			PORTC |= (1<<MTR4_STEP);
			J4.steps--;
		}
		if (J5.steps > -1) {
			PORTC |= (1<<MTR5_STEP);
			J5.steps--;
		}
		if (J6.steps > -1) {
			PORTC |= (1<<MTR6_STEP);
			J6.steps--;
		}
	}
	else {
		PORTD &= ~(1<<MTR2_STEP);
		PORTC &= ~(1<<MTR3_STEP) | ~(1<<MTR4_STEP) | ~(MTR5_STEP) | ~(MTR6_STEP);
	}
	step_generating_compare_match_toggle_J = !step_generating_compare_match_toggle_J;
}