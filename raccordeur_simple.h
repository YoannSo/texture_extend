#ifndef _RACCORDEUR_SIMPLE_H
#define	_RACCORDEUR_SIMPLE_H

#include "raccordeur.h"

/*!
 * Raccordeur simple : il coupe au milieu et calcule le cout du raccord comme
 * etant la somme des distances sur la zone de recouvrement.
 *
 * Cette classe est une classe concrete et derive/implemente le concept de
 * Raccordeur (\see{Raccordeur}).
 */
class RaccordeurSimple : public Raccordeur {

  /*!
   * le calcul du raccord (\see{Raccordeur#calculerRaccord}).
   * Le code effectif se trouve dans RaccordeurSimple.cpp
   */
  virtual int calculerRaccord(MatInt2* distances, int* coupe);

  virtual ~RaccordeurSimple(); // destructeur

};

#endif	/* _RACCORDEUR_SIMPLE_H */

