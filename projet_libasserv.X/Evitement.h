

#ifndef __EVITEMENT_H
#define __EVITEMENT_H

void Init_Evitement(void);

void Choose_Enabled_Sicks(int Sicks_En);
void Choose_Enabled_US(int US_En);

void New_Order_Evitement_Handling(void);

//void Must_do_Gestion_Evitement(void);

void Gestion_Evitement_Every_few_ms(void);
char Get_Blocked_Evitement (void);




#endif