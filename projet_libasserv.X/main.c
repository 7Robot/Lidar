/*
 * Template dsPIC33F
 * Compiler : Microchip xC16
 * µC : 33FJ128MC804
 * Mars 2016
 *
 *    ______ _____      _           _
 *   |___  /| ___ \    | |         | |
 *      / / | |_/ /___ | |__   ___ | |_
 *     / /  |    // _ \| '_ \ / _ \| __|
 *    / /   | |\ \ (_) | |_) | (_) | |_
 *   /_/    |_| \_\___/|____/ \___/'\__|
 *			      7robot.fr
 */



/********Headers********/
#include <stdbool.h>       // Includes true/false definition
#include <math.h>

#include "main.h"

int main(int argc, char** argv) 
{
    u8 c;
    
    Init_All();
    
    set_Constraint_vitesse_max(0.3);
    set_Constraint_vt_max(1.5);

    while (1) // boucle principale
    {
        if(Get_Uart_Cmd(&c))
        {
            Interp(c);
        }
	}
}