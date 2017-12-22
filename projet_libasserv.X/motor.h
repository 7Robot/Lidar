#ifndef MOTOR_H
#define	MOTOR_H

// Mode des pins de sens des moteurs
#define MOTOR_BREAK1_tris TRISAbits.TRISA7
#define MOTOR_DIR1_tris TRISBbits.TRISB15
#define MOTOR_BREAK2_tris TRISBbits.TRISB11
#define MOTOR_DIR2_tris TRISBbits.TRISB13   // remapage au fil !
// Valeur des pin de sens des moteurs (output)
#define MOTOR_BREAK1 LATAbits.LATA7
#define MOTOR_DIR1 LATBbits.LATB15
#define MOTOR_BREAK2 LATBbits.LATB11
#define MOTOR_DIR2 LATBbits.LATB13

// min et max de P1TPER (periode du PWM en nb de cycles)
// ici min et max permettent de rester dans des fréquences comprises entre
// 26kHz et 1.2kHz pour respecter la datasheet du pont en H
#define P1TPER_MIN 1500UL
#define P1TPER_MAX 32000UL
// Duty-Cycle max (au cas ou on veut limiter la puissance moteur)
#define DC_MAX 100


void PWM_init(void);
void QEI_init(void);
void PWM_set_dutycycle(float dc_left, float dc_right);
void PWM_set_param(float frequence, float dc_left, float dc_right);
void PWM_set_dutycycleLeft(float dc_left);
void PWM_set_dutycycleRight(float dc_right);

#endif	/* MOTOR_H */

