#ifndef _MATINT2_H
#define _MATINT2_H

#include <cstdlib>
#include <stdio.h>

// Implementation de matrices a 2 dimensions
// TRES NAIF: le code ici n'est pas forcement efficace !
//  (mais il est defensif, et simple)

// matrice a 2 dimensions, valeurs int
class MatInt2
{
public:
  // constructeur : d1 lignes de d2 colonnes
  MatInt2(int d1,int d2);
  ~MatInt2();

  int nLignes();
  int nColonnes();
  int get(int n1,int n2);
  void set(int n1,int n2, int val);
  void display();
  void display(int d1a,int d1b, int d2a,int d2b);

private:
  int dim1,dim2;
  int * pdata;
  int index(int n1, int n2);
  bool checkdim(int n1,int n2,const char* msg);
};

// matrice a 2 dimensions, valeurs double
class MatDouble2
{
public:
  // constructeur : d1 lignes de d2 colonnes
  MatDouble2(int d1,int d2);
  ~MatDouble2();

  int nLignes();
  int nColonnes();
  double get(int n1,int n2);
  void set(int n1,int n2, double val);
  void display();
  void display(int d1a,int d1b, int d2a,int d2b);

private:
  int dim1,dim2;
  double * pdata;
  int index(int n1, int n2);
  bool checkdim(int n1,int n2,const char* msg);
};

#endif
