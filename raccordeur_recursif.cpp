#include "raccordeur_recursif.h"
int RaccordeurRecursif::calculerRaccord(MatInt2* distances, int* coupe){

    int milieu = distances->nColonnes()/2;
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
    for(int j=hauteur-2;j>=0;j--){ // ici ca marche pas 
        if(minI==0){
            if(E.get(j,minI)<E.get(j,minI+1))
                coupe[j]=minI;
            else
                coupe[j] =  minI+1;
        }
        else if(minI==largeur-1){
            if(E.get(j,minI)<E.get(j,minI-1))
                coupe[j]=minI;
            else
                coupe[j] = minI-1;
        }
        else{
            coupe[j] = minI+getOfsetOfMin(E.get(j,minI-1),E.get(j,minI),E.get(j,minI+1));
        }
        minI = coupe[j];

    }
    
    return 0;
}

void calcul_Recursif(MatInt2* distances, int* coupe, int indices, MatInt2* E){
    int cost;
    int y;
    int hauteur = distances->nLignes();
    int largeur=distances->nColonnes();
    if(indices==0){
        for(int i=0;i<largeur;i++){
            E->set(0,i,distances->get(0,i));
        }
    }
    else{
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
                masterMin=smallest(E->get(indices-1,i-1),E->get(indices-1,i),E->get(indices-1,i+1));
                

            }
            
            E->set(indices,i,masterMin+distances->get(indices,i));

        }
    }
}

RaccordeurRecursif::~RaccordeurRecursif(){
      // pas de ressources a liberer
}


int smallest(int x, int y, int z){
    return min(min(x, y), z);
}

int getOfsetOfMin(int x,int y, int z){
    if(x<y){
        if(x<z){
            return -1;
        }
        if(y<z){
            return 0;
        }
        return 1;
    }
    if(y<z)
        return 0;
    return 1;
}