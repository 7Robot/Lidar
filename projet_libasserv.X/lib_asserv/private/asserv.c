#include "extern_globals_asserv.h"
#include "asserv.h"
#include "odo.h"
#include "pid.h"
#include "../libasserv.h"
#include "debug.h"
#include "tools.h"
#include <math.h>


/******************************    Variables    *******************************/
volatile int asserv_mode;
volatile PositionAsserv pos_asserv;
volatile AngleAsserv angle_asserv;
volatile SpeedAsserv speed_asserv;

/******************************    Fonctions    *******************************/

// initialiser le mode et les differents asservissement

void asserv_init() {
    asserv_mode = DEFAULT_ASSERV_MODE;

    // PID pour l'asserv en position (delta)
    Pid pid_pos_delta;
    PidCoefs coefs_pos_delta = DEFAULT_PID_POS_COEFS_DELTA;
    PidState state_pos_delta = {0, 0, 0, 0, 0, 0, 0};
    PidEps eps_pos_delta = DEFAULT_PID_EPS_DELTA;
    pid_set_coefs(&pid_pos_delta, coefs_pos_delta);
    pid_set_state(&pid_pos_delta, state_pos_delta);
    pid_set_eps(&pid_pos_delta, eps_pos_delta);
    pid_set_order(&pid_pos_delta, 0);

    // PID pour l'asserv en position angulaire (alpha)
    Pid pid_pos_alpha;
    PidCoefs coefs_pos_alpha = DEFAULT_PID_POS_COEFS_ALPHA;
    PidState state_pos_alpha = {0, 0, 0, 0, 0, 0, 0};
    PidEps eps_pos_alpha = DEFAULT_PID_EPS_ALPHA;
    pid_set_coefs(&pid_pos_alpha, coefs_pos_alpha);
    pid_set_state(&pid_pos_alpha, state_pos_alpha);
    pid_set_eps(&pid_pos_alpha, eps_pos_alpha);
    pid_set_order(&pid_pos_alpha, 0);

    // PID pour l'asserv en vitesse (delta)
    Pid pid_delta;
    PidCoefs coefs_delta = DEFAULT_PID_COEFS_DELTA;
    PidState state_delta = {0, 0, 0, 0, 0, 0, DEFAULT_PID_MAX_INT_DELTA};
    PidEps eps_delta = DEFAULT_PID_EPS_DELTA;
    pid_set_coefs(&pid_delta, coefs_delta);
    pid_set_state(&pid_delta, state_delta);
    pid_set_eps(&pid_delta, eps_delta);
    pid_set_order(&pid_delta, 0);

    // PID pour l'asserv en vitesse angulaire (alpha)
    Pid pid_alpha;
    PidCoefs coefs_alpha = DEFAULT_PID_COEFS_ALPHA;
    PidState state_alpha = {0, 0, 0, 0, 0, 0, DEFAULT_PID_MAX_INT_ALPHA};
    PidEps eps_alpha = DEFAULT_PID_EPS_ALPHA;
    pid_set_coefs(&pid_alpha, coefs_alpha);
    pid_set_state(&pid_alpha, state_alpha);
    pid_set_eps(&pid_alpha, eps_alpha);
    pid_set_order(&pid_alpha, 0);

    // initialisation de l'asservissement en position
    pos_asserv.pos_order = (Position){0, 0, 0};
    pos_asserv.pid_delta = pid_pos_delta;
    pos_asserv.pid_alpha = pid_pos_alpha;
    pos_asserv.stop_distance = DEFAULT_STOP_DISTANCE;

    // respect des contraintes d'acceleration max avec ce coef
    pos_asserv.kp = 1.6;
    pos_asserv.state = (RobotState*) (&motionState);
    pos_asserv.constraint = (MotionConstraint*) (&motionConstraint);
    pos_asserv.done = 0;
    pos_asserv.distance = (Distance){0, 0};
    pos_asserv.old_distance = (Distance){0, 0};

    // initialisation de l'asservissement en vitesse
    speed_asserv.speed_order = (Speed){0, 0};
    speed_asserv.speed_order_constrained = (Speed){0, 0};
    speed_asserv.state = (RobotState*) (&motionState);
    speed_asserv.constraint = (MotionConstraint*) (&motionConstraint);
    speed_asserv.pid_delta = pid_delta;
    speed_asserv.pid_alpha = pid_alpha;
    speed_asserv.done = 0;
    speed_asserv.courbure = 0;

    // initialisation de l'asservissement en angle
    angle_asserv.angle_order = 0;
    angle_asserv.state = (RobotState*) (&motionState);
    angle_asserv.constraint = (MotionConstraint*) (&motionConstraint);
    angle_asserv.pid_alpha = pid_alpha;
    angle_asserv.done = 0;
}

// choisir le mode d'asservissement (desactive, en position, en vitesse)

void set_asserv_off() {
    asserv_mode = ASSERV_MODE_OFF;
}

void set_asserv_pos_mode() {
    asserv_mode = ASSERV_MODE_POS;
    pid_reset((Pid*)&(speed_asserv.pid_alpha));
    pid_reset((Pid*)&(speed_asserv.pid_delta));
}

void set_asserv_angle_mode() {
    asserv_mode = ASSERV_MODE_ANGLE;
    pid_reset((Pid*)&(speed_asserv.pid_alpha));
    pid_reset((Pid*)&(speed_asserv.pid_delta));
}

void set_asserv_speed_mode() {
    asserv_mode = ASSERV_MODE_SPEED;
    pid_reset((Pid*)&(speed_asserv.pid_alpha));
    pid_reset((Pid*)&(speed_asserv.pid_delta));
}

void set_asserv_linear_speed_mode() {
    asserv_mode = ASSERV_MODE_LINEAR_SPEED;
    pid_reset((Pid*)&(speed_asserv.pid_alpha));
    pid_reset((Pid*)&(speed_asserv.pid_delta));
}

void set_asserv_angular_speed_mode() {
    asserv_mode = ASSERV_MODE_ANGULAR_SPEED;
    pid_reset((Pid*)&(speed_asserv.pid_alpha));
    pid_reset((Pid*)&(speed_asserv.pid_delta));
}

void set_asserv_seq_mode() {
    asserv_mode = ASSERV_MODE_SEQUENCE;
}

void set_asserv_pos_segment_mode() {
    asserv_mode = ASSERV_MODE_POS_SEGMENT;
    pid_reset((Pid*)&(speed_asserv.pid_alpha));
    pid_reset((Pid*)&(speed_asserv.pid_delta));
}

// observer les contraintes aux vitesse et vitesse angulaire

void constrain_speed(
        float v, float vt,
        float *v_constrained, float *vt_constrained,
        float v_max, float vt_max,
        float a_max, float at_max, float v_vt_max) {

    // periode de l'asservissement
    float period = DEFAULT_PERIOD;

    // ancienne vitesse contrainte
    float v_c_old = *v_constrained;
    float vt_c_old = *vt_constrained;

    // contraintes liees a l'acceleration et vitesse absolues
    *v_constrained = limit_float(v, v_c_old - a_max * period, v_c_old + a_max * period);
    *v_constrained = limit_float(*v_constrained, -v_max, v_max);

    // contraintes liees a l'acceleration et vitesse angulaires
    *vt_constrained = limit_float(vt, vt_c_old - at_max * period, vt_c_old + at_max * period);
    *vt_constrained = limit_float(*vt_constrained, -vt_max, vt_max);

    if ((asserv_mode == ASSERV_MODE_POS || asserv_mode == ASSERV_MODE_SEQUENCE) && speed_asserv.courbure != 0) {
        // contraintes liees a l'acceleration absolue
        *v_constrained = limit_float(v, v_c_old - a_max * period, v_c_old + a_max * period);

        //        // contraintes liees a l'acceleration angulaire !!DANGER, A DEBUGUGER!!
        //        *v_constrained = limit_float(*v_constrained, vt_c_old - at_max * period / speed_asserv.courbure,
        //                vt_c_old + at_max * period / speed_asserv.courbure);

        // contraintes liees a la vitesse absolue
        *v_constrained = limit_float(*v_constrained, -v_max, v_max);

        // contraintes liees a la vitesse angulaire
        *v_constrained = limit_float(*v_constrained, -vt_max / fabsf(speed_asserv.courbure), vt_max / fabsf(speed_asserv.courbure));

        //        // contrainte vitesse des roues en courbe !!DANGER, A DEBUGUGER!!
        //        *v_constrained = limit_float(*v_constrained, -v_max * 2 / (2 + odo.coefs.spacing * fabsf(speed_asserv.courbure)),
        //                v_max * 2 / (2 + odo.coefs.spacing * fabsf(speed_asserv.courbure)));

        // contrainte accelération centripète
        *v_constrained = limit_float(*v_constrained, -sqrtf(v_vt_max / fabsf(speed_asserv.courbure)),
                sqrtf(v_vt_max / fabsf(speed_asserv.courbure)));

        *vt_constrained = speed_asserv.courbure * fabsf(*v_constrained);
    }
}

// contraint la consigne de vitesse avec la fonction precedente constrain_speed

void constrain_speed_order() {

    // vitesse consigne(o comme order) et consigne contrainte(oc)
    float v_o = speed_asserv.speed_order.v;
    float vt_o = speed_asserv.speed_order.vt;
    float v_oc = speed_asserv.speed_order_constrained.v;
    float vt_oc = speed_asserv.speed_order_constrained.vt;

    // contraintes de vitesse
    float v_max = motionConstraint.v_max.v;
    float vt_max = motionConstraint.v_max.vt;

    // contraintes d'acceleration
    float a_max = motionConstraint.a_max.a;
    float at_max = motionConstraint.a_max.at;
    float v_vt_max = motionConstraint.a_max.v_vt;

    // application des contraintes
    constrain_speed(v_o, vt_o, &v_oc, &vt_oc, v_max, vt_max, a_max, at_max, v_vt_max);
    speed_asserv.speed_order_constrained.v = v_oc;
    speed_asserv.speed_order_constrained.vt = vt_oc;
}

// effectue un pas d'asservissement

void asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    // choix en fonction du mode d'asservissement (off, position ou vitesse)
    switch (asserv_mode) {
            // si on est en roue libre
        case ASSERV_MODE_OFF:
            *commande_g = 0;
            *commande_d = 0;
            speed_asserv.speed_order_constrained = motionState.speed;
            break;
            // si on est en asservissement en position
        case ASSERV_MODE_POS:
            if (debug_mode) debug_asserv();
            pos_asserv_step(odo, commande_g, commande_d);
            break;
            // si on est en asservissement en angle
        case ASSERV_MODE_ANGLE:
            if (debug_mode)debug_asserv();
            angle_asserv_step(odo, commande_g, commande_d);
            break;
            // si on est en asservissement en vitesse
        case ASSERV_MODE_SPEED:
            if (debug_mode) debug_asserv();
            speed_asserv_step(odo, commande_g, commande_d);
            break;
            // si on est en asservissement en vitesse, sans regarder l'angle
        case ASSERV_MODE_LINEAR_SPEED:
            if (debug_mode) debug_asserv();
            linear_speed_asserv_step(odo, commande_g, commande_d);
            break;
            // si on est en asservissement en vitesse, sans regarder la distance
        case ASSERV_MODE_ANGULAR_SPEED:
            if (debug_mode) debug_asserv();
            angular_speed_asserv_step(odo, commande_g, commande_d);
            break;
        case ASSERV_MODE_SEQUENCE:
            seq_asserv_step(odo, commande_g, commande_d);
            break;
            // si on est en asservissement en position sans courbe
        case ASSERV_MODE_POS_SEGMENT:
            if (debug_mode) debug_asserv();
            pos_segment_asserv_step(odo, commande_g, commande_d);
            break;
    }
}

void pos_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    /*
     * On calcule les consignes de vitesse et vitesse angulaire
     * en fonction de la position actuelle et de la consigne de position.
     *
     * Idees :
     * On impose un rayon de courbure à la trajectoire en fonction de la
     * distance au point et de l'angle courant. Les contraintes sont appliquées
     * dans contrain_speed() pour que tout se passe bien.
     */

    // recuperation de la consigne (o pour "order") en position
    float x_o = pos_asserv.pos_order.x; // consigne en x
    float y_o = pos_asserv.pos_order.y; // consigne en y

    // recuperation de la position et vitesse courantes
    float x = odo->state->pos.x;
    float y = odo->state->pos.y;
    float v = odo->state->speed.v;

    float vt_max = motionConstraint.v_max.vt;

    // calcul de la distance a la consigne en position
    float d = sqrtf((x_o - x)*(x_o - x) + (y_o - y)*(y_o - y));
    // calcul de la deviation angulaire par rapport a la consigne en position
    float dt = principal_angle(atan2f(y_o - y, x_o - x) - odo->state->pos.t);

    // recuperation de la position precedente
    float old_d = pos_asserv.distance.d;
    float old_dt = pos_asserv.distance.dt;

    // declaration des consignes en vitesse, vitesse angulaire et courbure
    float v_o, vt_o, courbure;

    // hysteresis pour eviter les allers retours
    float epsi = 0.1745; // 10deg
    int derriere = 0;

    // maj de la distance à la consigne
    pos_asserv.old_distance.d = old_d;
    pos_asserv.old_distance.dt = old_dt;
    pos_asserv.distance.d = d;
    pos_asserv.distance.dt = dt;

    // si on est arrive on ne bouge plus
    if (d < DEFAULT_STOP_DISTANCE && old_d < DEFAULT_STOP_DISTANCE) {
        pos_asserv.done = 1;
        commande_g = 0;
        commande_d = 0;
    } else {
        // On determine dans un premier temps si le robot doit plutot avancer
        // ou reculer pour atteindre la consigne de position. Pour cela, il suffit
        // de regarder la valeur de l'ecart angulaire "dt" a la consigne:
        //     - si |dt| < pi/2 , l'objectif est en face de nous.
        //     - si |dt| > pi/2 , il est derriere.
        // On ajoute en plus un hysteresis pour eviter les allers retours
        // quand l'objectif est quasiment a angle droit sur le cote (|dt| proche de PI/2)

        if (v > -0.001) {
            derriere = fabsf(dt) > PI / 2 + epsi;
        } else {
            derriere = fabsf(dt) > PI / 2 - epsi;
        }

        if (derriere) {
            d = -d;
            dt = principal_angle(dt + PI);
        }

        // maj des consignes du PID
        pid_set_order((Pid*)&(pos_asserv.pid_delta), d);

        // maj des valeurs du PID
        pid_maj((Pid*)&(pos_asserv.pid_delta), 0);

        // calcul des sorties du PID
        v_o = pid_process((Pid*)&(pos_asserv.pid_delta));

        // calcul du rayon de courbure de la trajectoire (la moitié de la distance au point)
        courbure = 2 * (2 * sin(dt / 2) / fabsf(d));

        // calcul de la vitesse angulaire en fonction de la vitesse linéaire et de la courbure
        vt_o = courbure * fabsf(v_o);

        // appel de l'asserve en vitesse avec les bonnes consignes
        speed_asserv.speed_order.v = v_o;
        speed_asserv.speed_order.vt = vt_o;
        speed_asserv.courbure = courbure;

        // limite vt pour stabilité si proche du point (10 cm))
        if (fabsf(d) < 0.1) motionConstraint.v_max.vt = 1.57;

        speed_asserv_step(odo, commande_g, commande_d);

        if (fabsf(d) < 0.1) motionConstraint.v_max.vt = vt_max;
    }
}

void pos_segment_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    /*
     * On calcule les consignes de vitesse et vitesse angulaire
     * en fonction de la position actuelle et de la consigne de position.
     *
     * Idees :
     * On se déplace sans courbe en tournant d'abord vers le point demandé avant
     * de commencer à avancer. !!! NE PAS UTILISER AVEC MOTION SEQUENCE !!!
     */

    // recuperation de la consigne (o pour "order") en position
    float x_o = pos_asserv.pos_order.x; // consigne en x
    float y_o = pos_asserv.pos_order.y; // consigne en y

    // recuperation de la position et vitesse courantes
    float x = odo->state->pos.x;
    float y = odo->state->pos.y;
    float v = odo->state->speed.v;

    // calcul de la distance a la consigne en position
    float d = sqrtf((x_o - x)*(x_o - x) + (y_o - y)*(y_o - y));
    // calcul de la deviation angulaire par rapport a la consigne en position
    float dt = principal_angle(atan2f(y_o - y, x_o - x) - odo->state->pos.t);

    // recuperation de la position precedente
    float old_d = pos_asserv.distance.d;
    float old_dt = pos_asserv.distance.dt;

    // declaration des consignes en vitesse, vitesse angulaire
    float v_o, vt_o;

    // hysteresis pour eviter les allers retours
    float epsi = 0.1745; // 10deg
    int derriere = 0;

    // maj de la distance à la consigne
    pos_asserv.old_distance.d = old_d;
    pos_asserv.old_distance.dt = old_dt;
    pos_asserv.distance.d = d;
    pos_asserv.distance.dt = dt;

    // si on est arrive on ne bouge plus
    if (d < DEFAULT_STOP_DISTANCE && old_d < DEFAULT_STOP_DISTANCE) {
        pos_asserv.done = 1;
        commande_g = 0;
        commande_d = 0;
    } else {
        // On determine dans un premier temps si le robot doit plutot avancer
        // ou reculer pour atteindre la consigne de position. Pour cela, il suffit
        // de regarder la valeur de l'ecart angulaire "dt" a la consigne:
        //     - si |dt| < pi/2 , l'objectif est en face de nous.
        //     - si |dt| > pi/2 , il est derriere.
        // On ajoute en plus un hysteresis pour eviter les allers retours
        // quand l'objectif est quasiment a angle droit sur le cote (|dt| proche de PI/2)

        if (v > -0.001) {
            derriere = fabsf(dt) > PI / 2 + epsi;
        } else {
            derriere = fabsf(dt) > PI / 2 - epsi;
        }

        if (derriere) {
            d = -d;
            dt = principal_angle(dt + PI);
        }

        // maj des consignes du PID
        pid_set_order((Pid*)&(pos_asserv.pid_delta), d);
        pid_set_order((Pid*)&(pos_asserv.pid_alpha), dt);

        // maj des valeurs du PID
        pid_maj((Pid*)&(pos_asserv.pid_delta), 0);
        pid_maj((Pid*)&(pos_asserv.pid_alpha), 0);

        // calcul des sorties du PID
        v_o = pid_process((Pid*)&(pos_asserv.pid_delta));
        vt_o = pid_process((Pid*)&(pos_asserv.pid_alpha));

        /* appel de l'asserve en vitesse avec les bonnes consignes en fonction
         * de l'écart d'angle
         */
        if (fabsf(dt) > 0.08727) { // +-5deg
            speed_asserv.speed_order.v = 0;
            speed_asserv.speed_order.vt = vt_o;
        } else {
            speed_asserv.speed_order.v = v_o;
            speed_asserv.speed_order.vt = vt_o;
        }

        speed_asserv_step(odo, commande_g, commande_d);
    }
}

// asservissement en angle

void angle_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    // angle restant a parcourir
    float dt = principal_angle(angle_asserv.angle_order - odo->state->pos.t);
    float old_dt = pos_asserv.distance.dt;
    float vt_o;

    pos_asserv.old_distance.dt = old_dt;
    pos_asserv.distance.dt = dt;

    // si on est arrive on ne bouge plus
    if (fabsf(dt) < DEFAULT_STOP_ANGLE && fabsf(old_dt) < DEFAULT_STOP_ANGLE) {
        angle_asserv.done = 1;
        commande_g = 0;
        commande_d = 0;
    } else {
        // maj des consignes du PID
        pid_set_order((Pid*)&(pos_asserv.pid_alpha), dt);

        // maj des valeurs du PID
        pid_maj((Pid*)&(pos_asserv.pid_alpha), 0);

        // calcul des sorties du PID
        vt_o = pid_process((Pid*)&(pos_asserv.pid_alpha));

        // appel de l'asserve en vitesse avec les bonnes consignes
        speed_asserv.speed_order.v = 0;
        speed_asserv.speed_order.vt = vt_o;
        speed_asserv_step(odo, commande_g, commande_d);
    }
}

void speed_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    // commandes des PID en vitesse absolue (delta) et angulaire (alpha)
    float commande_delta, commande_alpha;

    // verifier qu'on est pas bloque par un obstacle
    check_blocked(motionState.speed, speed_asserv.speed_order_constrained);

    // on commence par verifier les contraintes de vitesses et acceleration
    constrain_speed_order();

    // maj des consignes des PID
    pid_set_order((Pid*)&(speed_asserv.pid_delta), speed_asserv.speed_order_constrained.v);
    pid_set_order((Pid*)&(speed_asserv.pid_alpha), speed_asserv.speed_order_constrained.vt);

    // maj des valeurs des PID
    pid_maj((Pid*)&(speed_asserv.pid_delta), odo->state->speed.v);
    pid_maj((Pid*)&(speed_asserv.pid_alpha), odo->state->speed.vt);

    // calcul des sorties des PID
    commande_delta = pid_process((Pid*)&(speed_asserv.pid_delta));
    commande_alpha = pid_process((Pid*)&(speed_asserv.pid_alpha));

    // renvoie des commandes gauche et droite
    *commande_g = commande_delta - commande_alpha;
    *commande_d = commande_delta + commande_alpha;
}

void linear_speed_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    // commandes des PID en vitesse absolue (delta)
    float commande_delta;

    // verifier qu'on est pas bloque par un obstacle
    check_blocked(motionState.speed, speed_asserv.speed_order_constrained);

    // on commence par verifier les contraintes de vitesses et acceleration
    constrain_speed_order();

    // maj des consignes du PID
    pid_set_order((Pid*)&(speed_asserv.pid_delta), speed_asserv.speed_order_constrained.v);

    // maj des valeurs du PID
    pid_maj((Pid*)&(speed_asserv.pid_delta), odo->state->speed.v);

    // calcul des sorties du PID
    commande_delta = pid_process((Pid*)&(speed_asserv.pid_delta));

    // renvoie des commandes gauche et droite
    *commande_g = commande_delta;
    *commande_d = commande_delta;
}

void angular_speed_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    // commandes des PID en vitesse absolue (delta) et angulaire (alpha)
    float commande_alpha;

    // verifier qu'on est pas bloque par un obstacle
    check_blocked(motionState.speed, speed_asserv.speed_order_constrained);

    // on commence par verifier les contraintes de vitesses et acceleration
    constrain_speed_order();

    // maj des consignes du PID
    pid_set_order((Pid*)&(speed_asserv.pid_alpha), speed_asserv.speed_order_constrained.vt);

    // maj des valeurs du PID
    pid_maj((Pid*)&(speed_asserv.pid_alpha), odo->state->speed.vt);

    // calcul des sorties du PID
    commande_alpha = pid_process((Pid*)&(speed_asserv.pid_alpha));

    // renvoie des commandes gauche et droite
    *commande_g = -commande_alpha;
    *commande_d = commande_alpha;
}

// asservissement en sequence

void seq_asserv_step(Odo *odo, float *commande_g, float *commande_d) {
    // si on est arrive on ne bouge plus
    if (!(motionSequence.waiting)) {
        *commande_g = 0;
        *commande_d = 0;
    } else {
        // choix de la position en cours
        pos_asserv.pos_order = motionSequence.pos_seq[motionSequence.in_progress];
        pos_asserv.stop_distance = motionSequence.stop_distance[motionSequence.in_progress];
        pos_asserv_step(odo, commande_g, commande_d);

        // si cette etape est finie, passer a la suivante
        if (pos_asserv.done) {
            pid_reset((Pid*)&(speed_asserv.pid_alpha));
            pid_reset((Pid*)&(speed_asserv.pid_delta));
            pos_asserv.done = 0;
            motionSequence.waiting--;
            motionSequence.in_progress = !(motionSequence.in_progress);
        }
    }
}

// indique si l'asservissement en cours a termine

int asserv_done() {
    if (asserv_mode == ASSERV_MODE_OFF) {
        return 0;
    } else if (asserv_mode == ASSERV_MODE_POS) {
        return pos_asserv.done;
    } else if (asserv_mode == ASSERV_MODE_POS_SEGMENT) {
        return pos_asserv.done;
    } else if (asserv_mode == ASSERV_MODE_ANGLE) {
        return angle_asserv.done;
    } else if (asserv_mode == ASSERV_MODE_SPEED) {
        return speed_asserv.done;
    } else if (asserv_mode == ASSERV_MODE_LINEAR_SPEED) {
        return speed_asserv.done;
    } else if (asserv_mode == ASSERV_MODE_ANGULAR_SPEED) {
        return speed_asserv.done;
    } else if (asserv_mode == ASSERV_MODE_SEQUENCE) {
        return !(motionSequence.waiting);
    } else {
        return -1;
    }
}

int Is_Asserv_Mode_Pos(void) {
    if ((asserv_mode == ASSERV_MODE_POS) || (asserv_mode == ASSERV_MODE_SEQUENCE))
        return 1;
    else
        return 0;
}

int Sens_Vitesse_Deplacement(void) {
    float valf = speed_asserv.speed_order.v;
    if (valf > 0.01)
        return 1;
    else if (valf < -0.01)
        return -1;
    else
        return 0;
}
