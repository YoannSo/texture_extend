#include <stdio.h> // printf
#include "permuteur.h"

int main(int argc, char** argv) {
  Permuteur * p = new Permuteur (100);
  for (int i=0; i<100; i++)
    printf("%d  ",p->suivant());

  return 0;
}

