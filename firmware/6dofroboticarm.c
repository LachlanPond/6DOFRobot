#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int micro_steps = 1;
int radians_per_second_x1000 = 100;
int packet_complete = 0;
int packet_byte_counter = 0;
unsigned char packet[32];

#include "globals.h"
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