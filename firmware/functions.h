#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void Setup(void);
void USART_Transmit_Byte(char data);
void USART_Transmit_Msg(char *data);
int Set_Joint_Speed(int radians_per_second_x1000, int joint1, int J1_gear_ratio, int j_gear_ratio, int micro_steps);
void Set_Motor_Directions(int J1, int J2, int J3, int J4, int J5, int J6);
void Check_Packet(volatile int *packet_complete, volatile unsigned char packet[]);

#endif