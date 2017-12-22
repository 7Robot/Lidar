#include "extern_globals_asserv.h"
#include "../libasserv.h"
#include "debug.h"
#include "motion.h"
#include "asserv.h"

/******************************    Variables    *******************************/

volatile int debug_mode;
volatile int debug_count;
volatile AsservDebug asservDebug;

/******************************    Fonctions    *******************************/


void init_debug_table(volatile float *table) {
    int i;
    for (i = 0; i < DEBUG_COUNT_MAX; i++) {
        table[i] = 0;
    }
}

void debug_init() {
    debug_mode = DEFAULT_DEBUG_MODE;
    debug_count = 0;
    init_debug_table(asservDebug.v);
    init_debug_table(asservDebug.vt);
    init_debug_table(asservDebug.cons_v);
    init_debug_table(asservDebug.cons_vt);
    init_debug_table(asservDebug.pid_v);
    init_debug_table(asservDebug.pid_vt);
}

void debug_asserv() {
    if (debug_count < DEBUG_COUNT_MAX) {
        (asservDebug.v)[debug_count] = motionState.speed.v;
        (asservDebug.vt)[debug_count] = motionState.speed.vt;
        (asservDebug.cons_v)[debug_count] = speed_asserv.speed_order_constrained.v;
        (asservDebug.cons_vt)[debug_count] = speed_asserv.speed_order_constrained.vt;
        (asservDebug.pid_v)[debug_count] = pid_process((Pid*)&(speed_asserv.pid_delta));
        (asservDebug.pid_vt)[debug_count] = pid_process((Pid*)&(speed_asserv.pid_alpha));
        debug_count++;
    }
}
