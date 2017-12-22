
#include <math.h>
#include "extern_globals_asserv.h"
#include "odo.h"
#include "../libasserv.h"
#include "tools.h"

/******************************    Variables    *******************************/
volatile Odo odo;

/******************************    Fonctions    *******************************/

/**
 * \fn void odo_init(void)
 * \brief Initialise le module pour l'odométrie.
 */
void odo_init(void) {
    OdoCoefs coefs = DEFAULT_ODO_COEFS;
    OdoTics tics = {0, 0};
    odo_set_coefs((Odo*) (&odo), coefs);
    odo.tics = tics;
    odo_set_state((Odo*) (&odo), (RobotState*) (&motionState));
    odo_set_period((Odo*) (&odo), DEFAULT_PERIOD);
}

/**
 * \fn void odo_set_coefs(Odo *odo, OdoCoefs coefs)
 * \brief Assigne les coefficients permettant de passer de tics en mètre ainsi
 * que la distance entre les roues.
 * 
 * \param odo Pointeur sur la structure d'odométrie à modifier.
 * \param coefs Nouveaux coefficients.
 */
void odo_set_coefs(Odo *odo, OdoCoefs coefs) {
    odo->coefs = coefs;
}

/**
 * \fn void odo_set_tickByMeter(Odo *odo, int ticByMeter)
 * \brief Assigne les coefficients permettant de passer de tics en mètre.
 * 
 * @param odo Pointeur sur la structure d'odométrie à modifier.
 * @param ticByMeter Nouveau coefficient
 */
void odo_set_ticByMeter(Odo *odo, int ticByMeter) {
    odo->coefs.tic_by_meter = ticByMeter;
    odo->coefs.meter_by_tic = 1 / ((float) ticByMeter);
}

/**
 * \fn void odo_set_spacing(Odo *odo, float spacing)
 * \brief Assigne la distance entre les roues
 * 
 * \param odo Pointeur sur la structure d'odométrie à modifier.
 * \param spacing Nouvelle distance entre les roues.
 */
void odo_set_spacing(Odo *odo, float spacing) {
    odo->coefs.spacing = spacing;
}

// lier l'odomÃ©trie Ã  un Ã©tat du robot (positions, vitesses, ...)
void odo_set_state(Odo *odo, RobotState *state) {
    odo->state = state;
}

/**
 * \fn void odo_set_period(Odo *odo, float period)
 * \brief Change la période de rafraichissementde l'odométrie.
 * 
 * \param odo Pointeur sur la structure d'odométrie à modifier.
 * \param period Nouvelle période de rafraichissement
 */
void odo_set_period(Odo *odo, float period) {
    odo->period = period;
}

/**
 * \fn void odo_step(Odo *odo, int qei_g, int qei_d)
 * \brief Réalise un rafraichissement de l'odométrie.
 * 
 * \param odo Pointeur sur la structure d'odométrie à modifier.
 * \param qei_g Position du moteur gauche en tics.
 * \param qei_d Position du moteur droit en tics.
 */
void odo_step(Odo *odo, int qei_g, int qei_d) {
    // calculs intermédiaires des déplacements gauches et droites
    float dg = (float) (qei_g - (odo->tics).tics_g)*(odo->coefs).meter_by_tic;
    float dd = (float) (qei_d - (odo->tics).tics_d)*(odo->coefs).meter_by_tic;

    // distance et angle de dÃ©placement depuis le dernier step
    float d = (dd + dg) / 2;
    float dt = atan2f((dd - dg), (odo->coefs).spacing);

    // sauvegarde des anciennes vitesses
    float v = odo->state->speed.v;
    float vt = odo->state->speed.vt;

    // maj des tics
    odo->tics.tics_g = qei_g;
    odo->tics.tics_d = qei_d;

    // maj des positions
    odo->state->pos.x += d * cos(odo->state->pos.t);
    odo->state->pos.y += d * sin(odo->state->pos.t);
    odo->state->pos.t += dt;
    odo->state->pos.t = principal_angle(odo->state->pos.t);

    // maj des vitesses
    odo->state->speed.v = d / (odo->period);
    odo->state->speed.vt = dt / (odo->period);

    // maj des accÃ©lÃ©rations
    odo->state->acc.a = odo->state->speed.v - v;
    odo->state->acc.at = odo->state->speed.vt - vt;
    odo->state->acc.v_vt = (odo->state->speed.v)*(odo->state->speed.vt);
}