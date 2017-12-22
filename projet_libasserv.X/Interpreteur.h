
#ifndef __INTERPRETEUR_H
#define __INTERPRETEUR_H

#define MAX_CMD_LENGTH 100

typedef struct {
    char* Name;
    u8 (*Func)(void);
} Command;


void Interp(char c);
u8 Comp_Cmd (u16 Len, u16 ind);
char To_UpperCase (char c);
u8 Get_Param_Float (float *retour);
u8 Print_All_CMD_Cmd (void);

#endif