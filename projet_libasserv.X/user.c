#include <xc.h>



#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <timer.h>

//#include "tests.h"
//#include "atp-asserv.h"
//#include "atp.h"

#include "main.h"

/******************************************************************************/
/* User Functions                                                             */

/******************************************************************************/

void ConfigureOscillator(void) {
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBDbits.PLLDIV = 150; // M=152
    CLKDIVbits.PLLPRE = 5; // N1=7
    CLKDIVbits.PLLPOST = 0; // N2=2
    /* Fosc = M/(N1.N2)*Fin
     * Fin : 7.37MHz (quartz interne)
     * Fosc à 80 MHZ (ne doit pas dépasser 80 MHZ)
     * la solution la plus proche est 152*7.37/(7*2) = 80.017
     * attention, cela entraine donc une FCY et une FPériphériques à 40 MHZ
     */
    while (!OSCCONbits.LOCK); // attente que la PLL soit lockée sur se nouvelle configuration.
}


// initialize all things

void Init_All(void) {
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    
    _NSTDIS = 0; // activation de la priorit� des interruptions
    AD1PCFGL = 0xFFFF; //Pins analogiques en num�rique pour que ATP marche !!
    
    Timers_Init();
    UART_Cmd_Init();
    
    motion_free();
    
    Position Pos0 = {0, 0, 0};
    set_position(Pos0);
    Delay_ms(10);
    PWM_init();
    QEI_init();
    Delay_ms(10);
    motion_init(); // start asserv
    Delay_ms(10);
    set_position(Pos0);

    TRISAbits.TRISA9 = 1;
}
