#include "permuteur.h"

// ======================
//  Constructeur
// ======================
using namespace std;
#include <iostream>
#include <bits/stdc++.h>
void shuffle_array(int arr[], int n)
{
 
    // To obtain a time-based seed
    unsigned seed = 0;
 
    // Shuffling our array
    shuffle(arr, arr + n,
            default_random_engine(seed));
 
    // Printing our array
    //for (int i = 0; i < n; ++i)
        //cout << arr[i] << " ";
    //cout << endl;
}

Permuteur::Permuteur(int max) {
    this->perm=(int*)malloc(sizeof(int)*max);
    for(int i=0;i<max;i++){
        //cout << i;
        this->perm[i]=i;
    } 
    this->max = max;
    indices=(int*)malloc(sizeof(max)); // tableau des indices � permuter
    shuffle_array(this->perm,max);
    this->i_perm=0;
}

Permuteur::~Permuteur(){
    free(this->perm);
    free(this->indices);
}

int Permuteur::suivant(){
   if(this->i_perm>this->max){
        shuffle_array(this->perm,sizeof(this->perm) / sizeof(this->perm[0]));
        this->i_perm=0;
    }
    int returned=this->perm[this->i_perm];
    this->i_perm++;
    return returned;
}

