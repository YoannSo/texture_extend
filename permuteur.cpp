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
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

Permuteur::Permuteur(int max) {
    this->perm=(int*)malloc(sizeof(max));
    for(int i=0;i<max;i++){
        this->perm[i]=i;
    } 
    this->max = max;
    indices=(int*)malloc(sizeof(max)); // tableau des indices � permuter
    shuffle_array(this->perm,sizeof(this->perm) / sizeof(this->perm[0]));
    i_perm=0;
}

Permuteur::~Permuteur(){
    free(this->perm);
}

int Permuteur::suivant(){
    if(this->i_perm>this->max){
        shuffle_array(this->perm,sizeof(this->perm) / sizeof(this->perm[0]));
        this->i_perm=0;
    }
    int returned=perm[this->i_perm];
    this->i_perm++;
    return returned;
}

