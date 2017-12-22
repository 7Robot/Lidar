#include "extern_globals_asserv.h"
#include "motion.h"
#include "asserv.h"
#include "odo.h"
#include "debug.h"
#include "uart.h"               //\TODO � virer
#include "../libasserv.h"
#include <math.h>
#include "../../Evitement.h"    //\TODO � virer

/******************************    Variables    *******************************/
volatile float motion_initialized = 0;
volatile RobotState motionState;
volatile MotionConstraint motionConstraint;
volatile int blocked; // compteur qui incremente quand on est bloqué par quelquechose

volatile PositionOrder lastPosOrder;
volatile MotionSequence motionSequence;


/******************************    Fonctions    *******************************/

// initialiser la lib d'asservissement
/**
 * \fn void motion_init(void)
 * \brief Initalise le module motion
 */
void motion_init(void) {
    Position posZero = {0, 0, 0};

    reset_last_order(posZero);
    Speed v_max = DEFAULT_CONSTRAINT_V_MAX;
    Acceleration a_max = DEFAULT_CONSTRAINT_A_MAX;
    blocked = 0;
    // initialiser les contraintes avant le reste (utile par exemple dans l'initialisation de l'asserve)
    motionConstraint.v_max = v_max;
    motionConstraint.a_max = a_max;

    // initialiser la séquence de déplacement
    motionSequence.in_progress = 0;
    motionSequence.waiting = 0;
    motionSequence.pos_seq[0] = posZero;
    motionSequence.pos_seq[1] = posZero;
    motionSequence.stop_distance[0] = DEFAULT_STOP_DISTANCE;
    motionSequence.stop_distance[1] = DEFAULT_STOP_DISTANCE;
    
    //\TODO � modifier.
    odo_init();
    asserv_init();
    debug_init();
    motion_initialized = 1;
}

void reset_last_order(Position pos) {
    lastPosOrder.mode = NO_ORDER;
    lastPosOrder.pos = pos;
    lastPosOrder.stop_distance = 0;
}
// assigner des valeurs à la position (x, y et theta)

void set_position(Position pos) {
    motionState.pos = pos;
}

void set_position_x(float x) {
    motionState.pos.x = x;
}

void set_position_y(float y) {
    motionState.pos.y = y;
}

void set_position_t(float t) {
    motionState.pos.t = t;
}

// ajout pepino

void set_Constraint_vitesse_max(float vl_max) {
    if (vl_max != 0) {
        motionConstraint.v_max.v = vl_max;
    } else {
        float tab_default[] = DEFAULT_CONSTRAINT_V_MAX;
        motionConstraint.v_max.v = tab_default[0];
    }
}

void set_Constraint_vt_max(float vt_max) {
    if (vt_max != 0) {
        motionConstraint.v_max.vt = vt_max;
    } else {
        float tab_default[] = DEFAULT_CONSTRAINT_V_MAX;
        motionConstraint.v_max.vt = tab_default[1];
    }
}

// ajout daniel

void set_Constraint_acceleration_max(float al_max, float at_max, float a_max) {
    float tab_default[] = DEFAULT_CONSTRAINT_A_MAX;

    if (al_max != 0) {
        motionConstraint.a_max.a = al_max;
    } else {
        motionConstraint.a_max.a = tab_default[0];
    }
    if (at_max != 0) {
        motionConstraint.a_max.at = at_max;
    } else {
        motionConstraint.a_max.at = tab_default[1];
    }
    if (a_max != 0) {
        motionConstraint.a_max.v_vt = a_max;
    } else {
        motionConstraint.a_max.v_vt = tab_default[2];
    }
}

// assigner des valeurs à la vitesse (vitesse et vitesse angulaire)

void set_speed(RobotState *state, Speed speed) {
    state->speed = speed;
}

void set_speed_v(RobotState *state, float v) {
    state->speed.v = v;
}

void set_speed_vt(RobotState *state, float vt) {
    state->speed.vt = vt;
}

// assigner des valeurs à l'accélération

void set_acceleration(RobotState *state, Acceleration acc) {
    state->acc = acc;
}

void set_acceleration_a(RobotState *state, float a) {
    state->acc.a = a;
}

void set_acceleration_at(RobotState *state, float at) {
    state->acc.at = at;
}

void set_acceleration_v_vt(RobotState *state, float v_vt) {
    state->acc.v_vt = v_vt;
}

// connaître l'état du robot

Position get_position() {
    return motionState.pos;
}

Speed get_speed() {
    return motionState.speed;
}

Acceleration get_acceleration() {
    return motionState.acc;
}

// obtenir les vitesses des roues gauche et droite

float get_vg() {
    float delta_v = odo.coefs.spacing * 0.5 * motionState.speed.vt;
    return motionState.speed.v - delta_v;
}

float get_vd() {
    float delta_v = odo.coefs.spacing * 0.5 * motionState.speed.vt;
    return motionState.speed.v + delta_v;
}

// consignes de déplacements du robot

void motion_free() {
    set_asserv_off();
}

void motion_pos(Position pos) {
    pos_asserv.stop_distance = DEFAULT_STOP_DISTANCE;
    pos_asserv.done = 0;
    pos_asserv.pos_order = pos;

    lastPosOrder.mode = POSITION_ORDER;
    lastPosOrder.pos = pos;
    lastPosOrder.stop_distance = DEFAULT_STOP_DISTANCE;
    //New_Order_Evitement_Handling();

    set_asserv_pos_mode();
}

void motion_pos_segment(Position pos) {
    pos_asserv.stop_distance = DEFAULT_STOP_DISTANCE;
    pos_asserv.done = 0;
    pos_asserv.pos_order = pos;

    lastPosOrder.mode = POSITION_ORDER;
    lastPosOrder.pos = pos;
    lastPosOrder.stop_distance = DEFAULT_STOP_DISTANCE;
    //New_Order_Evitement_Handling();

    set_asserv_pos_segment_mode();
}

// tourner pour être à un angle (absolu) alpha

void motion_angle(float abs_angle) {
    angle_asserv.done = 0;
    angle_asserv.angle_order = abs_angle;

    lastPosOrder.mode = NO_ORDER;
    //New_Order_Evitement_Handling();

    set_asserv_angle_mode();
}

void motion_speed(Speed speed) {
    speed_asserv.done = 0;
    speed_asserv.speed_order = speed;

    lastPosOrder.mode = NO_ORDER;
    //New_Order_Evitement_Handling();

    set_asserv_speed_mode();
}

void motion_linear_speed(Speed speed) {
    speed_asserv.done = 0;
    speed_asserv.speed_order = speed;

    lastPosOrder.mode = NO_ORDER;
    //New_Order_Evitement_Handling();

    set_asserv_linear_speed_mode();
}

void motion_angular_speed(Speed speed) {
    speed_asserv.done = 0;
    speed_asserv.speed_order = speed;

    lastPosOrder.mode = NO_ORDER;
    //New_Order_Evitement_Handling();

    set_asserv_angular_speed_mode();
}

void motion_sequence(Position pos1, Position pos2) {
    pos_asserv.done = 0;
    motionSequence.in_progress = 0;
    motionSequence.waiting = 2; // 2 positions en attente
    motionSequence.pos_seq[0] = pos1;
    motionSequence.pos_seq[1] = pos2;

    set_asserv_seq_mode();
}

void motion_push(Position pos, float stop_distance) {
    // si on a pas d'ordre en attente on ajoute cet ordre dans l'ordre en cours
    if (stop_distance < DEFAULT_STOP_DISTANCE) {
        stop_distance = DEFAULT_STOP_DISTANCE;
    }
    if (!motionSequence.waiting) {
        pos_asserv.done = 0;
        motionSequence.stop_distance[motionSequence.in_progress] = stop_distance;
        motionSequence.pos_seq[motionSequence.in_progress] = pos;
        motionSequence.waiting = 1;

        lastPosOrder.mode = POSITION_ORDER;
        lastPosOrder.pos = pos;
        lastPosOrder.stop_distance = stop_distance;
        //New_Order_Evitement_Handling();
        // sinon on remplace l'ordre suivant par celui là
    } else {
        motionSequence.stop_distance[!motionSequence.in_progress] = stop_distance;
        motionSequence.pos_seq[!motionSequence.in_progress] = pos;
        motionSequence.waiting++;
        if (motionSequence.waiting > 2) {
            motionSequence.waiting = 2;
        }
    }
    set_asserv_seq_mode();
}

// checker si le déplacement est terminé

int Get_Motion_Done() {
    return asserv_done();
}

// v�rifier qu'on est pas bloqu� par un obstacle
// si bloqu�, annule la consigne de vitesse

void check_blocked(Speed speed, Speed order) {
    if (fabsf(speed.v - order.v) > 0.1 || fabsf(speed.vt - order.vt) > 0.4) {
        if (blocked >= BLOCK_LIMIT) {
            motion_free();
        }
        blocked++;
    } else {
        blocked = 0;
    }
}

void load_last_order(void) {
    if (lastPosOrder.mode != NO_ORDER) {
        motion_push(lastPosOrder.pos, lastPosOrder.stop_distance);
    }
}

// renvoie les commandes des roues gauche et droite (appelé par l'interruption toutes les 5 ms)

void motion_step(int tics_g, int tics_d, float *commande_g, float *commande_d) {
    // int needToSendDone = 0;
    if (!motion_initialized) {
        *commande_g = 0;
        *commande_d = 0;
    } else {
        // maj de l'odométrie
        odo_step((Odo*) (&odo), tics_g, tics_d);
        // on appelle la bonne fonction d'asservissement
        asserv_step((Odo*) (&odo), commande_g, commande_d);
    }
}
