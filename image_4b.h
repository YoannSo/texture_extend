#ifndef _IMAGE_4B_H
#define _IMAGE_4B_H

// The following system dependent constants should be generated by configure

// Comment this line under non-linux systems (ensibm)
#define LINUX linux



#ifdef LINUX
// values are the same for enisbm and classic linux
// but loading tiff differs (in .cpp file)
#define RED_CHANNEL 2
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 0
#define ALPHA_CHANNEL 3
//#else
#endif

#include <cstdlib>
#include <stdio.h>
#include <tiff.h>
#include <tiffio.h>
#include <string.h>
#include <math.h>

const int BytesPerPixel=4;

class Image_4b{

public:
  // les constructeurs: depuis un fichier, image vide, sousimage
  Image_4b();
  Image_4b(int w,int h,unsigned char* b = NULL);
  Image_4b(int x0,int y0,int w,int h,Image_4b* ima);
  
  ~Image_4b();
  bool init(const char* filename) ;

  int GetRed(int x,int y);
  int GetGreen(int x,int y);
  int GetBlue(int x,int y);
  int GetAlpha(int x,int y);

  void SetRed(int x,int y,int val);
  void SetGreen(int x,int y,int val);
  void SetBlue(int x,int y,int val);
  void SetAlpha(int x,int y,int val);

  unsigned char* GetLinePtr(int y);
  unsigned char* GetPixelPtr(int x,int y);

  int GetWidth(void);
  int GetHeight(void);
  int GetLineOffset(void);

  // distance entre 2 pixels
  double distPix(int x1,int y1,int x2,int y2);
  // distance entre sous-images
  // chaque sous-image definie par: PSG - w - h
  double distA(int col1,int lig1,int col2,int lig2,int w,int h);

  // for internal use

  void Alloc (int w, int h, unsigned char* b);

  void FlipVertically1 (void);
  bool checkdim(int x,int y,const char* where);

private:
  int width, height;
  int LineOffset;           // d'une ligne a la suivante
  unsigned char * pdata;
  unsigned char ** linePtr;
  void InitLinePtr(void);
};

// auxiliary function (tiff problem)
void pixel_permute(unsigned char * ima, int w, int h);

// methodes inline

#ifndef RAVI_INTERFACE

inline int Image_4b::GetWidth (void) {return width;}
inline int Image_4b::GetHeight (void) {return height;}
inline int Image_4b::GetLineOffset(void) {return LineOffset;}
inline unsigned char * Image_4b::GetLinePtr (int i) {return linePtr[i];}
inline unsigned char * Image_4b::GetPixelPtr (int x, int y)
{ return checkdim(x,y,"GetPixelPtr")
    ? GetLinePtr(y) + x * BytesPerPixel
    : 0 ; }

inline int Image_4b::GetRed (int x, int y)
{ return checkdim(x,y,"GetRed")
    ?
    (int)*(GetLinePtr(y) + x * BytesPerPixel + RED_CHANNEL)
    : 0; }
inline void Image_4b::SetRed (int x, int y,int val)
{ if(checkdim(x,y,"SetRed"))
  *(GetLinePtr(y) + x * BytesPerPixel + RED_CHANNEL)=(unsigned char) val; }

inline int Image_4b::GetGreen (int x, int y)
{ return checkdim(x,y,"GetGreen")
    ?
    (int)*(GetLinePtr(y) + x * BytesPerPixel + GREEN_CHANNEL)
    : 0; }
inline void Image_4b::SetGreen (int x, int y,int val)
{ if(checkdim(x,y,"SetGreen"))
  *(GetLinePtr(y) + x * BytesPerPixel + GREEN_CHANNEL)=(unsigned char) val; }

inline int Image_4b::GetBlue (int x, int y)
{ return checkdim(x,y,"GetBlue")
    ?
    (int)*(GetLinePtr(y) + x * BytesPerPixel + BLUE_CHANNEL)
    : 0; }
inline void Image_4b::SetBlue (int x, int y,int val)
{ if(checkdim(x,y,"SetBlue"))
  *(GetLinePtr(y) + x * BytesPerPixel + BLUE_CHANNEL)=(unsigned char) val; }

inline int Image_4b::GetAlpha (int x, int y)
{ return checkdim(x,y,"GetAlpha")
    ?
    (int)*(GetLinePtr(y) + x * BytesPerPixel + ALPHA_CHANNEL)
    : 0; }
inline void Image_4b::SetAlpha (int x, int y,int val)
{ if(checkdim(x,y,"SetAlpha"))
  *(GetLinePtr(y) + x * BytesPerPixel + ALPHA_CHANNEL)=(unsigned char) val; }

#endif
#endif
