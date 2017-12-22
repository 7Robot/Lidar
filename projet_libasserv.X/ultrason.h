
#ifndef __ULTRASON
#define __ULTRASON

#define NUMBER_OF_US                2
#define ULTRASON_THRESOLD           250      // seuil d'alarme en mm
#define ULTRASON_THRESOLD_TRIGGER   25      // zone du triger de schmitt


#define PIN_ULTRASON_AR        _LATC4            // RC4 = connecteur 5V n3
#define TRIS_ULTRASON_AR       _TRISC4
#define PIN_CN_ULTRASON_AR_IE  _CN25IE
//#define PULL_UP_ULTRASON_AR    _CN25PUE
#define ULTRASON_AR_NUM_PIN     4

#define PIN_ULTRASON_AV        _LATC5            // RC5 = connecteur 5V n2
#define TRIS_ULTRASON_AV       _TRISC5
#define PIN_CN_ULTRASON_AV_IE  _CN26IE
//#define PULL_UP_ULTRASON_AV    _CN26PUE
#define ULTRASON_AV_NUM_PIN     5



#define U_ETAT_OFF              0x00
#define U_ETAT_FOR_SEND1        0x01
#define U_ETAT_SEND1            0x02
#define U_ETAT_WAIT             0x04
//#define U_ETAT_WAIT0            0x08
#define U_ETAT_WAIT0_OVERSHOOT  0x10
#define U_ETAT_WAIT_FOR_RESTART 0x20

#define US1_IS_FRONT  1
#define US2_IS_FRONT  0
#define US1_IS_BACK   0
#define US2_IS_BACK   1


void Init_Ultrasons(void);
void Init_CN(void);

void Start_Stop_Debug_Ultrason(void);

int Get_US_Sector(int US);

void Set_Threshold_US(int limit_mm);


#endif

