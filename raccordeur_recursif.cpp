#include "raccordeur_recursif.h"

int RaccordeurRecursif::calculerRaccord(MatInt2* distances, int* coupe){

    int milieu = distances->nColonnes()/2;
    int hauteur = distances->nLignes();
    int largeur = distances->nColonnes();
    int cost = calcul_Recursif(distances,coupe,largeur);


    return cost;


}

int calcul_Recursif(MatInt2* distances, int* coupe, int maxLargeur){
    int cost;
    int y;
    int hauteur = distances->nLignes();
    for (y = 0; y < hauteur; y++) {
       /*  cout +=  */
    }

    if(maxLargeur > 1){
        cost = calcul_Recursif(distances, coupe, maxLargeur-1);
    }

    return cost;
}

RaccordeurRecursif::~RaccordeurRecursif(){
      // pas de ressources a liberer
}