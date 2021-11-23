
#include "raccordeur_simple.h"
#include "iostream"
using namespace std;

int RaccordeurSimple::calculerRaccord(MatInt2* distances, int* coupe)
{
  int milieu = distances->nColonnes()/2;
  int hauteur = distances->nLignes();
  int largeur = distances->nColonnes();
  int cost = 0;
  int y, x;
  for (y = 0; y < hauteur; y++) {
    for (x = 0; x < largeur; x++) {
      cost += distances->get(y, x);
    }
  }
  //cout << "COUPE Y" << endl;
  for (y = 0; y < hauteur; y++) {
    coupe[y] = milieu;
    //cout << coupe[y] << endl;
  }

  //distances->display();

  for (y = 0; y < hauteur; y++) {
    //coupe[y] = largeur * y / hauteur; // cela ferait une coupe diagonale
  }


  return cost;
}

RaccordeurSimple::~RaccordeurSimple()
{
  // pas de ressources a liberer
}
