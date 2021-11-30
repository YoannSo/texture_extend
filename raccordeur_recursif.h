#ifndef _RACCORDEUR_RECURSIF_H
#define	_RACCORDEUR_RECURSIF_H

#include "raccordeur.h"

class RaccordeurRecursif : public Raccordeur {


    virtual int calculerRaccord(MatInt2* distances, int* coupe);

    virtual ~RaccordeurRecursif(); //destructeur
};

void calcul_Recursif(MatInt2* distances, int* coupe, int maxLargeur,MatInt2* E);

int smallest(int x, int y, int z);

int getOfsetOfMin(int x,int y, int z);
#endif 