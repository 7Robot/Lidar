#ifndef _LIB_ASSERV_H_
#define _LIB_ASSERV_H_

#include "private/motion.h"
#include "private/extern_globals_asserv.h"

/*##############################   MOTION   ##################################*/

// initialiser la lib d'asservissement
void motion_init(void);

// consignes de déplacements du robot
void motion_free(); // passer en roue libre
void motion_pos(Position pos); // aller à cette position
void motion_sequence(Position pos1, Position pos2); // sequence de 2 positions
void motion_push(Position pos, float stop_distance); // ajoute ou remplace le prochain ordre de la sequence
void motion_speed(Speed speed); // avancer à cette vitesse
void motion_linear_speed(Speed speed); // avancer à cette vitesse, sans s'occuper de l'angle
void motion_angle(float abs_angle); // tourner pour être à un angle (absolu) alpha

// checker si le déplacement est terminé
int motion_done();

int Is_Asserv_Mode_Pos(void); // r�pond 1 si vraiment position (ou s�quence de position)
int Sens_Vitesse_Deplacement(void); // 1 si vitesse positive -1 si negative, 0 si moins que 3 cm/sec


// renvoie les commandes des roues gauche et droite (appelé par l'interruption toutes les 10 ms)
void motion_step(int tics_g, int tics_d, float *commande_g, float *commande_d);

/*############################   MOTION STATE   ##############################*/

// assigner des valeurs à la position (x, y et theta)
void set_position(Position pos);
void set_position_x(float x);
void set_position_y(float y);
void set_position_t(float t);


// connaître l'état du robot
Position get_position();
Speed get_speed();
Acceleration get_acceleration();


#endif // _LIB_ASSERV_H_