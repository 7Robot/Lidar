#ifndef _MOTION_H_
#define _MOTION_H_

#define NO_ORDER        0
#define POSITION_ORDER  1
#define ANGLE_ORDER     2

#include "../libasserv.h"
#include "odo.h"

/*****************************    Structures    *******************************/

/**
 * \struct MotionConstraint
 * \brief Contraintes de vitesse et d'acc�l�ration du robot.
 */
typedef struct {
    Speed v_max;            /*!< Vitesse max du robot*/
    Acceleration a_max;     /*!< Acc�l�ration max du robot*/
} MotionConstraint;

/**
 * \struct MotionSequence 
 * \brief Suite de deux s�quences de d�placement
 */
typedef struct {
    Position pos_seq[2];    /*!< Positions que doit prendre le robot*/
    float stop_distance[2]; /*!< Je en sais pas*/
    int in_progress;        /*!< Status de l'action en cours (0 ou 1)*/
    int waiting;            /*!< 0, 1 ou 2 si un ordre est en attentes*/
} MotionSequence;

// Sauvegarde du dernier ordre demande par la PI.
/**
 * \struct PositionOrder
 * \brief Sauvegarde du dernier ordre demand� par la PI
 */
typedef struct {
    int mode;               /*!< Mode de fonctionnement*/
    Position pos;           /*!< Position d�sir�e*/
    float stop_distance;    /*!< Distance d'arret dans le cas d'un motion push*/
} PositionOrder;
/******************************    Fonctions    *******************************/

// initialiser la lib d'asservissement
void motion_init();
void reset_last_order();

// assigner des valeurs à la position (x, y et theta)
void set_position(Position pos);
void set_position_x(float x);
void set_position_y(float y);
void set_position_t(float t);

// ajout pepino
void set_Constraint_vitesse_max(float vl_max);

// ajout Daniel
void set_Constraint_vt_max(float vt_max);
void set_Constraint_acceleration_max(float al_max, float at_max, float a_max);

// assigner des valeurs à la vitesse (vitesse et vitesse angulaire)
void set_speed(RobotState *state, Speed speed);
void set_speed_v(RobotState *state, float v); // vitesse
void set_speed_vt(RobotState *state, float vt); // vitesse angulaire

// assigner des valeurs à l'accélération
void set_acceleration(RobotState *state, Acceleration acc);
void set_acceleration_a(RobotState *state, float a); // accélération
void set_acceleration_at(RobotState *state, float at); // accélération angulaire
void set_acceleration_v_vt(RobotState *state, float v_vt); // vitesse * vitesse angulaire

// connaître l'état du robot
Position get_position();
Speed get_speed();
Acceleration get_acceleration();

// obtenir les vitesses des roues gauche et droite
float get_vg();
float get_vd();

// consignes de déplacements du robot
void motion_free(); // passer en roue libre
void motion_pos(Position pos); // aller à cette position
void motion_pos_segment(Position pos); // aller à cette position sans courbe
void motion_angle(float abs_angle); // tourner pour être à un angle (absolu) alpha
void motion_speed(Speed speed); // avancer à cette vitesse
void motion_linear_speed(Speed speed); // avancer à cette vitesse, sans s'occuper de l'angle
void motion_angular_speed(Speed speed); // avancer à cette vitesse, sans s'occuper de la distance
void motion_sequence(Position pos1, Position pos2); // sequence de 2 positions
void motion_push(Position pos, float stop_distance); // ajoute ou remplace le prochain ordre de la sequence


// checker si le déplacement est terminé
int Get_Motion_Done();

// vérifier qu'on est pas bloqué par un obstacle
void check_blocked(Speed speed, Speed order);

// Deblocage apres release de sick/ultrason/evitement
void load_last_order(void);

// renvoie les commandes des roues gauche et droite (appelé par l'interruption toutes les 5 ms)
void motion_step(int tics_g, int tics_d, float *commande_g, float *commande_d);

#endif // _MOTION_H_