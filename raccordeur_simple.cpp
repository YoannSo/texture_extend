
#include "raccordeur_simple.h"

int RaccordeurSimple::calculerRaccord(MatInt2* distances, int* coupe)
{
  int milieu = distances->nColonnes()/2;
  int hauteur = distances->nLignes();
  int largeur = distances->nColonnes();
  int cout = 0;
  int y, x;
  for (y = 0; y < hauteur; y++) {
    for (x = 0; x < largeur; x++) {
      cout += distances->get(y, x);
    }
  }
  for (y = 0; y < hauteur; y++) {
    coupe[y] = milieu;
  }
  for (y = 0; y < hauteur; y++) {
    //coupe[y] = largeur * y / hauteur; // cela ferait une coupe diagonale
  }
  return cout;
}

RaccordeurSimple::~RaccordeurSimple()
{
  // pas de ressources a liberer
}
