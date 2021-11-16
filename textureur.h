#ifndef _TEXTUREUR_H
#define _TEXTUREUR_H

#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <float.h>
#include <limits>
#include <climits>

#include "image_4b.h"
#include "x11display_code.h"
#include "matrix2d.h"
#include "descripteur_tache.h"
#include "raccordeur.h"
#include "permuteur.h"

class Textureur {
public:
  Textureur();   //constructeur
  ~Textureur();  //destructeur
  
  /*!
   * Execute la tache fournie en parametre.
   */
  void traiterTache(DescripteurTache* tache);

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // Tout ce qui suit est interne a la classe et n'est pas important pour son
  // utilisation. Ce code reprend le code des versions precedentes. Beaucoup
  // de choses sont probablement de trop.
private:

  /*   Dimensions et parametres divers                 */
  /*   Valeurs constantes pendant une execution        */
  /*   Dependent des dimensions de l'image texture     */
  /*   et des dimensions voulues du resultat           */

  // Variables concernant les blocs
  int bloc_fac;   // fraction de l'image de texture pour un bloc (exemple : pour 1/4 de l'image de texture, bloc_fac=4)
   
  // largeur de la zone de recouvrement
  int TRecouv;
  int TRecouv2;   // utile

  // parametres bord: normalement, largeur bord = TRecouv2
  int bord;
  
  // resultat: nombre de blocs (donnee), dimensions de l'image (calculees)
  int res_hori;  // nombre de blocs en largeur - horizontale
  int res_verti;  // et en hauteur - verticale

  void init(DescripteurTache& tache);
  void doAlgo(Raccordeur* raccordeur);

  bool utiliserPermuteur;
  Permuteur* permuteur;
  bool choisirMeilleurBloc;

  int randomBlockIndex();
  int bestBlockIndex(Raccordeur* raccordeur, int vg, int vh);
  
  // Image de texture
  Image_4b* im_texture;
  int tex_w;       // largeur de l'image de texture
  int tex_h;       // hauteur de l'image de texture

  // Image resultat
  Image_4b* im_res;
  int res_w;    // largeur de l'image r?sultat
  int res_h;    // hauteur de l'image r?sultat
  
  // Pour les blocs:
  int nb_bloc;  // nombre total de blocs

  int bloc_w;   // Largeur d'un bloc
  int bloc_h;   // Hauteur d'un bloc

  /* Declarations bloc */
  struct bloc {int x0; int y0;};  // coordonnees du PSG (Point Superieur Gauche)

  bloc* table_blocs;    // table des descripteurs de blocs

  int bloc_uw;   // dimensions de la partie utile d'un bloc
  int bloc_uh;
 
  // tableau des numeros de blocs
  MatInt2* res_bindex;   // (res_hori,res_verti)

  // Matrices pour les calculs des distances
  MatDouble2* tab_Dist_GD;  // (TRecouv,bloc_h)
  MatDouble2* tab_Dist_HB;  // (bloc_w,TRecouv)

  // Matrices pour les calculs des plus courts chemins
  MatDouble2* tab_Chemin_GD;  // (TRecouv,bloc_h)
  MatDouble2* tab_Chemin_HB;  // (bloc_w,TRecouv)
  
  // Stockage de la coupe optimale
  int* coupe_opt;
	
  // Pour l'affichage
  X11Display* screen;
  
  // placer avec bord
  void placer_avec_bord(int b,int c,int l);
	
  // Pour le calcul avec la coupe optimale
  int Coupe_GD(Raccordeur* raccordeur, bloc* b1, bloc* b2, int* coupe);
  void ajuster_coupe_GD(int c, int l, int* coupe);

  int Coupe_HB(Raccordeur* raccordeur, bloc* b1, bloc* b2, int* coupe);
  void ajuster_coupe_HB(int c, int l, int* coupe);

  bool copy2window(Image_4b& src,Image_4b& dest,int dest_x,int dest_y);
  // copy window to window (linewise, should not overlap)
  bool win2win(Image_4b& src,int src_x,int src_y,int w,int h,
	       Image_4b& dest,int dest_x,int dest_y);

};

#endif // _TEXTUREUR_HH
