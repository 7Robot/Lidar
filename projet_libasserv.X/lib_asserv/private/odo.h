#ifndef _ODO_H_
#define _ODO_H_

#include "motion.h"


/*****************************    Structures    *******************************/

/**
 * \struct OdoCoefs
 * \brief Contient les coefficients permettant de passer de tics en mètres et la
 * distance entre les deux roues.
 */
typedef struct {
    int tic_by_meter;   /*!< Coefficient de passage de mètre en tic*/
    float meter_by_tic; /*!< Coefficient de passage de tic en mètre*/
    float spacing;      /*!< Distance entre les roues du robot*/
} OdoCoefs;

/**
 * \struct OdoTics
 * \brief Positions des roues en tics.
 */
typedef struct {
    int tics_g;         /*!< Position de la roue gauche en tics*/
    int tics_d;         /*!< Position de la roue droite en tics*/
} OdoTics;

/**
 * \struct Odo
 * \brief Contient les différents éléments nécessaires à l'odométrie ainsi
 * que les données de l'odométrie.
 */
typedef struct {
    OdoCoefs coefs;     /*!< Coefficients de passage et distance entre les roues*/
    OdoTics tics;       /*!< Position des roues en tics*/
    RobotState *state; /*!< Etat du robot (Pos, Vit, Acc)*/
    float period;       /*!< Période de rafraichissement*/
} Odo;

/******************************    Fonctions    *******************************/

void odo_init();
void odo_set_coefs(Odo *odo, OdoCoefs coefs);
void odo_set_ticByMeter(Odo *odo, int tic_by_meter);
void odo_set_spacing(Odo *odo, float spacing);
void odo_set_state(Odo *odo, RobotState *state);
void odo_set_period(Odo *odo, float period);
void odo_step(Odo *odo, int qei_g, int qei_d);

#endif // _ODO_H_