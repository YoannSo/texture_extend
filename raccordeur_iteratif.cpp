#include "raccordeur_iteratif.h"

int RaccordeurIteratif::calculerRaccord(MatInt2* distances, int* coupe){

    int milieu = distances->nColonnes()/2;
    int hauteur = distances->nLignes();
    int largeur = distances->nColonnes();
    MatInt2 E = MatInt2(hauteur,largeur);
    
    int minI= 0;
    for(int i=0;i<largeur;i++){
        E.set(0,i,distances->get(0,i));
    }
    int masterMin;
    for (int i=1;i<hauteur;i++){
        for(int j=0;j<largeur;j++){
            if(j==0){
                masterMin=min(E.get(i-1,j),E.get(i-1,j+1));
            }
            else if(j==largeur-1){
                masterMin=min(E.get(i-1,j),E.get(i-1,j-1));
            } else {
                masterMin=smallest(E.get(i-1,j-1),E.get(i-1,j),E.get(i-1,j+1));
            }
             E.set(i,j,masterMin+distances->get(i,j));
        }
    }
    
    for(int i = 1;i<largeur;i++){ // calcul du premier min
        if(E.get(hauteur-1,minI)<E.get(hauteur-1,i))
            continue;
        minI = i;
    }


    for(int j=hauteur-2;j>=0;j--){
        if(minI==0){
            if(E.get(j,minI)<E.get(j,minI+1)){
                coupe[j]=minI;
            }
            else{
                coupe[j] =  minI+1;
            }
        }
        else if(minI==largeur-1){
            if(E.get(j,minI)<E.get(j,minI-1)){
                coupe[j]=minI;
            }
            else{
                coupe[j] = minI-1;
            }
        }
        else{
            coupe[j] = minI+getOfsetOfMin(E.get(j,minI-1),E.get(j,minI),E.get(j,minI+1));
        }
        minI = coupe[j];

    }
    
    return 0;
}

RaccordeurIteratif::~RaccordeurIteratif(){
      // pas de ressources a liberer
}
