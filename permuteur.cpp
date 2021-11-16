#include "permuteur.h"
#include <string>

// ======================
//  Constructeur
// ======================
Permuteur::Permuteur(int max) {
    this->perm=NULL; 
    this->max = max;
    indices=NULL; // tableau des indices � permuter
    i_perm=0;
}

Permuteur::~Permuteur(){
    
}

int Permuteur::suivant(){
    return 0;
}

