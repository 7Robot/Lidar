#ifndef LIBASSERV_H
#define LIBASSERV_H

#ifndef NULL
#define NULL 0
#endif

/*****************************    Structures    *******************************/

/**
 * \struct Position
 * \brief Position du robot
 */
typedef struct {
    float x;        /*!< Position du robot sur l'axe x en mètre*/
    float y;        /*!< Position du robot sur l'axe x en mètre*/
    float t;        /*!< Orientation du robot en radian*/
} Position;

/**
 * \struct Speed
 * \brief Vitesse du robot
 */
typedef struct {
    float v;        /*!< Vitesse linéaire du robot*/
    float vt;       /*!< Vitesse angulaire du robot*/
} Speed;

/**
 * \struct Acceleration
 * \brief Accélération du robot.
 */
typedef struct {
    float a;        /*!< Accélération linéaire du robot en m/s2*/
    float at;       /*!< Accélération angulaire du robot rad/s2*/
    float v_vt;     /*!< Je ne sais pas en rad*m/s2*/
} Acceleration;

/**
 * \struct RobotState
 * \brief Position, vitesse et accélération du robot.
 */
typedef struct {
    Position pos;       /*!< Position du robot*/
    Speed speed;        /*!< Vitesses du robot*/
    Acceleration acc;   /*!< Accélération du robot*/
} RobotState;

/*############################################################################*/
/*                                  Motion                                    */
/*############################################################################*/

// {v, vt} v = 0.9 * v max moteur, vt = v/(entre roues/2)
#ifndef DEFAULT_CONSTRAINT_V_MAX
#define DEFAULT_CONSTRAINT_V_MAX {0.9, 4}
#endif

/* {a, at, v_vt} a = a max sans glissement, at = a/(entre roues/2),
 * v_vt = acc centripete (trop fort -> erreur odo) 0.1g
 */
#ifndef DEFAULT_CONSTRAINT_A_MAX
#define DEFAULT_CONSTRAINT_A_MAX {1.8, 7.28, 0.981}
#endif

#ifndef BLOCK_LIMIT // 5s
#define BLOCK_LIMIT 500
#endif

/*############################################################################*/
/*                                    Odo                                     */
/*############################################################################*/

#ifndef DEFAULT_ODO_COEFS // {tic/m, m/tic, entre roues}
#define DEFAULT_ODO_COEFS {53876, 0.000018561, 0.184}
#endif

#ifndef DEFAULT_PERIOD
#define DEFAULT_PERIOD 0.010
#endif

/*############################################################################*/
/*                                  Asserv                                    */
/*############################################################################*/

#ifndef DEFAULT_ASSERV_MODE
#define DEFAULT_ASSERV_MODE ASSERV_MODE_OFF
#endif
#ifndef DEFAULT_STOP_DISTANCE
#define DEFAULT_STOP_DISTANCE 0.005 // +-5mm
#endif
#ifndef DEFAULT_STOP_ANGLE // en radian
#define DEFAULT_STOP_ANGLE 0.08726// +-5deg
#endif

/*############################################################################*/
/*                                   PID                                      */
/*############################################################################*/

// 2 PID lies a l'asserve en position (delta: position absolue, alpha: position angulaire)
#ifndef DEFAULT_PID_POS_COEFS_DELTA // {kp, ki, kd, mu_p}
#define DEFAULT_PID_POS_COEFS_DELTA {5,0,140,1} // {7,0,140,1}
#endif
#ifndef DEFAULT_PID_POS_COEFS_ALPHA // {kp, ki, kd, mu_p}
#define DEFAULT_PID_POS_COEFS_ALPHA {17,0,450,1} // {17,0,450,1}
#endif

// 2 PID lies a l'asserve en vitesse (delta: vitesse absolue, alpha: vitesse angulaire)
#ifndef DEFAULT_PID_COEFS_DELTA // {kp, ki, kd, mu_p}
#define DEFAULT_PID_COEFS_DELTA {90,8,0,1} // {90,8,0,1}
#endif
#ifndef DEFAULT_PID_COEFS_ALPHA // {kp, ki, kd, mu_p}
#define DEFAULT_PID_COEFS_ALPHA {30,1,0,1} //  {30,1,0,1}
#endif

/* valeurs max de l'intÃ©grale en vitesse et vitesse angulaire
 *  (pas trop petit sinon on atteint pas la vitesse demandée,
 *   on pourrait trouver une meilleur borne sup)
 */
#ifndef DEFAULT_PID_MAX_INT_DELTA
#define DEFAULT_PID_MAX_INT_DELTA 1000
#endif
#ifndef DEFAULT_PID_MAX_INT_ALPHA
#define DEFAULT_PID_MAX_INT_ALPHA 1000
#endif

/* conditions d'arrÃªt des PID en vitesse et vitesse angulaire
 * (empiriques, ne pas trop toucher)
 */
#ifndef DEFAULT_PID_EPS_DELTA
#define DEFAULT_PID_EPS_DELTA {0.015,0.015}
#endif
#ifndef DEFAULT_PID_EPS_ALPHA
#define DEFAULT_PID_EPS_ALPHA {0.15,0.15}
#endif

/*############################################################################*/
/*                                  DEBUG                                     */
/*############################################################################*/

#ifndef DEFAULT_DEBUG_MODE
#define DEFAULT_DEBUG_MODE 0
#endif

#endif // _LIB_ASSERV_DEFAULT_H_
