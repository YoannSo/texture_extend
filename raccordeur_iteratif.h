#ifndef _RACCORDEUR_ITERATIF_H
#define	_RACCORDEUR_ITERATIF_H

#include "raccordeur.h"
#include "utils/utils.h"

class RaccordeurIteratif : public Raccordeur {

    virtual int calculerRaccord(MatInt2* distances, int* coupe);

    virtual ~RaccordeurIteratif(); //destructeur
};



#endif