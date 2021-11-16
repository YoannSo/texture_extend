#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "textureur.h"
#include "descripteur_tache.h"
#include "raccordeur.h"
#include "raccordeur_simple.h"



int main(int argc, char **argv){

    srand((unsigned)time(NULL)); // Pour que le random() soit different d'une execution sur l'autre
    int algo; // Version de l'algorithme
    
    // on instancie un textureur et une tache que l'on va remplir
    Textureur textureur;
    DescripteurTache tache;
    
    // valeurs par defaut
    algo = 0;
    tache.fichierImage = "gravier.tif";
    tache.racineNombreBlocs = 3;
    tache.recouvrement = 20;
    tache.largeurSortie = 600;
    tache.hauteurSortie = 600;
    
    if (!strcmp(argv[argc-1], "--help")) {
        fprintf(stderr, "Accepte jusqu'a 6 parametres :\n");
        fprintf(stderr, " fichier image, defaut : %s\n", tache.fichierImage);
        fprintf(stderr, " numero de l'algorithme, defaut : %d\n", algo);
        fprintf(stderr, " racine du nombre de blocs, defaut : %d\n", tache.racineNombreBlocs);
        fprintf(stderr, " taille de la zone de recouvrement, defaut : %d\n", tache.recouvrement);
        fprintf(stderr, " largeur de l'image de sortie, defaut : %d\n", tache.largeurSortie);
        fprintf(stderr, " hauteur de l'image de sortie, defaut : %d\n", tache.hauteurSortie);
        fprintf(stderr, "\nAlgorithmes :\n");
        fprintf(stderr, " 0 : bloc au hasard, raccord simple\n");
        fprintf(stderr, " 1 : bloc au hasard avec le permuteur, raccord simple\n");
        fprintf(stderr, " 2 : meilleur bloc, raccord simple\n");
        fprintf(stderr, " 3 : raccord avec coupe optimale (recursif naif)\n");
        fprintf(stderr, " 4 : raccord avec coupe optimale (recursif utilisable)\n");
        fprintf(stderr, " 5 : raccord avec coupe optimale (iteratif)\n");
        exit(0);
    }
    
    // traitement (simpliste) des arguments Unix
    if (argc>=2) tache.fichierImage = argv[1];
    if (argc>=3) algo = atoi(argv[2]);
    if (argc>=4) tache.racineNombreBlocs = atoi(argv[3]);
    if (argc>=5) tache.recouvrement = atoi(argv[4]);
    if (argc>=6) tache.largeurSortie = atoi(argv[5]);
    if (argc>=7) tache.hauteurSortie = atoi(argv[6]);
    if (argc >7) fprintf(stderr, "on ignore les arguments Unix au dela du 6eme\n");
    
    tache.choisirMeilleurBloc = true;
    tache.utiliserPermuteur = false;
    switch(algo){
        case 0:
            // sauf si le parametre de recouvrement a ete specifie, le mettre a 1
            if (argc<5) tache.recouvrement = 1;
            tache.choisirMeilleurBloc = false;
            tache.raccordeur = new RaccordeurSimple();
            break;
        case 1:
            tache.choisirMeilleurBloc = false;
            tache.utiliserPermuteur = true;
            tache.raccordeur = new RaccordeurSimple();
            break;
        case 2:
            tache.raccordeur = new RaccordeurSimple();
            break;
        case 3:
            //tache.raccordeur = new RaccordeurRecursifNaif();
            break;
        case 4:
          
            break;
        case 5:
            
            break;
        default:
            fprintf(stderr, "Valeur non geree pour algo : %d\n", algo);
            exit(-2);
    }
    textureur.traiterTache(&tache);
    return 0;
}
