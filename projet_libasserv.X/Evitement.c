
#include <xc.h>

#include "main.h"

uint8_t Motion_Free_Activ_Sick[4];
uint8_t Motion_Free_Activ_US[2];

volatile uint8_t Can_Restart_Order = 0;
volatile uint8_t Old_Blocked_Front = 0, Old_Blocked_Back = 0;

char var_debug;

void Init_Evitement(void) {
    Motion_Free_Activ_US[0] = 0;
    Motion_Free_Activ_US[1] = 0;
    Motion_Free_Activ_Sick[0] = 1;
    Motion_Free_Activ_Sick[1] = 1;
    Motion_Free_Activ_Sick[2] = 1;
    Motion_Free_Activ_Sick[3] = 1;
    // Soft IT pour la gestion des secteurs
    // qui prend l'IT SPI1 Erreur
    //IFS0bits.SPI1EIF = 0;
    //IEC0bits.SPI1EIE = 1;
    //IPC2bits.SPI1EIP = 1; // pas tres prioritaire
    //_SPI1ErrInterrupt
}

void Choose_Enabled_Sicks(int Sicks_En) {

    Motion_Free_Activ_Sick[0] = ((Sicks_En & 0x01) != 0);
    Motion_Free_Activ_Sick[1] = ((Sicks_En & 0x02) != 0);
    Motion_Free_Activ_Sick[2] = ((Sicks_En & 0x04) != 0);
    Motion_Free_Activ_Sick[3] = ((Sicks_En & 0x08) != 0);
}

void Choose_Enabled_US(int US_En) {
    Motion_Free_Activ_US[0] = ((US_En & 0x01) != 0);
    Motion_Free_Activ_US[1] = ((US_En & 0x02) != 0);
}

void New_Order_Evitement_Handling(void) {
    Can_Restart_Order = 0;
    Old_Blocked_Front = 0;
    Old_Blocked_Back = 0;
    Raz_Delay_WatchDone();
}
/*
// TODO : enlever ça
void Must_do_Gestion_Evitement(void) {
    IFS0bits.SPI1EIF = 1;
}

void __attribute__((interrupt, auto_psv)) _SPI1ErrInterrupt(void) {
    Gestion_Evitement_Every_few_ms();
    IFS0bits.SPI1EIF = 0;
}*/

void Gestion_Evitement_Every_few_ms(void) {/*
    SICK1_IS_FRONT  0
#define SICK2_IS_FRONT  0
#define SICK3_IS_FRONT  1
#define SICK4_IS_FRONT  1
#define SICK1_IS_BACK   1*/

    uint8_t Blocked_Front, Blocked_Back;
    //    // sector 0 = obstacle detecté, sector 1 = "sûr
    //    Blocked_Front = ((!Get_Sick_Sector(0)) && SICK1_IS_FRONT && Motion_Free_Activ_Sick[0]) ||
    //                    ((!Get_Sick_Sector(1)) && SICK2_IS_FRONT && Motion_Free_Activ_Sick[1]) ||
    //                    ((!Get_Sick_Sector(2)) && SICK3_IS_FRONT && Motion_Free_Activ_Sick[2]) ||
    //                    ((!Get_Sick_Sector(3)) && SICK4_IS_FRONT && Motion_Free_Activ_Sick[3]) ||
    //
    //                    //((!Get_US_Sector(0)) && US1_IS_FRONT && Motion_Free_Activ_US[0]) ||
    //                    //((!Get_US_Sector(1)) && US2_IS_FRONT && Motion_Free_Activ_US[1]) ||
    //            0;  // juste pour commentaires faciles
    //
    //    Blocked_Back  = ((!Get_Sick_Sector(0)) && SICK1_IS_BACK && Motion_Free_Activ_Sick[0]) ||
    //                    ((!Get_Sick_Sector(1)) && SICK2_IS_BACK && Motion_Free_Activ_Sick[1]) ||
    //                    ((!Get_Sick_Sector(2)) && SICK3_IS_BACK && Motion_Free_Activ_Sick[2]) ||
    //                    ((!Get_Sick_Sector(3)) && SICK4_IS_BACK && Motion_Free_Activ_Sick[3]) ||
    //
    //                    //((!Get_US_Sector(0)) && US1_IS_BACK && Motion_Free_Activ_US[0]) ||
    //                    //((!Get_US_Sector(1)) && US2_IS_BACK && Motion_Free_Activ_US[1]) ||
    //            0; // juste pour commentaires faciles

    // sector 0 = obstacle detecté, sector 1 = "sûr
    Blocked_Front = ((!Sick_Sector[0]) && SICK1_IS_FRONT && Motion_Free_Activ_Sick[0]) ||
            ((!Sick_Sector[1]) && SICK2_IS_FRONT && Motion_Free_Activ_Sick[1]) ||
            ((!Sick_Sector[2]) && SICK3_IS_FRONT && Motion_Free_Activ_Sick[2]) ||
            ((!Sick_Sector[3]) && SICK4_IS_FRONT && Motion_Free_Activ_Sick[3]) ||

            //((!Get_US_Sector(0)) && US1_IS_FRONT && Motion_Free_Activ_US[0]) ||
            //((!Get_US_Sector(1)) && US2_IS_FRONT && Motion_Free_Activ_US[1]) ||
            0; // juste pour commentaires faciles

    Blocked_Back = ((!Sick_Sector[0]) && SICK1_IS_BACK && Motion_Free_Activ_Sick[0]) ||
            ((!Sick_Sector[1]) && SICK2_IS_BACK && Motion_Free_Activ_Sick[1]) ||
            ((!Sick_Sector[2]) && SICK3_IS_BACK && Motion_Free_Activ_Sick[2]) ||
            ((!Sick_Sector[3]) && SICK4_IS_BACK && Motion_Free_Activ_Sick[3]) ||

            //((!Get_US_Sector(0)) && US1_IS_BACK && Motion_Free_Activ_US[0]) ||
            //((!Get_US_Sector(1)) && US2_IS_BACK && Motion_Free_Activ_US[1]) ||
            0; // juste pour commentaires faciles


    Blocked_Front = Blocked_Front && ((Sens_Vitesse_Deplacement() == 1) || Old_Blocked_Front);
    Blocked_Back = Blocked_Back && ((Sens_Vitesse_Deplacement() == -1) || Old_Blocked_Back);

    if (Blocked_Front && !Old_Blocked_Front) {
        if (Is_Asserv_Mode_Pos())
            Can_Restart_Order = 1;
        motion_free();
    } else if (!Blocked_Front && Old_Blocked_Front && Can_Restart_Order) {
        Can_Restart_Order = 0;
        load_last_order();
    }

    if (Blocked_Back && !Old_Blocked_Back) {
        if (Is_Asserv_Mode_Pos())
            Can_Restart_Order = 1;
        motion_free();
    } else if (!Blocked_Back && Old_Blocked_Back && Can_Restart_Order) {
        Can_Restart_Order = 0;
        load_last_order();
    }

    Old_Blocked_Front = Blocked_Front;
    Old_Blocked_Back = Blocked_Back;

    //ReleaseSick(channel_conv+1);			// on previent la PI

    //DetectSick(channel_conv+1);				// on previent la PI

}

char Get_Blocked_Evitement (void) {
    return Old_Blocked_Back + (Old_Blocked_Front*2);
}
        
