#ifndef __AVR_ATmega328P__
	#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "globals.h"

void Setup() {
	// First set the data direction for motor controlling signal pins
	DDRD |= 0b11111100;	// The last two bits of register D are for TX and RX
	DDRC |= 0b00111111;	// bit 7 is reserved/unused, bit 6 is reset
	DDRB |= 0b00000111;

	// Setup the USART serial with baud 115200, 8bit byte size, no parity, 1 stopbit
	UBRR0H = 0b00000000;		// Select the 115200 baud rate option for a 16Mhz clock (UBRR0 = 8)
	UBRR0L = 0b00001000;
	// Enable RX and TX, and the RX and TX interrupts
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1 << TXCIE0);
	// Frame format is set in the UCSRnC register but the defaults already give us 8bit, no parity, and 1 stopbit
	// Enable global interrupts in order to use the RX and TX interrupts
	sei();

	// Setup data direction registers
	DDRB = 0b00111111;
	DDRC = 0b00111111;
	DDRD = 0b11111100;
}

void USART_Transmit(unsigned char data) {
	// Wait for the trasmit buffer to be empty
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data into the buffer, sending it
	UDR0 = data;
}

int Set_Joint_Speed(int radians_per_second_x1000, int joint1, int J1_gear_ratio, int j_gear_ratio, int micro_steps) {
	double radians_per_second = (double)radians_per_second_x1000 / 1000;
	double degrees_per_pulse = 0;
	if (joint1) {
		degrees_per_pulse = (DEGREES_PER_STEP/(double)micro_steps) * (1/(double)J1_GEAR_RATIO);
	}
	else {
		degrees_per_pulse = (DEGREES_PER_STEP/(double)micro_steps) * (1/(double)J_GEAR_RATIO);
	}
	double radians_per_pulse = degrees_per_pulse * (PI/180);
	double pulses_to_make_a_radian = 1/radians_per_pulse;
	int step_delay = (MS_IN_SEC / pulses_to_make_a_radian) / radians_per_second;
	return step_delay;
}

void Set_Motor_Directions() {
	if (J1_direction) PORTC |= (1<<MTR1_DIR); else PORTC &= ~(1<<MTR1_DIR);
	if (J2_direction) PORTC |= (1<<MTR2_DIR); else PORTC &= ~(1<<MTR2_DIR);
	if (J3_direction) PORTD |= (1<<MTR3_DIR); else PORTD &= ~(1<<MTR3_DIR);
	if (J4_direction) PORTD |= (1<<MTR4_DIR); else PORTD &= ~(1<<MTR4_DIR);
	if (J5_direction) PORTD |= (1<<MTR5_DIR); else PORTD &= ~(1<<MTR5_DIR);
	if (J6_direction) PORTD |= (1<<MTR6_DIR); else PORTD &= ~(1<<MTR6_DIR);
}

void Check_Packet() {
	if (packet_complete) {
		// Set global variables with packet data
		radians_per_second_x1000 = (packet[1] << 8) | (packet[0]);
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
		Set_Motor_Directions();
	}
}

void Motors() {
	if (J1_delay_count == J1_delay) {	// Soft delay to handle speed of joint rotation
		if (J1_steps > 0) {
			PORTD |= (1<<MTR1_STEP);	// Step the motor
			J1_steps--;
		}
		J1_delay_count = 0;
	}
	else {
		// Reset the step signal to be ready for the next step
		PORTD &= ~(1<<MTR1_STEP);
	}
	if (J_delay_count == J_delay) {
		if (J2_steps > 0) {
			PORTD |= (1<<MTR2_STEP);	// Step the motor
			J2_steps--;
		}
		if (J3_steps > 0) {
			PORTC |= (1<<MTR3_STEP);	// Step the motor
			J3_steps--;
		}
		if (J4_steps > 0) {
			PORTC |= (1<<MTR4_STEP);	// Step the motor
			J4_steps--;
		}
		if (J5_steps > 0) {
			PORTC |= (1<<MTR5_STEP);	// Step the motor
			J5_steps--;
		}
		if (J6_steps > 0) {
			PORTC |= (1<<MTR6_STEP);	// Step the motor
			J6_steps--;
		}
		J_delay_count = 0;
	}
	else {
		// Reset the step signal to be ready for the next step
		PORTD &= ~(1<<MTR2_STEP);
		PORTC &= ~(1<<MTR3_STEP | 1<<MTR4_STEP | 1<<MTR5_STEP | 1<<MTR6_STEP);
	}
}