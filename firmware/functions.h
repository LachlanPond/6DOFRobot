#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void Setup(void);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char);
int Set_Joint_Speed(int radians_per_second_x1000, int joint1, int J1_gear_ratio, int j_gear_ratio, int micro_steps);
void Check_Packet(void);
void Save_Joint_Positions_To_EEPROM(void);
void Calibrate_Joints(void);

#endif