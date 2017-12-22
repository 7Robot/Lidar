
#ifndef __UART_CMD_H
#define __UART_CMD_H


//#define BAUDRATECMD 1000000
//#define BAUDRATECMD 500000
//#define BAUDRATECMD 400000
//#define BAUDRATECMD 250000
//#define BAUDRATECMD 200000
#define BAUDRATECMD 115200
//#define BAUDRATECMD 57600
//#define BAUDRATECMD 38400
//#define BAUDRATECMD 19200
//#define BAUDRATECMD 9600

#define BRGBAUDRATECMD (((FCY / BAUDRATECMD) / 16) - 1)

#define UART_CMD_SIZE_BUFF 300

void UART_Cmd_Init(void);
//void Transmit_String(char *str);
//int	puts(const char *symbol);
void Transmit_Char(u8 symbol);
u8 Is_TX_Empty(void);
void Wait_Transmited(void);

//int putchar (int c);
int write(int handle, void *buffer, unsigned int len);
// void test_transmit(void);

u8 Get_Uart_Cmd(u8 *c);




#endif
