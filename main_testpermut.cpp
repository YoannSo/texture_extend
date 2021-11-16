#include <stdio.h> // printf
#include "permuteur.h"

int main(int argc, char** argv) {
  Permuteur * p = new Permuteur (12);
  for (int i=0; i<12; i++)
    printf("%d  ",p->suivant());
  

  return 0;
}

