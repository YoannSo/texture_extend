#ifndef _PERMUTEUR_H
#define	_PERMUTEUR_H

/*!
 *
 * Les instances de cette classe sont des iterateurs sur des permutations
 * aleatoires d'un intervalle donne. Ces instances sont construites en donnant
 * un nombre entier definissant l'intervalle d'interet. Des appels successifs
 * a suivant() permettent d'acceder aux elements de la premiere permutation
 * puis de la seconde et ainsi de suite.
 *
 * Par exemple, si l'on construit une permutation en donnant max=3, on
 * s'interesse alors a des permutations de l'intervalle [0,1,2]. Des appels
 * successifs a suivant pourraient alors renvoyer :
 *  1 2 0  0 2 1  0 1 2  2 1 0  2 1 0  1 0 2 ...
 * Les 18 valeurs successives retournees par les appels a suivant(), une fois
 * groupees par max=3, mettent bien en evidence que l'on retourne les elements
 * d'une permutation, puis d'une autre, ...
 *
 */
class Permuteur {
// A FAIRE : classe a completer au besoin
// A FAIRE : les implementations dans permuteur.cpp

public:

  /*!
   * Construit un nouveau permuteur qui retournera des nombres entiers dans
   * l'intervalle [0 .. (max-1)].
   */
  Permuteur(int max);

  /*!
   * Libere les ressources utilisees par ce permuteur.
   */
  ~Permuteur();

  /*!
   * Retourne l'entier suivant.
   */
  int suivant();

 private :
  int max;
  int* perm; // tableau de la permutation.
  int * indices; // tableau des indices à permuter
  int i_perm;  // indice d'avancement dans la permutation.

};


#endif	/* _PERMUTEUR_H */

