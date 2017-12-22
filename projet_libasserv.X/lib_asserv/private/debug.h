#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG_COUNT_MAX 500

// pour le réglage de l'asserv (tableaux à récupérer en mode debug)

typedef struct {
    // consignes en vitesse et vitesse angulaire du robot
    float v[DEBUG_COUNT_MAX];
    float vt[DEBUG_COUNT_MAX];
    float cons_v[DEBUG_COUNT_MAX];
    float cons_vt[DEBUG_COUNT_MAX];
    float pid_v[DEBUG_COUNT_MAX];
    float pid_vt[DEBUG_COUNT_MAX];
} AsservDebug;


/*#############################    Fonctions    ##############################*/

void debug_init();
void debug_asserv();

#endif // _DEBUG_H_