#ifndef _RACCORDEUR_RECURSIF_H
#define	_RACCORDEUR_RECURSIF_H

#include "raccordeur.h"
#include "tools.h"

class RaccordeurRecursif : public Raccordeur {


    virtual int calculerRaccord(MatInt2* distances, int* coupe);

    virtual ~RaccordeurRecursif(); //destructeur
};

void calcul_Recursif(MatInt2* distances, int* coupe, int maxLargeur,MatInt2* E);

#endif
