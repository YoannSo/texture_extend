// 
// Created by Augustin Lux Septembe 2003
//  from Ravi Module X11Display cf. www-prima.imag.fr/Ravi
//   which was based on code written by
//    Jerome Martin and Christophe LeGal
//

#ifndef _UX11Display_h_
#define _UX11Display_h_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <sys/resource.h>
// #include <sys/shm.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
// #include <X11/extensions/XShm.h>
// #include <X11/keysym.h>

// #include <PrimaVision/USuperClass.hh>

// #include <PrimaVision/config.h>
// static variables for X-connection
// single connection for all windows - else encapsulate in class

static Window rootWin ;
static int screen ;
static Display *dpy = NULL;
static GC gc ;
static Visual *visual ;
static int depth ;

// a few functions to facilitate X interface

// XOpenDisplay
void TTrueColor24Open(const char * name);

// XImage structure
XImage* make_XImage(unsigned char*,int,int);
void set_ximage_data(XImage* xim,unsigned char* dta);
void set_ximage_data(XImage* xim,unsigned char* dta,int w,int h);

// color values
int TrueColor24(char*);
// int TrueColor24(int,int,int);

// the important stuff comes now

class X11Display
{
public:
  X11Display(int W, int H, const char *titre);
  X11Display(int W, int H, const char *titre, int x, int y);
  //  X11Display(Window, Display*, GC);
  ~X11Display(void);

  // graphics
  void DrawPoint(int x1, int y1);
  void DrawCross(int x1, int y1, int s);
  void DrawLine(int x1, int y1, int x2, int y2);
  void DrawRectangle(int x1, int y1, int w, int h);
  void FillRectangle(int x1, int y1, int x2, int y2);
  void DrawArc(int x1, int y1, int w, int h,int a1,int a2);
  void DrawEllipse(int x1, int y1, int w, int h);
  void DrawEllipse(int x1, int y1, int w, int h, float theta);
  void DrawCircle(int x1, int y1, int r);
  void DrawString (const char * st, int x, int y);
  void DrawImageString (const char * st, int x, int y);
  Font LoadFont(char*);
  XFontStruct* LoadQueryFont(char*);
  void SetFont(Font);
  int SetForeground(char*);
  int SetForeground(int);
  int SetColor(char *);   // synonyme for SetForeground
  //  int SetForeground(int r, int g, int b);
  void SetBackground(char*);
  //  void SetBackground(int r,int g,int b);
  void SetWindowBackground(char*);
  //  void SetWindowBackground(int r,int g,int b);
  int SetFunction (int);

  // image display and window manipulation
  void Lower();
  void Raise(Window w=0);
  void ClearWindow();
  void ClearArea(int x,int y,int w,int h);
  void Resize(int width,int height);
  Window WindowWithName(char *name, Window top=0) ;

  void DisplayImage(void);
  void DisplayImage(unsigned char *);
  void DisplayImage(unsigned char *,int w,int h);
  void DisplayImage(unsigned char* data,int dst_x,int dst_y,int w,int h);

  void DisplayImage(XImage *,int src_x,int src_y,int dst_x,int dst_y,
		    int w,int h);

  // copy of complete ximage
  bool MakeCopy(int,int);  // border width: use 3,20 with fvwm
  XImage* GetCopy();
  void DisplayCopy();
  /*Ravi declare ((result-mode #t)) */
  unsigned char* GetCopyData(int& pw,int& ph);
  /*Ravi declare ((result-mode #f)) */
  //  void PutPixel(int x,int y,int r,int g, int b);
  void PutPixel(int x,int y,int val);

  // interaction
  void Click(int f=1);
  void ClickP(int f=1);
  void ClickLine(int f=1);

  void SetInputFocus(int);

  // Synchronization
  void Flush(void);
  void Sync(int);

  // access
  int GetWidth (void);
  int GetHeight (void);

  //  char * ObjectInfo (void);

  unsigned char * GetImagedata (void);
  XImage * GetXimage (void);
  void SetXimage (XImage *);

  char * GetName (void);
  void SetName (const char *);

  // access to globals
  Display * GetDisplay (void);
  Window GetRootwindow (void);
  int GetScreen (void);
  Visual * GetVisual (void);
  int GetDepth (void);
  GC GetGc (void);
  Window GetWindow (void);

protected:
  X11Display(void);
  void close (void);
  void open(int W, int H, const char *titre, int x, int y);
  
private:
  Window win;
  int width, height;
  XImage *ximage, *screencopy;
  char * name_;
  int foreground;
  int xfunct;
};

inline XImage * X11Display::GetXimage (void) { return ximage; }

inline void X11Display::SetXimage (XImage * i) { ximage = i; }

// inline int TrueColor24(int r,int g,int b)
// {return (r << RED_SHIFT) | (g << GREEN_SHIFT) | b << BLUE_SHIFT;}

inline GC X11Display::GetGc (void) {return gc;}

inline int X11Display::GetWidth (void) { return width; }

inline int X11Display::GetHeight (void) { return height; }

inline Display * X11Display::GetDisplay (void) { return dpy; }

inline Window X11Display::GetRootwindow (void) { return rootWin; }

inline Window X11Display::GetWindow (void) { return win; }

inline char * X11Display::GetName (void) { return name_; }

inline unsigned char * X11Display::GetImagedata (void)
{ return (unsigned char *)(ximage->data); }

inline int X11Display::GetScreen (void) { return screen; }

inline Visual * X11Display::GetVisual (void) { return visual; }

inline int X11Display::GetDepth (void) { return depth; }

inline void X11Display::DrawPoint(int x1, int y1)
{XDrawPoint(dpy, win, gc, x1, y1) ;}
 
inline void X11Display::DrawCross(int x1, int y1, int s) 
{
  DrawLine (x1-s,y1,x1+s,y1);
  DrawLine (x1,y1-s,x1,y1+s);
}

inline void X11Display::DrawLine(int x1, int y1, int x2, int y2)
{XDrawLine(dpy, win, gc, x1, y1, x2, y2) ;}

inline void X11Display::DrawRectangle(int x1, int y1, int w, int h)
{XDrawRectangle(dpy, win, gc, x1, y1, w, h) ;}

inline void X11Display::FillRectangle(int x1, int y1, int x2, int y2)
{XFillRectangle(dpy, win, gc, x1, y1, x2, y2) ;}

inline void X11Display::DrawArc(int x1, int y1, int w, int h,int a1,int a2)
{XDrawArc(dpy, win, gc, x1, y1, w, h,a1,a2);}

inline void X11Display::DrawEllipse(int x1, int y1, int w, int h)
{
  XDrawArc(dpy, win, gc, x1, y1, w, h,0,360*64);
}
inline void X11Display::DrawEllipse(int x1, int y1, int w, int h, float theta)
{
  int nPoints = 73;
  XPoint points[nPoints];
  
  for (int i = 0; i < nPoints - 1; i++) {
    float t = i * 2 * M_PI / nPoints;
    float xt = w * cos(t);
    float yt = h * sin(t);
    points[i].x = static_cast<short>(x1 + xt*cos(theta) - yt*sin(theta) + 0.5);
    points[i].y = static_cast<short>(y1 + xt*sin(theta) + yt*cos(theta) + 0.5);
  }
  points[nPoints - 1] = points[0];
  
  XDrawLines(dpy, win, gc, points, nPoints, CoordModeOrigin);
}

inline void X11Display::DrawCircle(int x1, int y1, int r)
{XDrawArc(dpy,win,gc,x1-r,y1-r,2*r,2*r,0,360*64);}

inline void X11Display::DrawString (const char * st, int x, int y)
{XDrawString (dpy, win, gc, x, y, st, strlen(st));}

inline void X11Display::DrawImageString (const char * st, int x, int y)
{XDrawString (dpy, win, gc, x, y, st, strlen(st));}

inline Font X11Display::LoadFont(char* name) {return XLoadFont(dpy,name);}

inline XFontStruct* X11Display::LoadQueryFont(char* name)
{return XLoadQueryFont(dpy,name);}

inline void X11Display::SetFont(Font fn) {XSetFont(dpy,gc,fn);}

// inline void X11Display::SetBackground(int r,int g,int b)
// {XSetBackground(dpy,gc,TrueColor24(r,g,b));}

// inline void X11Display::SetWindowBackground(int r,int g,int b)
// {XSetWindowBackground(dpy,win,TrueColor24(r,g,b));}

inline void X11Display::DisplayCopy()
{if(screencopy)XPutImage(dpy,win,gc,screencopy,0,0,0,0,
			 screencopy->width, screencopy->height);}

inline XImage* X11Display::GetCopy() {return screencopy;}

// inline void X11Display::PutPixel(int x,int y,int r,int g, int b)
//   {PutPixel(x,y,TrueColor24(r,g,b));}

// inline void X11Display::PutPixel(int x,int y,int val)
// {if(screencopy)XPutPixel(screencopy,x,y,val);}

inline int X11Display::SetFunction (int f)
{int r=xfunct; xfunct=f;XSetFunction (dpy, gc, f);return r;}

inline int X11Display::SetForeground(int v)
{int res=foreground;
  foreground=v;
  XSetForeground(dpy, gc,v);
  return res;}

// inline int X11Display::SetForeground(int r, int g, int b)
// {return SetForeground(TrueColor24(r,g,b));}

inline int X11Display::SetForeground(char* c)
{return SetForeground(TrueColor24(c));}

inline int X11Display::SetColor(char* c) {return SetForeground(c);}
 
inline void X11Display::SetBackground(char* name)
{XSetBackground(dpy,gc, TrueColor24(name));} 

inline void X11Display::SetWindowBackground(char* name)
{XSetWindowBackground(dpy,win, TrueColor24(name));}

inline  void X11Display::SetInputFocus(int i)
{XSetInputFocus(dpy,i==0 ? GetRootwindow() : win ,RevertToParent,CurrentTime);}

//XGrabKeyboard(dpy,win,False,GrabModeAsync,GrabModeAsync,CurrentTime);}

inline void X11Display::Flush() {XFlush(dpy) ;}

inline void X11Display::Sync(int i) {XSync(dpy,i) ;} 

inline void X11Display::Lower() {XLowerWindow(dpy,win);}

inline void X11Display::ClearWindow() {XClearWindow(dpy,win);}

inline void X11Display::ClearArea(int x,int y,int w,int h)
{XClearArea(dpy,win,x,y,w,h,False);}

inline void X11Display::Raise(Window w) {
  if(!w) w=win ;
  XRaiseWindow(dpy,w);
}

inline void X11Display::DisplayImage(unsigned char *dta,int w,int h)
{DisplayImage(dta,0,0,w,h);}

#endif
