#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void Setup(void);
void USART_Transmit_Byte(char data);
void USART_Transmit_Msg(char *data);
void Set_Motor_Directions(int J1, int J2, int J3, int J4, int J5, int J6);

#endif