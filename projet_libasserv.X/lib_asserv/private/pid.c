#include "pid.h"
#include "tools.h"
#include <math.h>


/******************************    Fonctions    *******************************/

// assigner des valeurs aux coefs

void pid_set_coefs(Pid *pid, PidCoefs coefs) {
    pid->coefs = coefs;
}

void pid_set_kp(Pid *pid, float kp) {
    pid->coefs.kp = kp;
}

void pid_set_ki(Pid *pid, float ki) {
    pid->coefs.ki = ki;
}

void pid_set_kd(Pid *pid, float kd) {
    pid->coefs.kd = kd;
}

void pid_set_mu_p(Pid *pid, float mu_p) {
    pid->coefs.mu_p = mu_p;
}

// assigner des valeurs à l'état du PID

void pid_set_state(Pid *pid, PidState state) {
    pid->state = state;
}

void pid_set_err(Pid *pid, float err) {
    pid->state.err = err;
}

void pid_set_err_int(Pid *pid, float err_int) {
    pid->state.err_int = err_int;
}

void pid_set_err_der(Pid *pid, float err_der) {
    pid->state.err_der = err_der;
}

void pid_set_max_int(Pid *pid, float max_int) {
    pid->state.max_int = max_int;
}

// assigner des valeurs aux erreur et derivée permettant de considérer que le robot est arrivé

void pid_set_eps(Pid *pid, PidEps eps) {
    pid->eps = eps;
}

void pid_set_eps_err(Pid *pid, float err) {
    pid->eps.err_eps = err;
}

void pid_set_eps_der(Pid *pid, float der) {
    pid->eps.der_eps = der;
}

// remise à zero de l'intégrale du PID

void pid_reset(Pid *pid) {
    pid_set_err_int(pid, 0);
}

// assigner une valeur de consigne

void pid_set_order(Pid *pid, float order) {
    pid->order = order;
}

// maj du PID avec une valeur "value"

void pid_maj(Pid *pid, float value) {
    float err_old = pid->state.err;
    float err_moy_old = pid->state.err_moy;
    pid_maj_err(pid, value);
    pid_maj_err_der(pid, err_old, err_moy_old);
    pid_maj_err_int(pid);
}

// maj de l'erreur du PID en tenant compte du coef de moyennage mu_p

void pid_maj_err(Pid *pid, float value) {
    pid->state.err = pid->order - value;
    pid->state.err_moy = (1 - pid->coefs.mu_p) * pid->state.err_moy + pid->coefs.mu_p * pid->state.err;
}

// maj de l'intégrale de l'erreur du PID en tenant compte de sa limite max

void pid_maj_err_int(Pid *pid) {
    pid->state.err_int = limit_float(pid->state.err_int + pid->state.err, -(pid->state.max_int), pid->state.max_int);
    pid->state.err_int_moy = limit_float(pid->state.err_int_moy + pid->state.err_moy, -(pid->state.max_int), pid->state.max_int);
}

// maj de la dérivée de l'erreur du PID en tenant compte du coef de moyennage mu_d

void pid_maj_err_der(Pid *pid, float err_old, float err_moy_old) {
    pid->state.err_der = pid->state.err - err_old;
    pid->state.err_der_moy = pid->state.err_moy - err_moy_old;
}

// calcule la commande du PID

float pid_process(Pid *pid) {
    float err = pid->state.err_moy;
    float err_int = pid->state.err_int; //_moy;
    float err_der = pid->state.err_der_moy;
    return pid->coefs.kp * err
            + pid->coefs.ki * err_int
            + pid->coefs.kd * err_der;
}

// indique si le pid est stable

int pid_done(Pid *pid) {
    float err = pid->state.err; //_moy;
    float err_der = pid->state.err_der; //_moy;
    return fabsf(err) <= pid->eps.err_eps && fabsf(err_der) <= pid->eps.der_eps;
}
