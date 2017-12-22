/*
 * Sick
 * Compiler : Microchip xC16
 * �C : 33FJ64MC804
 * Mai 2014
 *    ____________      _           _
 *   |___  /| ___ \    | |         | |
 *      / / | |_/ /___ | |__   ___ | |_
 *     / /  |    // _ \| '_ \ / _ \| __|
 *    / /   | |\ \ (_) | |_) | (_) | |_
 *   /_/    |_| \_\___/|____/ \___/'\__|
 *			      7robot.fr
 */

#ifndef __SICK_H
#define	__SICK_H

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define NUMBER_OF_SICK 4
#define NUMBER_OF_POINTS_MOY_SICK 8

#define SICK1_IS_FRONT  1
#define SICK2_IS_FRONT  1
#define SICK3_IS_FRONT  0
#define SICK4_IS_FRONT  0
#define SICK1_IS_BACK   0
#define SICK2_IS_BACK   0
#define SICK3_IS_BACK   1
#define SICK4_IS_BACK   1


#define AN_CH_SICK_1   7 //AN7		=> // an_sick 1  //
#define AN_CH_SICK_2   6 //AN6		=> // an_sick 3  //
#define AN_CH_SICK_3   5 //AN5		=> // an_sick 2  //
#define AN_CH_SICK_4   4 //AN4		=> // an_sick 4  //
#define AN_CH_V_BAT    0

// 1 = AVD
// 2 = ARD
// 3 = ARG
// 4 = AVG
/*
#define SICK_AR_DROIT   0   //sick 1
#define SICK_AR_GAUCHE  1   //sick 2
#define SICK_AV_DROIT   2   //sick 3
#define SICK_AV_GAUCHE  3   //sick 4
 */


#define DEFAULT_THRESHOLD 420 // 300 ~> 16 cm //420 pas assez homolo1
#define MARGIN_SICK 20		// en réalité, plutot un triger de schmitt
#define SICK_LIMIT_MIN 30	// limite minimum qui fait passer un sick en off


#define TIMER_FOR_VBAT  399    //  400 / sec, donc 2000 donne toutes les 5 secondes
#define NUMBER_MEAS_VBAT 8


//#define DEBUG_SICK

/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/

void InitSick_VBat();

unsigned int Get_Sick(int Sick_Voulu);
char Get_Sick_Sector(int Sick_Voulu);
extern volatile char Sick_Sector[];

void Start_Stop_Debug_Sick(void);

unsigned int Get_VBat(void);




#endif	/* SICK_H */

