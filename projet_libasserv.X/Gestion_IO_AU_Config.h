

#ifndef __GESTION_IO_H
#define __GESTION_IO_H


#define TRIS_DETECT_10V     _TRISA2
#define PIN_DETECT_10V      _RA2

#define TRIS_CONF1      _TRISC4
#define TRIS_CONF2      _TRISC5

#define PIN_CONF1       _RC4
#define PIN_CONF2       _RC5


#define PULL_UP_CONF1   _CN25PUE    //CN25
#define PULL_UP_CONF2   _CN26PUE    //CN26
//#define PULL_UP_CONF4   _CN23PUE    //CN23


#define PIN_TEAM _RA4       // ==> Pin 3.3V numero 2
#define TRIS_TEAM _TRISA4

#define PIN_LAISSE _RC3     // ==> Pin 5V numero 4.
#define TRIS_LAISSE _TRISC3

#define PIN_TURBINE       _LATB8
#define TRIS_TURBINE      _TRISB8

void Gestion_IO_AU_Config_Init(void);

void Gestion_IO_AU_Config_Loop(void);

uint8_t Get_Number_Config(void);


#endif

