


#include <xc.h>
#include "main.h"

#include <timer.h>
//#include <p33FJ128MC804.h>

volatile uint32_t Timer_ms1 = 0;


volatile uint8_t Match_En_Cours = 0;
volatile uint8_t Match_Fini = 0;
volatile uint32_t Delay_90 = 0;

volatile long Delay_WatchDone = 0;

void Timers_Init(void) {
    // activation du Timer3
    OpenTimer3(T3_ON &
            T3_IDLE_CON &
            T3_GATE_OFF &
            T3_PS_1_64 &
            T3_SOURCE_INT, 625); // 625 pour 1ms
    // configuration des interruptions
    ConfigIntTimer3(T3_INT_PRIOR_7 & T3_INT_ON);    // super haute priorité
    //IPC2bits.T3IP = 7
    
    // on va utiliser les interruptions SPI2 (normal et erreurs)
    // pour les timers avec moins de priorité...
    IFS2bits.SPI2EIF = 0;
    IFS2bits.SPI2IF = 0;
    IEC2bits.SPI2EIE = 1;
    IEC2bits.SPI2IE = 1;
    
    IPC8bits.SPI2IP = 2;        // normal basse prio
    IPC8bits.SPI2EIP = 4;       // Erreur "haute" prio, pour l'asserv    
}

void Raz_Delay_WatchDone(void) {
    Delay_WatchDone = 0;
}

void Delay_ms(uint32_t Nb_ms)
{   
    uint32_t Objectif = Timer_ms1 + Nb_ms;
    while (Timer_ms1 <= Objectif);
}

uint32_t GetTimer_ms()
{
    return Timer_ms1;
}

// every ms  super haute priorité
void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) {
    static uint8_t Count_Asserv = 0;

    Timer_ms1 ++;
    
    if (!Count_Asserv) {
        Count_Asserv = (ASSERV_EVERY-1);
        IFS2bits.SPI2EIF = 1;               // declenche l'IT asserv
    } else {
        Count_Asserv --;
    }
    
    IFS0bits.T3IF = 0;  // Flag de ce Timer à 0;
}

// toute les ms, pas prioritaire
void __attribute__((interrupt, auto_psv)) _SPI2Interrupt(void) {
    IFS2bits.SPI2IF = 0;    // clear le flag d'ici
}

// ASSERV ICI 
void __attribute__((interrupt, auto_psv)) _SPI2ErrInterrupt(void) {
    // compteurs QEI gauche et droit
    static int tics_g, tics_d;
    // commandes gauches et droite
    static float commande_g, commande_d;
    
    

    // récupération des données des compteurs qei gauche et droit
    tics_g = (int) POS1CNT;
    tics_d = (int) POS2CNT;
    // effectuer un pas de déplacement
    motion_step(tics_g, tics_d, &commande_g, &commande_d);
    // mettre ici les pwm gauche et droit
    PWM_set_dutycycle(commande_g, commande_d);

    IFS2bits.SPI2EIF = 0;    // clear le flag d'ici
}

uint8_t Get_Status_Match(void)  // 1 si match en cours, 2 si fini, 0 si avant
{
    return Match_En_Cours + (Match_Fini*2);
}

