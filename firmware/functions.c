#include <avr/io.h>
#include <avr/interrupt.h>
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
}

unsigned char USART_Receive() {
	// Wait for data to be received
	while(!(UCSR0A & (1<<RXC0)));
	// Return received byte from the buffer
	return UDR0;
}

void USART_Transmit(unsigned char data) {
	// Wait for the trasmit buffer to be empty
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data into the buffer, sending it
	UDR0 = data;
}

int Set_Joint_Speed(int radians_per_second_x1000, int joint1, int J1_gear_ratio, int j_gear_ratio, int micro_steps) {
	double radians_per_second = radians_per_second_x1000 / 1000;
	double degrees_per_pulse = 0;
	if (joint1) {
		degrees_per_pulse = (DEGREES_PER_STEP/micro_steps) * (1/J1_GEAR_RATIO);
	}
	else {
		degrees_per_pulse = (DEGREES_PER_STEP/micro_steps) * (1/J_GEAR_RATIO);
	}
	double radians_per_pulse = degrees_per_pulse * (PI/180);
	int pulses_to_make_a_radian = 1/radians_per_pulse;
	int step_delay = (MS_IN_SEC / pulses_to_make_a_radian) / radians_per_second;
	return step_delay;
}

void Save_Joint_Positions_To_EEPROM() {

}

void Check_Packet(int packet_complete, unsigned char packet[]) {
	if (packet_complete) {
		if (packet[0] == 'S') {
			Save_Joint_Positions_To_EEPROM();
			packet_complete = 0;
			return;
		}
	}
	packet_complete = 0;
}