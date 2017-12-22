#ifndef MAIN_H
#define	MAIN_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <xc.h>
#include <libpic30.h>
#include "qei.h"           // QEI definitions for easier use
//#include "uart.h"

#include "user.h"          // User funct/params, such as InitApp
#include "lib_asserv/libasserv.h"
#include "lib_asserv/lib_asserv.h"
#include "motor.h"
#include "Sick_VBat.h"
#include "Evitement.h"
#include "Gestion_IO_AU_Config.h"
#include "Timers.h"
#include "Cmd_For_Move.h"
#include "Cmd_From_Communication.h"
#include "Interpreteur.h"
#include "UART_Cmd.h"
#include "lib_asserv/private/tools.h"


#endif	/* MAIN_H */
