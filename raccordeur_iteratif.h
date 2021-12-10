#ifndef _RACCORDEUR_ITERATIF_H
#define	_RACCORDEUR_ITERATIF_H

#include "raccordeur.h"
#include "tools.h"

class RaccordeurIteratif : public Raccordeur {

    virtual int calculerRaccord(MatInt2* distances, int* coupe);

    virtual ~RaccordeurIteratif(); //destructeur
};



#endif
