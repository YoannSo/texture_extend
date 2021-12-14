#include "raccordeur_recursif.h"


int RaccordeurRecursif::calculerRaccord(MatInt2* distances, int* coupe){

    int hauteur = distances->nLignes();
    int largeur = distances->nColonnes();

    MatInt2 E = MatInt2(hauteur,largeur);
    calcul_Recursif(distances,coupe, hauteur-1,&E);
    int minI= 0;

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
                coupe[j] = minI+1;
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

void calcul_Recursif(MatInt2* distances, int* coupe, int indices, MatInt2* E){
    int largeur=distances->nColonnes();
    //si on est à la première ligne de notre image, on ne peut pas utiliser les valeurs de la ligne précédante
    //donc on initialise la valeur de E à la valeur de la distance
    if(indices==0){
        for(int i=0;i<largeur;i++){
            E->set(0,i,distances->get(0,i));
        }
    }
    //sinon on ajoute la valeur mininale des trois valeurs de l'étage inférieur sous la cellule avec
    //la valeur de la distance
    else{
        //on calcule la hauteur inférieur
        calcul_Recursif(distances,coupe,indices-1,E);
        for(int i=0;i<largeur;i++){
            int masterMin;
            if(i==0){
                 masterMin=min(E->get(indices-1,i),E->get(indices-1,i+1));
            }
            else if(i==largeur-1){
                 masterMin=min(E->get(indices-1,i),E->get(indices-1,i-1));
            }
            else{
                masterMin=min_double3(E->get(indices-1,i-1),E->get(indices-1,i),E->get(indices-1,i+1));
            }
            E->set(indices,i,masterMin+distances->get(indices,i));
        }
    }
}

RaccordeurRecursif::~RaccordeurRecursif(){
      // pas de ressources a liberer
}
