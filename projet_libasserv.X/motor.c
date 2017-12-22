/**
 * \file motor.c
 * \brief Bibliothèque dsPIC33F pour les moteurs
 * \author 7Robot
 */
#include <xc.h>
#include <math.h>
#include "motor.h"
#include "lib_asserv/private/tools.h"
#include "user.h"

/**
 * \fn void PWM_init(void)
 * \brief Initialise les modules PWM
 */
void PWM_init(void) {
    // P1TCON
    P1TCONbits.PTEN = 1;    // enable module PWM
    P1TCONbits.PTCKPS = 0;  // Clk input of the PWM module is TCY (no prescale)
    P1TCONbits.PTMOD = 0;   // free-runnig mode

    /*
     * La période des PWM (temps entre 2 fronts montants)
     * est fixée Ã  1500 cycles de 40MHz
     * Cela donne une periode de sortie de 37.5 µs soit 26.66 kHz
     * RMQ : les registres de rapport cycliques ayant une prÃ©cision double
     * leur valeur max sera 3000
     */
    P1TPER = 1500;

    // PWM1CON1
    PWM1CON1bits.PMOD2 = 1; // sorties indÃ©pendantes
    PWM1CON1bits.PMOD3 = 1;
    
    // Désactivation de toutes les sorties
    // à  réactiver une fois le pinout trouvé ;)

    // sorties High du pwm
    // remapage au fil, on oublie la pin 2H (seem really dead, even in GPIO)
    PWM1CON1bits.PEN1H = 1; // sortie pwm
    PWM1CON1bits.PEN2H = 1; // sortie pwm
    PWM1CON1bits.PEN3H = 0;

    // sorties Low (opposé du High, on ne l'utilise pas)
    PWM1CON1bits.PEN1L = 0;
    PWM1CON1bits.PEN2L = 0;
    PWM1CON1bits.PEN3L = 0;

    P1TPER = 1500;

    P1DC1 = 0;              // Init des rapports cyclique à 0
    P1DC2 = 0;

    MOTOR_BREAK1_tris = 0;
    MOTOR_BREAK2_tris = 0;
    MOTOR_DIR1_tris = 0;
    MOTOR_DIR2_tris = 0;
    
    // Activation en sortie des pin de sens du PONT en H
    MOTOR_BREAK1 = 0;
    MOTOR_BREAK2 = 0;
    MOTOR_DIR1 = 0;
    MOTOR_DIR2 = 0;
}

/**
 * \fn void QEI_init(void)
 * \brief Init les modules pour les roues codeuses
 */
void QEI_init(void) {
    // module QEI1 = Moteur Gauche
    QEI1CONbits.QEISIDL = 1;    // module toujours actif, meme en etat de pause du pic
    QEI1CONbits.QEIM = 6;       // module en mode x4 : regarde tous les fronts, reset sur index, dÃ©sactivÃ© en dessous
    QEI1CONbits.POSRES = 0;     // desactive l'index => pas de reset du compteur;
    QEI1CONbits.TQCS = 0;       // use PIC clock

    // configuration des pins A et B du module
    RPINR14bits.QEA1R = 24;     // 24 = pin RP24
    RPINR14bits.QEB1R = 22;

    // module QEI2 = Moteur Droit
    QEI2CONbits.QEISIDL = 1;    // module toujours actif, meme en etat de pause du pic
    QEI2CONbits.QEIM = 6;       // module en mode x4 : regarde tous les fronts, reset sur index, dÃ©sactivÃ© en dessous
    QEI2CONbits.POSRES = 0;     // desactive l'index => pas de reset du compteur;
    QEI2CONbits.TQCS = 0;       // use PIC clock

    // configuration des pins A et B du module
    RPINR16bits.QEA2R = 23;     // 23 = pin RP23
    RPINR16bits.QEB2R = 25;
}

/**
 * \fn void PWM_set_dutycycle(float dc_left, float dc_right)
 * \brief Change la valeur des rapports cycliques des PWM.
 * 
 * \param dc_left Rapport cyclique du moteur gauche [-100; 100]
 * \param dc_right Rapport cyclique du moteur droit [-100; 100]
 */
void PWM_set_dutycycle(float dc_left, float dc_right) {

    if (dc_right >= 0)
        MOTOR_DIR2 = 0;
    else
        MOTOR_DIR2 = 1;

    if (dc_left >= 0)
        MOTOR_DIR1 = 1;
    else
        MOTOR_DIR1 = 0;

    P1TPER = 1500;

    // limitation des Duty-Cycle
    dc_left = limit_float(dc_left, -DC_MAX, DC_MAX);
    dc_right = limit_float(dc_right, -DC_MAX, DC_MAX);

    // calcul des temps High des moteurs (cf datasheet)
    // RMQ : ici la précision est 2 fois plus grande que pour P1TPER
    P1DC1 = (int) (30 * fabs(dc_left));
    P1DC2 = (int) (30 * fabs(dc_right));
}

/**
 * \fn void PWM_set_param(float dc_left, float dc_right)
 * \brief Change la valeur de la fréquence de découpage et des rapports cycliques des PWM.
 * 
 * \param frequence Fréquence de découpage [1; 26,66]kHz
 * \param dc_left Rapport cyclique du moteur gauche [-100; 100]
 * \param dc_right Rapport cyclique du moteur droit [-100; 100]
 */
void PWM_set_param(float frequency, float dc_left, float dc_right) {
    // calcul du nombre de cycles pour avoir la bonne fréquence (FCY/frequence)
    P1TPER = (int) limit_int((long int) (FCY / frequency), P1TPER_MIN, P1TPER_MAX);
    
    if (dc_right >= 0)
        MOTOR_DIR2 = 0;
    else
        MOTOR_DIR2 = 1;

    if (dc_left >= 0)
        MOTOR_DIR1 = 1;
    else
        MOTOR_DIR1 = 0;
    
    // limitation des Duty-Cycle
    dc_left = limit_float(dc_left, -100, 100);
    dc_right = limit_float(dc_right, -100, 100);

    dc_left = 2 * P1TPER * fabs(dc_left) / 100.0;
    dc_right = 2 * P1TPER * fabs(dc_right) / 100.0;

    // calcul des temps High des moteurs (cf datasheet)
    // RMQ : ici la précision est 2 fois plus grande que pour P1TPER
    P1DC1 = (int) limit_int((long int) (dc_left), 0, (long int) (2 * P1TPER_MAX));
    P1DC2 = (int) limit_int((long int) (dc_right), 0, (long int) (2 * P1TPER_MAX));
}

/**
 * \fn void PWM_set_dutycycleLeft(float dc_left)
 * \brief Change la valeur du rapport cyclique du moteur gauche.
 * 
 * \param dc_left Rapport cyclique du moteur gauche [-100; 100]
 */
void PWM_set_dutycycleLeft(float dc_left) {
    if (dc_left >= 0)
        MOTOR_DIR1 = 1;
    else
        MOTOR_DIR1 = 0;

    // limitation des Duty-Cycle
    dc_left = limit_float(dc_left, -DC_MAX, DC_MAX);

    // calcul des temps High des moteurs (cf datasheet)
    // RMQ : ici la précision est 2 fois plus grande que pour P1TPER
    P1DC1 = (int) (30 * fabs(dc_left));
}

/**
 * \fn void PWM_set_dutycycleRight(float dc_right)
 * \brief Change la valeur du rapport cyclique du moteur droit.
 * 
 * \param dc_right Rapport cyclique du moteur droit [-100; 100]
 */
void PWM_set_dutycycleRight(float dc_right) {
    if (dc_right >= 0)
        MOTOR_DIR2 = 0;
    else
        MOTOR_DIR2 = 1;

    // limitation des Duty-Cycle
    dc_right = limit_float(dc_right, -DC_MAX, DC_MAX);

    // calcul des temps High des moteurs (cf datasheet)
    // RMQ : ici la précision est 2 fois plus grande que pour P1TPER
    P1DC2 = (int) (30 * fabs(dc_right));
}
