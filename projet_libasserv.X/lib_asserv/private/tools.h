#ifndef H_TOOLS
#define H_TOOLS

// constante pi
#define PI 3.14159265359

/* Fonctions génériques pouvant servir à plusieurs endroits */

// fonction min pour float
float fmin(float x, float y);

// Renvoyer une valeur comprise entre inf et sup
float limit_float(float valeur, float inf, float sup);
long int limit_int(long int valeur, long int inf, long int sup);

// angle principal
float principal_angle(float angle);

#endif