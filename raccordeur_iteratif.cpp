#include "raccordeur_iteratif.h"

int RaccordeurIteratif::calculerRaccord(MatInt2* distances, int* coupe){
    int hauteur = distances->nLignes();
    int largeur = distances->nColonnes();
    MatInt2 E = MatInt2(hauteur,largeur);
    //E est la valeur minimale de l'erreure cumulative de ce pixel.

    int minI= 0;
    for(int i=0;i<largeur;i++){
        E.set(0,i,distances->get(0,i));
    }
    int masterMin;
    //pour chaque hauteur
    for (int i=1;i<hauteur;i++){
      //et pour chaque cases de ces hauteurs on calcule la valeur de E, qui est le
      //minimum des 3 cases de la hauteur - 1 qui sont en contact avec la case en cours de
      //traitement.
        for(int j=0;j<largeur;j++){
            if(j==0){
                masterMin=min(E.get(i-1,j),E.get(i-1,j+1));
            }
            else if(j==largeur-1){
                masterMin=min(E.get(i-1,j),E.get(i-1,j-1));
            } else {
                masterMin=min_double3(E.get(i-1,j-1),E.get(i-1,j),E.get(i-1,j+1));
            }
             E.set(i,j,masterMin+distances->get(i,j));
        }
    }

    for(int i = 1;i<largeur;i++){ // calcul du premier min
        if(E.get(hauteur-1,minI)<E.get(hauteur-1,i))
            continue;
        minI = i;
    }


    //pour chaque hauteur à partir de chaque dernière coupe ajoutée, on calcule
    //l'indice de la coupe liée à cette hauteur puis on l'ajoute au tableau
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
            coupe[j] = minI+ind_min_double(E.get(j,minI-1),E.get(j,minI),E.get(j,minI+1));
        }
        minI = coupe[j];

    }

    return 0;
}

RaccordeurIteratif::~RaccordeurIteratif(){
      // pas de ressources a liberer
}
