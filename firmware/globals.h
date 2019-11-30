#ifndef GLOBALS_H
#define GLOBALS_H

extern int micro_steps;
extern int radians_per_second_x1000;
extern int packet_complete;
extern int packet_byte_counter;
extern int J1_delay;
extern int J_delay;
extern int J1_delay_count;
extern int J_delay_count;
extern unsigned char packet[32];
extern unsigned int J1_steps;
extern unsigned int J2_steps;
extern unsigned int J3_steps;
extern unsigned int J4_steps;
extern unsigned int J5_steps;
extern unsigned int J6_steps;
extern unsigned char J1_direction;
extern unsigned char J2_direction;
extern unsigned char J3_direction;
extern unsigned char J4_direction;
extern unsigned char J5_direction;
extern unsigned char J6_direction;

#endif