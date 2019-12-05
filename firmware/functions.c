#ifndef __AVR_ATmega328P__
	#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"

void Setup() {
	cli();
	// First set the data direction for motor controlling signal pins
	DDRD |= 0b11111100;	// The last two bits of register D are for TX and RX
	DDRC |= 0b00111111;	// bit 7 is reserved/unused, bit 6 is reset
	DDRB |= 0b00000111;

	// Setup the USART serial with baud 115200, 8bit byte size, no parity, 1 stopbit
	UBRR0H = 0b00000000;		// Select the 115200 baud rate option for a 16Mhz clock (UBRR0 = 8)
	UBRR0L = 0b00001000;
	// Enable RX and TX, and the RX interrupt
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	// Frame format is set in the UCSRnC register but the defaults already give us 8bit, no parity, and 1 stopbit
	// Enable CTC (Clear Timer on Compare mode) for 8-bit timer/Counter0
	TCCR0A |= (1<<WGM01);
	// Set the Output Compare Register A to some value before the rest of the timer setup (not sure if neccessary)
	OCR0A = 255;
	// Set the prescalar for the 8-bit timer 0 to 1024, providing a nice range of speeds for the stepper motors
	TCCR0B |= (1<<CS00) | (1<<CS02);
	// Turn on the Compare Match A interrupt
	TIMSK0 |= (1<<OCIE0A);

	// Enable CTC (Clear Timer on Compare mode) for 8-bit timer/Counter2
	TCCR2A |= (1<<WGM21);
	// Set the Output Compare Register A to some value before the rest of the timer setup (not sure if neccessary)
	OCR2A = 255;
	// Set the prescalar for the 8-bit timer 2 to 1024, providing a nice range of speeds for the stepper motors
	TCCR0B |= (1<<CS20) | (1<<CS21) | (1<<CS22);
	// Turn on the Compare Match A interrupt
	TIMSK2 |= (1<<OCIE2A);

	// Enable global interrupts in order to use the RX and TX interrupts
	sei();

	// Setup data direction registers
	DDRB = 0b00111111;
	DDRC = 0b00111111;
	DDRD = 0b11111100;
}

void USART_Transmit_Byte(unsigned char data) {
	// Wait for the trasmit buffer to be empty
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data into the buffer, sending it
	UDR0 = data;
}

void USART_Transmit_Msg(unsigned char *data) {
	while(*data != '\0'){
		USART_Transmit_Byte(*data);
		data++;
	}
}

void Set_Motor_Directions(int J1, int J2, int J3, int J4, int J5, int J6) {
	if (J1) PORTC |= (1<<MTR1_DIR); else PORTC &= ~(1<<MTR1_DIR);
	if (J2) PORTC |= (1<<MTR2_DIR); else PORTC &= ~(1<<MTR2_DIR);
	if (J3) PORTD |= (1<<MTR3_DIR); else PORTD &= ~(1<<MTR3_DIR);
	if (J4) PORTD |= (1<<MTR4_DIR); else PORTD &= ~(1<<MTR4_DIR);
	if (J5) PORTD |= (1<<MTR5_DIR); else PORTD &= ~(1<<MTR5_DIR);
	if (J6) PORTD |= (1<<MTR6_DIR); else PORTD &= ~(1<<MTR6_DIR);
}