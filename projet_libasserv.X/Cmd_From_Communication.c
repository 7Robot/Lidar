

#include "main.h"


// INIT

uint8_t INIT_Cmd(void) {
    Init_All();
    return 0;
}

uint8_t Robot_Running_Status_Cmd(void) {  
    
    return 0;
}

uint8_t Robot_Static_Status_Cmd(void) {
    
    return 0;
}

uint8_t Test_Cmd(void) {
    float valfa, valfb, valfc;

    if (Get_Param_Float(&valfa))
        return 1;

    printf("J'ai recu %f\r\n", valfa);
    Delay_ms(200);
    if (Get_Param_Float(&valfb))
        return 1;
    printf("J'ai recu %f\r\n", valfb);
    Delay_ms(200);
    if (Get_Param_Float(&valfc))
        return 1;
    printf("J'ai recu %f\r\n", valfc);
    Delay_ms(200);
    return 0;
}

