#ifndef _RACCORDEUR_H
#define	_RACCORDEUR_H

#include "matrix2d.h"

/*!
 * Definit le concept de raccordeur qui est utilise pour calculer un raccord
 * entre deux blocs d'une image ainsi que le cout de ce raccord.
 * 
 * Cette classe est une classe abstraite pure (interface en Java).
 * Ceci signifie qu'elle definit un concept sans en donner aucune implementation.
 * 
 */
class Raccordeur {
public:
  /*!
   * 
   * Calcule un raccord entre deux blocs. Les blocs ne sont pas donnes, seule la
   * zone de recouvrement est fournie. Elle est fournie sous la forme d'un
   * tableau d'entiers a deux dimensions. Chaque case de ce tableau donne la
   * distance (distance dans l'espace de couleur) entre le pixel d'un des deux
   * blocs et celui de l'autre bloc.
   *
   * La methode doit realiser une coupe verticale dans la matrice. Le code se
   * charge d'appeler calculerRaccord et se charge de transposer l'entree et la
   * sortie quand il a besoin d'une coupe horizontale.
   *
   * Cette methode doit retourner le cout du raccordement et remplir le tableau
   * coupe avec les indices decrivant la coupe. Le tableau de coupe contient
   * autant d'elements qu'il y a de lignes dans la matrice distances.
   *
   * @param[in] distances   matrices des distances dans la zone de recouvrement
   * @param[out] coupe      tableau d'indices representant la coupe
   * @return le cout du raccordement calcule
   */
  virtual int calculerRaccord(MatInt2* distances, int* coupe) = 0;
  // virtual permet le mecanisme de liaison dynamique
  // =0 signifie que l'on ne donne pas d'implementation
  // virtual doit figurer dans les classes derivees de cette classe

  virtual ~Raccordeur(){}; // destructeur vide
  // toute classe ayant des methodes virtuelles doit avoir son destructeur virtuel.

};

#endif	/* _RACCORDEUR_H */
