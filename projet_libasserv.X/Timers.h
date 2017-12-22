#ifndef __TIMER_MS_H
#define __TIMER_MS_H

// asserv toutes les 10 ms
#define ASSERV_EVERY 10

void Timers_Init(void);

//unsigned int Get_Maxtime_Seq_AX12(void);
//void Set_Maxtime_Seq_AX12(unsigned int val);
//
//void Set_Maxtime(unsigned int val);
//char Get_Maxtime(void);

//ms

void Delay_ms(uint32_t Nb_ms);

#define MAX_DELAY_WATCHDONE  5000   
void Raz_Delay_WatchDone(void);

uint8_t Get_Status_Match(void);

uint32_t GetTimer_ms();

#endif