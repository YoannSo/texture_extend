#include "textureur.h"

// ======================
//  Constructeur
// ======================
Textureur::Textureur() {
  
  im_texture = NULL;
  permuteur = NULL;
    
  // Image de texture
  tex_w=-1;       // largeur
  tex_h=-1;       // hauteur
	
  // Image resultat
  res_w=-1;    // largeur
  res_h=-1;    // hauteur
  	
  // les blocs
  bloc_fac=4;          // fraction de l'image de texture pour un bloc
  nb_bloc=bloc_fac*bloc_fac;  // nombre total de blocs
  
  // largeur de la zone de recouvrement
  TRecouv=8;
  TRecouv2=TRecouv/2;   // utile

  // parametres concernant le bord: normalement, largeur bord = TRecouv2
  bord=0;
  
  // resultat: nombre de blocs (donnees), et dimensions de l'image (calculees)
  res_hori=8;  // nombre de blocs en largeur - horizontale
  res_verti=6;  // et en hauteur - verticale
}

// =======================
// Destructeur
// =======================

Textureur::~Textureur(){
  if (im_texture) delete im_texture;
  if (permuteur) delete permuteur;
}


void Textureur::traiterTache(DescripteurTache* tache)
{
    if (tache->raccordeur == NULL) {
        fprintf(stderr, "Raccordeur non fourni\n");
        exit(-3);
    }
    init(*tache);
    doAlgo(tache->raccordeur);
}

// ============================
// Initialisation du textureur
//=============================

void Textureur::init(DescripteurTache& tache) {

    bloc_fac = tache.racineNombreBlocs;
    TRecouv = tache.recouvrement;
    TRecouv2 = TRecouv/2;
    bord = TRecouv2;
    const char* fichier = tache.fichierImage;
    utiliserPermuteur = tache.utiliserPermuteur;
    choisirMeilleurBloc = tache.choisirMeilleurBloc;
	
  // parametres dependants
  nb_bloc=bloc_fac*bloc_fac;  // nombre total de blocs

      if (utiliserPermuteur) permuteur = new Permuteur(nb_bloc);

  // Gestion de l'image de texture
  im_texture = new Image_4b();
  bool resu = im_texture->Image_4b::init(fichier); 
  if (!resu) {
    delete im_texture ;
    exit(-1) ;  
  }
  tex_w=im_texture->GetWidth();
  tex_h=im_texture->GetHeight();
  fprintf(stderr,"tex_w=%d,tex_h=%d\n",tex_w,tex_h);
  if(tex_w<=0) exit(-1);

  // Affichage de l'image de texture
  X11Display* tex_screen = new X11Display(tex_w,tex_h,"Image texture");
  fprintf(stderr,"point20\n");
  tex_screen->DisplayImage(im_texture->GetLinePtr(0),tex_w,tex_h);
  // tex_screen->Flush();  // peut provoquer SEGFAULT -:-()
  fprintf(stderr,"point21 (on doit voir l'image %s)\n",fichier);

  printf("cliquer dans l'image  pour continuer\n");
  tex_screen->Click();

  fprintf(stderr,"point22\n");
  	
  // Calcul de la taille des sous-blocs
  // On divise l'image de texture en bloc_fac dans chaque dimension

  int min_dim=tex_w>tex_h ? tex_h : tex_w;
  bloc_w=bloc_h=min_dim / bloc_fac;
  bloc_uw=bloc_w-2*bord;
  bloc_uh=bloc_h-2*bord;

  fprintf(stderr,"bloc_fac=%d, TRecouv=%d, TRecouv2=%d,res_hori=%d, res_verti=%d, bord=%d\n",
	  bloc_fac,TRecouv,TRecouv2,res_hori,res_verti,bord);

  fprintf(stdout,"tex_w=%d,tex_h=%d,bloc_w=%d,bloc_h=%d,bloc_uw=%d,bloc_uh=%d\n",
	  tex_w,tex_h,bloc_w,bloc_h,bloc_uw,bloc_uh);

  if(bloc_w<TRecouv)
    {
      fprintf(stderr,"Mauvaise combinaison bloc_w - TRecouv: %d - %d\n",
	      bloc_w,TRecouv);
      exit(-1);
    }
  // Construction de la "table des blocs"
  // avec les coordonnees du PSG de chaque bloc
  table_blocs = new bloc[nb_bloc];   // allocation dynamique en C++
  int i, j;
  for(i = 0; i < bloc_fac; i++)           // colonne i
    for(j = 0; j < bloc_fac; j++)         // ligne j
    {
	table_blocs[j + bloc_fac*i].x0 = j*bloc_w;
	table_blocs[j + bloc_fac*i].y0 = i*bloc_h;
    }
 
  // nombres de blocs en sorties
  res_hori = (tache.largeurSortie) / (bloc_uw);
  res_verti = (tache.hauteurSortie) / (bloc_uh);
  

  // Allocation du tableau des blocs resultat
  res_bindex = new MatInt2(res_hori,res_verti);
  // et des tableaux de distance
  tab_Dist_GD = new MatDouble2(TRecouv,bloc_h);
  tab_Dist_HB = new MatDouble2(bloc_w,TRecouv);

  tab_Chemin_GD = new MatDouble2(TRecouv,bloc_h);
  tab_Chemin_HB = new MatDouble2(bloc_w,TRecouv);

  coupe_opt = new int[bloc_h];   // attention: ou bloc_w!

  // l'image resultat: calcul des dimensions
  res_w = bloc_uw * res_hori;
  res_h = bloc_uh * res_verti;

  fprintf(stdout,"Dimensions image resultat: %d %d\n",res_w,res_h);
	
  // Gestion de l'image resultat
  im_res = new Image_4b(res_w,res_h);
  // fenetre d'affichage pour le resultat
  screen = new X11Display(res_w,res_h,"Image resultat");

}
//======================================== 
//
// ======================================== 

// Procedure de recopie d'image 
// param?tres : src :	pointeur sur Image_4b source
//		dest:	pointeur sur Image_4b destination
//		dest_x : largeur ? recopier
//		dest_y : hauteur ? recopier 


bool Textureur::copy2window(Image_4b& src,Image_4b& dest,int dest_x,int dest_y)
{
  printf("on entre dans copy2window, %d %d \n",dest_x,dest_y);
  int src_w=src.GetWidth(),
    src_h=src.GetHeight();
  if(dest_x<0 && dest_x+src_w>dest.GetWidth() &&
     dest_y<0 && dest_y+src_h>dest.GetHeight())
  {
	  fprintf(stderr,"copy2window: mauvais arguments, %d%d%d%d\n",
	     src_w,src_h,dest_x,dest_y);
    return false;
  }
  for(int y=0;y<src_h;y++)
    memcpy(dest.GetLinePtr(dest_y+y)+dest_x*BytesPerPixel,
	   src.GetLinePtr(y),src.GetLineOffset());
  return true;
}

// Copie d'image ? image
// Copy window to window (linewise, should not overlap)
// void *memcpy(void *dest, const void *src, size_t n);
// param?tres : 
//	src : image source 
//	src_x = coordonn?e x du pixel correspondant au coin haut gauche de la zone ? copier 
//	src_y = coordonn?e y du pixel correspondant au coin haut gauche de la zone ? copier
//	w = largeur de la zone ? copier
//	h = hauteur de la zone ? copier
//      dest : image destination
//	dest_x = coordonn?e x du pixel correspondant au coin haut gauche de la zone destination
//	dest_y = coordonn?e y du pixel correspondant au coin haut gauche de la zone destination

bool Textureur::win2win(Image_4b& src,int src_x,int src_y,int w,int h,
			Image_4b& dest,int dest_x,int dest_y)
{
  int ll=w*BytesPerPixel;   // nombre d'octets a copier par ligne

  /*  fprintf(stdout,"win2win: src=(%d,%d), w h=(%d,%d), dest=(%d,%d)\n",
      src_x,src_y,w,h,dest_x,dest_y);
  */
  if(src.checkdim(src_x,src_y,"win2win-src-1")
     && src.checkdim(src_x+w-1,src_y+h-1,"win2win-src-2")
     && dest.checkdim(dest_x,dest_y,"win2win-dest-1")
     && dest.checkdim(dest_x+w-1,dest_y+h-1,"win2win-dest-2"))
  {
      for(int y=0;y<h;y++)  // copie par ligne
	memcpy(dest.GetLinePtr(dest_y+y)+dest_x*BytesPerPixel,
	       src.GetLinePtr(src_y+y)+src_x*BytesPerPixel,
	       ll);
  }
  else {
	  fprintf(stderr,"win2win - problem with %d,%d,%d,%d,%d,%d\n",
		src_x,src_y,w,h,dest_x,dest_y);
  	  return false;
  }
  return true;
}

// ============================================================
// placement du bloc b de im_texture "avec bord"en position (c,l) dans l'image resultat point?e par im_res 
// ============================================================
// parametres : b : numero du bloc
//		c : coordonnee x du bloc resultat
//		l : coordonnee y du bloc resultat


void Textureur::placer_avec_bord(int b,int c,int l)
{
  fprintf(stdout,"placer b %d en (c=%d,l=%d)\n",b,c,l);
  bloc* bb = & table_blocs[b];
  
  if (!win2win(*im_texture,bb->x0+bord,bb->y0+bord,
	       bloc_uw,bloc_uh,
	       *im_res,c*bloc_uw,l*bloc_uh))
    fprintf(stdout,"erreur dans placer_avec_bord");
}

// ===================================
// Recherche de la coupe optimale
// ===================================

// Calcul du tableau de distances-entre-pixels
// bord-droit du bloc b1, bord-gauche du bloc b2
// les pixels concernes sont les memes que pour dist_GD
int Textureur::Coupe_GD(Raccordeur* raccordeur, bloc* b1, bloc* b2, int* coupe){

    int col1 = b1->x0+bloc_w-TRecouv;
    int lig1 = b1->y0;
    int col2 = b2->x0;
    int lig2 = b2->y0;
    
    MatInt2 zone(bloc_h, TRecouv);
    for (int y=0; y<bloc_h; y++) {
        for (int x=0; x<TRecouv; x++) {
            zone.set(y, x, im_texture->distPix(col1+x,lig1+y,col2+x,lig2+y));
        }
    }
    return raccordeur->calculerRaccord(&zone, coupe);
}

void Textureur::ajuster_coupe_GD(int c, int l, int* coupe){

  //fprintf(stdout,"ajuster_coupe_GD avec (c=%d,l=%d)\n",c,l);
  int nb1=res_bindex->get(c-1,l);
  int nb2=res_bindex->get(c,l);

  bloc* b1 = & table_blocs[nb1];
  int col1=b1->x0+bloc_w-TRecouv2;
  bloc* b2 = & table_blocs[nb2];

  int col_g=c*bloc_uw-TRecouv2;   // col G de la zone de recouvrement dans b1
  int col_m=c*bloc_uw;            // col milieu de la zone de recouvrement
  int dest_y=l*bloc_uh;           // cf. placer_avec_bord
  int i;

  for(i = TRecouv2; i < bloc_h - TRecouv2; i++)
  {
      if(coupe[i] < TRecouv2)
	// on recopie un bout de b2
	win2win(*im_texture,
		b2->x0+coupe[i],b2->y0+i,
		TRecouv2-coupe[i],1,
		*im_res,
		col_g+coupe[i],dest_y+i-TRecouv2);
      else
	// on recopie un bout de b1
	win2win(*im_texture,
		col1,b1->y0+i,
		coupe[i]-TRecouv2,1,
		*im_res,
		col_m,dest_y+i-TRecouv2);
  }
}

int Textureur::Coupe_HB(Raccordeur* raccordeur, bloc* b1, bloc* b2, int* coupe){

    
    int col1 = b1->x0;
    int lig1 = b1->y0 + bloc_h - TRecouv;
    int col2 = b2->x0;
    int lig2 = b2->y0;

    MatInt2 zone(bloc_w, TRecouv);
    for (int x = 0; x < bloc_w; x++) {
        for (int y = 0; y < TRecouv; y++) {
            // transpose la matrice des distances en la remplissant
            zone.set(x, y, im_texture->distPix(col1+x, lig1+y, col2+x, lig2+y));
        }
    }
    return raccordeur->calculerRaccord(&zone, coupe);
}

void Textureur::ajuster_coupe_HB(int c, int l, int* coupe){

  //fprintf(stdout,"ajuster_coupe_HB avec (c=%d,l=%d)\n",c,l);
  int nb1 = res_bindex->get(c,l-1);
  int nb2 = res_bindex->get(c,l);

  bloc* b1 = & table_blocs[nb1];
  int lig1 = b1->y0 + bloc_h - TRecouv2;
  bloc* b2 = & table_blocs[nb2];

  int lig_g = l*bloc_uh - TRecouv2;   // col G de la zone de recouvrement dans b1
  int lig_m = l*bloc_uh;            // col milieu de la zone de recouvrement
  int dest_x = c*bloc_uw;           // cf. placer_avec_bord
  int i;

  for(i = TRecouv2; i < bloc_w - TRecouv2; i++)
  {
      if(coupe[i] < TRecouv2)
	// on recopie un bout de b2
	win2win(*im_texture,
		b2->x0+i, b2->y0+coupe[i],
		1, TRecouv2-coupe[i],
		*im_res,
		dest_x+i-TRecouv2, lig_g+coupe[i]);
      else
	// on recopie un bout de b1
	win2win(*im_texture,
		b1->x0+i,lig1,
		1, coupe[i]-TRecouv2,
		*im_res,
		dest_x+i-TRecouv2, lig_m);
  }
}

int Textureur::randomBlockIndex() {
    if (utiliserPermuteur) {
        return permuteur->suivant();
    } else {
        return random() % nb_bloc;
        // ceci n'est pas la meilleur facon de faire un random (voir tools.h)
    }
}

// =====================================================
// Recherche du bloc optimal
// =====================================================

// indice du bloc optimal par rapport au voisin haut - voisin gauche
// la valeur -1 code "pas de voisin"
// parametres : vg : numero de bloc gauche
//              vh : numero de bloc haut
int Textureur::bestBlockIndex(Raccordeur* raccordeur, int vg, int vh) {
    int coutMin = INT_MAX;
    int res = -1;
    int voisinage = 0;
    if (vh == -1)
        if (vg == -1) {
            fprintf(stdout, "bestBlockIndex - deux voisins -1\n");
            return 0;
        }
        else voisinage = 1;
    if (vg==-1) voisinage = 2;
    
    int *coupegd = new int[bloc_h];
    int *coupehb = new int[bloc_w];
    
    for (int i=0; i<nb_bloc; i++) {
        int cout;
        switch (voisinage) {
            case 0: {
                cout= Coupe_HB(raccordeur, &table_blocs[vh], &table_blocs[i], coupehb) +
                        Coupe_GD(raccordeur, &table_blocs[vg], &table_blocs[i], coupegd);
                break;
            }
            case 1:       // voisin gauche seulement
            {
                cout = Coupe_GD(raccordeur, &table_blocs[vg], &table_blocs[i], coupegd);
                break;
            }
            case 2:       // voisin haut seulement
            {
                cout = Coupe_HB(raccordeur, &table_blocs[vh], &table_blocs[i], coupehb);
                break;
            }
        }  // switch
        if (cout < coutMin) {
            coutMin = cout;
            res = i;
        }
    }     // for
    delete[] coupehb;
    delete[] coupegd;
    //  fprintf(stdout,"resultat minDbloc = %d\n",res);
    return res; 
}

void Textureur::doAlgo(Raccordeur* raccordeur) {
    // premier bloc choisi au hasard
    int c, l;
    for (l = 0;l < res_verti; l++) { // indice ligne - progression par bloc
        for (c = 0; c < res_hori; c++) { // indice colonne
            int ind;
            if (choisirMeilleurBloc) {
                int vg = c>0 ? res_bindex->get(c-1, l) : -1;
                int vh = l>0 ? res_bindex->get(c, l-1) : -1;
                ind = bestBlockIndex(raccordeur, vg, vh);
            } else {
                ind = randomBlockIndex();
            }
            // fprintf(stderr, "xr=%d,yr=%d,bloc= %d\n", c, l, ind);
            res_bindex->set(c, l, ind);
            placer_avec_bord(ind, c, l);
            if (c > 0) {
                int *coupe = new int[bloc_h];
                Coupe_GD(raccordeur, &table_blocs[ind], &table_blocs[res_bindex->get(c-1, l)], coupe);
                ajuster_coupe_GD(c, l, coupe);
                delete[] coupe;
            }
            if (l > 0) {
                int *coupe = new int[bloc_w];
                Coupe_HB(raccordeur, &table_blocs[ind], &table_blocs[res_bindex->get(c, l-1)], coupe);
                ajuster_coupe_HB(c, l, coupe);
                delete[] coupe;
            }
        }
    }
    screen->DisplayImage(im_res->GetLinePtr(0), res_w, res_h);
    printf("c'est tout - cliquer dans l'image resultat pour finir\n");
    screen->Click();
    screen->DisplayImage(im_res->GetLinePtr(0), res_w, res_h);
    printf("c'est tout - cliquer dans l'image resultat pour finir\n");
    screen->Click();
}

