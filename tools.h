#ifndef _TOOLS_H
#define _TOOLS_H

#include <stdlib.h>
#include <limits>

/*!
 * Retourne un nombre entier au hasard entre min et max inclus.
 */
static inline int randomIntervalle(int min, int max) {
  return min+int((max-min+1.0)*rand()/(RAND_MAX + 1.0));
}

/*!
 * Retourne la plus petite des deux valeurs recues.
 */
static inline double min_double2(double i0, double i1){
  return i0<=i1 ? i0 : i1;
}

/*!
 * Retourne la plus petite des trois valeurs recues.
 */
static inline double min_double3(double i0, double i1, double i2)
{
  return min_double2(i0, min_double2(i1, i2));
}

/*!
 * Retourne l'indice, dans {-1,0,1}, de la plus petite des trois valeurs recues.
 * @retval -1 quand i0 est le plus petit
 * @retval  0 quand i1 est le plus petit
 * @retval  1 quand i2 est le plus petit
 * 
 * Peut etre utilise avec uniquement deux parametres (retourne alors -1 ou 0).
 */
static inline int ind_min_double(double i0, double i1, double i2 = std::numeric_limits<double>::max())
{
  double a = min_double2(i1,i2);
  return i0<=a ? -1 : i1<=i2 ? 0 : 1;
}

#endif // _TOOLS_HH
