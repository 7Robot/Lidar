
#include <xc.h>
#include "main.h"

uint8_t Detect_10V = 0;

void Gestion_IO_AU_Config_Init(void) {
    TRIS_DETECT_10V = 1;
    TRIS_TEAM = 1; // input for bouton vert/violet(rouge)
    TRIS_LAISSE = 1; // input for laisse


    TRIS_CONF1 = 1;
    TRIS_CONF2 = 1;
    PIN_TURBINE = 0;
    TRIS_TURBINE = 0;

    PULL_UP_CONF1 = 1;
    PULL_UP_CONF2 = 1;
    
    Detect_10V = PIN_DETECT_10V;
}

void Gestion_IO_AU_Config_Loop(void) {
    if (PIN_DETECT_10V) {
        if (!Detect_10V) {
            motion_free();
            Delay_ms(500);
            Lauch_Init_AX12_Cmd();
        }
        Detect_10V = 1;
    } else {
        if (Detect_10V) {
            motion_free();
            PIN_TURBINE = 0;
        }
        Detect_10V = 0;
    }
}

uint8_t Get_Number_Config(void) {
    uint8_t val = 0;

    if (!PIN_CONF1)
        val += 1;
    if (!PIN_CONF2)
        val += 2;

    return val;
}