

#include "main.h"
#include <uart.h>

volatile u8 TX_CMD_Buff[UART_CMD_SIZE_BUFF];
volatile u16 i_TX_CMD_Buff = 0;
volatile u8 RX_CMD_Buff[UART_CMD_SIZE_BUFF];
volatile u16 i_RX_CMD_Buff = 0;


void UART_Cmd_Init(void)
{
    u8 toto;
    OpenUART1(UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW
        & UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK
        & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN
        & UART_NO_PAR_8BIT & UART_1STOPBIT,
          UART_INT_TX_BUF_EMPTY & UART_IrDA_POL_INV_ZERO
        & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_TX_BUF_NOT_FUL & UART_INT_RX_CHAR
        & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR,
          BRGVALRASPI);

    
    //Remapage uart 1
    _U1RXR = 18;
    _RP4R = 0b0011; // RP4 = U1TX (p.167)
    
    // purge buffer récepetion
    IFS0bits.U1RXIF = 0;
    toto = U1RXREG;
    toto = U1RXREG;
    toto = U1RXREG;
    toto = U1RXREG;
        
    // vu qu'on peut gerer 4 transmits à la fois, c'est plus une IT super urgente..
    ConfigIntUART1(UART_RX_INT_PR4 & UART_RX_INT_EN
                 & UART_TX_INT_PR4 & UART_TX_INT_DIS);
    
    IFS0bits.U1TXIF = 1;    // init le flag 
    
    
}


void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void)
{
    static u16 i_TX_Transmit = 0;
    // cette IT déclenche quand le buffer (FIFO de 4 places) devient vide
    // par contre, le shift register (ce qui est en train de transmètre), n'est pas forcément vide
    // on rajoute des choses dans la FIFO tant qu'elle n'est pas pleine, et tant que l'on a qqchose à envoyer
    
    IFS0bits.U1TXIF = 0;
    while ((i_TX_Transmit != i_TX_CMD_Buff) && (!U1STAbits.UTXBF)) {
        U1TXREG = TX_CMD_Buff[i_TX_Transmit];
        i_TX_Transmit++;
        if (i_TX_Transmit == UART_CMD_SIZE_BUFF)
            i_TX_Transmit = 0;
    }

    if (i_TX_Transmit == i_TX_CMD_Buff) // si on a tout transmit, on s'arrete
        IEC0bits.U1TXIE = 0;
}

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
    u16 i = i_RX_CMD_Buff;
    IFS0bits.U1RXIF = 0;
    while (U1STAbits.URXDA) {      // tant que la FIFO de réception n'est pas vide
        RX_CMD_Buff[i] = U1RXREG;
        i++;
        if (i == UART_CMD_SIZE_BUFF)
            i = 0;
    }
    i_RX_CMD_Buff = i;
}

u8 Is_TX_Empty(void)
{
    return !IEC0bits.U1TXIE;
}

void Wait_Transmited(void)
{   while(!Is_TX_Empty());  }

int write(int handle, void *buffer, unsigned int len)
{
    unsigned int i;
    u8 *buff = buffer;
 //   if (handle == 0) {
        for (i = 0; i < len; i ++) {
            Transmit_Char(*buff);
            buff ++;
        }
 //   }
    //Transmit_Char('c');
    return len;
}

void Transmit_Char(u8 symbol) {

    u8 i = i_TX_CMD_Buff;
    TX_CMD_Buff[i] = symbol;
    i++;
    if (i == UART_CMD_SIZE_BUFF) {
        i = 0;
    }
    i_TX_CMD_Buff = i;
    IEC0bits.U1TXIE = 1;
}

//void test_transmit(void)
//{
//    u8 i = i_TX_CMD_Buff;
//    printf ("%d\n", i);
//}

u8 Get_Uart_Cmd(u8 *c) {
    static u16 i_RX = 0;

    if (i_RX != i_RX_CMD_Buff) { // si il y a qq chose dans le buffer
        *c = RX_CMD_Buff[i_RX];
        i_RX++;
        if (i_RX == UART_CMD_SIZE_BUFF)
            i_RX = 0;
        return 1;
    } else {
        return 0;
    }
}



