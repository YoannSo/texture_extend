#include "image_4b.h"

// =======================================================
// Constructeur dependant de libtiff
//  lecture d'images en format tiff
//


Image_4b::Image_4b()
{
  
  width=-1;  // init utiles si echec lecture fichier
  height=-1;
  pdata=NULL;
  linePtr=NULL;
}

Image_4b::~Image_4b(){
  if (pdata != NULL) delete[] pdata;
  if (linePtr != NULL) delete[] linePtr; 
}

// initialisation

bool Image_4b::init(const char* filename){
  int w,h;
  TIFF* tif = TIFFOpen(filename,"r");
  if(tif>0){
    // fprintf(stderr,"TIFFOpen donne %x\n",(unsigned int)tif);
    TIFFGetField(tif,TIFFTAG_IMAGEWIDTH,&w);
    TIFFGetField(tif,TIFFTAG_IMAGELENGTH,&h);
  }else
    {    return false ;
    }
	
  // on a les dimensions - on alloue
  Alloc(w,h,NULL);
	
  // puis on lit le fichier
  int res= TIFFReadRGBAImage(tif,w,h,(uint32*)pdata,0);
  if(res<0){
    fprintf(stderr,"TIFFReadRGBAImage donne %d\n",res);
    return false ;
  }
  TIFFClose(tif);
  // on refait le resultat pour qu'il soit conforme a X11
  pixel_permute(pdata,w,h);
  FlipVertically1();
  //  printf("%d\n", GetRed(1,1));
  //  printf("%d\n", GetGreen(1,1));
  //  printf("%d\n", GetBlue(1,1));
  //  printf("%d\n", GetAlpha(1,1));

	
  return true ; 
}



void Image_4b::FlipVertically1 (void)
{
  unsigned char* tmp_store = new unsigned char[LineOffset];
  int endloop = height/2;
  
  for (int y1=0; y1<endloop; y1++)
    {
      int y2 = height-y1-1;
      memcpy(tmp_store, GetPixelPtr(0,y2), LineOffset);
      memcpy(GetPixelPtr(0,y2), GetPixelPtr(0,y1), LineOffset);
      memcpy(GetPixelPtr(0,y1), tmp_store, LineOffset);
    }
  delete [] tmp_store;
}

#ifndef LINUX
void pixel_permute(unsigned char * ima, int w, int h)
{
  // version ensibm: on passe de ABGR a BGRA
  unsigned char r,g,b,a;
  unsigned char* lim=w*h*4+ima;
  unsigned char* p;
  for(p=ima;p<lim;p+=4)
    {
      a=p[0];
      b=p[1];
      g=p[2];
      r=p[3];
      p[0]=b;
      p[1]=g;
      p[2]=r;
      p[3]=a;
    }
}
#else
void pixel_permute(unsigned char * ima, int w, int h)
//version pc: on passe de RGBA a BGRA
{
  unsigned char r,b;
  unsigned char* lim=w*h*4+ima;
  unsigned char* p;
  for(p=ima;p<lim;p+=4)
    {
      r=p[0];
      b=p[2];
      p[0]=b;
      p[2]=r;
    }
}
#endif

// fin de la partie dependant de libtif

Image_4b::Image_4b(int w,int h,unsigned char* b)
{
  pdata=NULL;
  linePtr=NULL;
  Alloc(w,h,b);
}

Image_4b::Image_4b(int x0,int y0,int w,int h,Image_4b* ima)
{
  int w1=ima->GetWidth(),
    h1=ima->GetHeight();

  pdata=NULL;  // init utiles si echec lecture fichier
  linePtr=NULL;

  if (x0<0 || w<=0 || x0+w >w1 || y0<0 || h<=0 || y0+h>h1)
    {fprintf(stderr,
	     "Mauvais parametres sous-image x0=%d,y0=%d,w=%d,h=%d,w1=%d,h1=%d\n",
	     x0,y0,w,h,w1,h1);
    return;}

  // allocate, then copy subimage from ima
  
  Alloc(w,h,NULL);
  for (int y1=0; y1<height; y1++)
    memcpy(GetPixelPtr(0,y1),ima->GetPixelPtr(x0,y0+y1),LineOffset);

  // Code with pixel sharing - not practical for display
  //  width=w;
  //  height=h;
  //  LineOffset=ima->GetLineOffset();
  //  linePtr = new unsigned char* [height];
  //   unsigned char ** tmp= linePtr;
  //   for (int i=0; i<height; i++) {
  //     *(tmp++)= ima->GetPixelPtr(x0,y0+i);
  //   }
  
}

void Image_4b::Alloc (int w, int h, unsigned char* b)
{
  // simpliste! tester w!=width || h!=height || b!=NULL
  if(pdata!=NULL) delete[] pdata;
  width=w;
  height=h;
  LineOffset=w*BytesPerPixel;
  if(b==NULL)
    pdata=new unsigned char [w*h*BytesPerPixel];
  else
    pdata=b;
  InitLinePtr();
}

void Image_4b::InitLinePtr(void)
{
  if (linePtr != NULL)
    delete [] linePtr;
  linePtr = new unsigned char* [height];
  
  unsigned char* v= pdata;
  unsigned char ** tmp= linePtr;
  for (int i=0; i<height; i++) {
    *(tmp++)= v;
    v+= LineOffset;
  }
}

double Image_4b::distPix(int x1,int y1,int x2,int y2)
{
  double difRed=fabs(GetRed(x1,y1)-GetRed(x2,y2));
  double difGreen=fabs(GetGreen(x1,y1)-GetGreen(x2,y2));
  double difBlue=fabs(GetBlue(x1,y1)-GetBlue(x2,y2));
  return sqrt(difRed*difRed + difGreen*difGreen + difBlue*difBlue);

}

double Image_4b::distA(int col1,int lig1,int col2,int lig2,int w,int h)
{
  if(checkdim(col1,lig1,"methode distA, point 1")
     && checkdim(col2,lig2,"methode distA, point 2")
     && checkdim(col1+w-1,lig1+h-1,"methode distA, point 1bis")
     && checkdim(col2+w-1,lig2+h-1,"methode distA, point 2bis"))
    {
      double res=0.0;
      int x,y;
      for(y=0;y<h;y++)
	for(x=0;x<w;x++)
	  res+=distPix(col1+x,lig1+y,col2+x,lig2+y);
      return res;
    }
  else
    {
      fprintf(stdout,"checkdim echoue pour distA (%d,%d), (%d,%d),w=%d,h=%d\n",
	      col1,lig1,col2,lig2,w,h);
      return -1.0;
    }
}


bool Image_4b::checkdim(int x,int y,const char* where)
{
  if (x>=0 && x<width && y>=0 && y<height)
    return true;
  else {
    fprintf(stderr,"Mauvais indice(s) dans %s(%d,%d) (width=%d,height=%d)\n",
	    where,x,y,width,height);
    return false;
  }
}

