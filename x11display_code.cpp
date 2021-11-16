#include "x11display_code.h"

static XColor my_color;
static Colormap cmap ;

void TTrueColor24Open(const char * name)
{
  dpy = XOpenDisplay(name) ;
  if(dpy==NULL)
    {fprintf(stderr,"XOpenDisplay %s failed\n",name);
    return;}
  rootWin = DefaultRootWindow(dpy) ;
  screen = DefaultScreen(dpy) ;
  gc = DefaultGC(dpy, screen) ;
  cmap = DefaultColormap(dpy, screen) ;
  visual = XDefaultVisual(dpy, screen) ;
  depth = DefaultDepth(dpy,screen);
  if(depth!=24)fprintf(stderr,"running with screendepth %d\n",depth);
}

XImage* make_XImage(unsigned char* data,int w,int h)
{
  XImage* res=XCreateImage(dpy, visual, depth, ZPixmap, 1, NULL, w, h,
			   32, 0);
  res->data=(char*)data;
  return res;
}

void set_ximage_data(XImage* xim,unsigned char* dta)
{
  xim->data=(char*) dta;
}

void set_ximage_data(XImage* xim,unsigned char* dta,int w,int h)
{
  if((w!=xim->width) || (h!=xim->height))
    {
      xim->width=w;
      xim->height=h;
      xim->bytes_per_line=w*4;}
  xim->data=(char*) dta;
}

int TrueColor24(char* name)
{
  if(dpy==NULL)
    {
      fprintf(stderr,"TrueColor24: NULL display\n");
      return 0;
    }
  XParseColor(dpy,cmap,name,&my_color);
  if (!XAllocColor(dpy,cmap,&my_color))
    fprintf(stderr,"XAllocColor fails for %s\n",name);
  return (int)my_color.pixel;
}

X11Display::X11Display(void) : name_(NULL) {}

X11Display::X11Display(int W, int H, const char *titre) {open (W, H, titre,-1,-1);}

X11Display::X11Display(int W, int H, const char *titre, int x, int y) 
{open (W, H, titre, x, y);}

void X11Display::open(int W, int H, const char *titre, int x, int y) 
{
  XSetWindowAttributes attr ;
  XGCValues gcvals;
  XEvent myevent;

  if(dpy==NULL)TTrueColor24Open("");
  if(dpy==NULL) return;

  win = XCreateSimpleWindow(dpy, rootWin, x, y, W, H, 2, 0, 0) ;
  XSizeHints hints;
  hints.width      = W;
  hints.height     = H;
  hints.max_width  = 1536;
  hints.max_height = 1024;
  hints.min_width  = W;
  hints.min_height = H;
  hints.width_inc  = 0;
  hints.height_inc = 0;
  if (x >= 0)
    {
      hints.x          = x;
      hints.y          = y;   
      hints.flags = USSize | USPosition | PMinSize | PMaxSize | PResizeInc; 
    }
  else
    hints.flags = USSize | PMinSize | PMaxSize | PResizeInc; 

  XSetStandardProperties(dpy, win, titre, titre, None, NULL, 0, &hints);
  width = W ;
  height = H ;
  attr.event_mask = (ExposureMask);
  attr.backing_store = Always ;
  XChangeWindowAttributes(dpy, win, CWBackingStore | CWEventMask, &attr) ;

  XGetGCValues(dpy,gc,GCFunction | GCForeground,&gcvals);
  xfunct = gcvals.function;
  foreground = gcvals.foreground;
  screencopy=NULL;
  XMapWindow(dpy, win) ;

  name_=NULL;
  SetName (titre);
  if(depth==24)
    ximage = XCreateImage(dpy, visual, depth, ZPixmap, 1, NULL, width, height,
			  32, 0);
  else
    fprintf(stderr,
	    "\n\nmodule X11Display: no ximage created -screen depth %d\n\n",depth);
  // wait for window exposure before going on
  XNextEvent(dpy,&myevent);
  if(myevent.type != Expose)
    fprintf(stderr,
	    "\n\nFirst event not expose event %d\n\n",
	    myevent.type);
  else printf("exposure arrive\n");
  attr.event_mask = NoEventMask;
  XChangeWindowAttributes(dpy, win, CWEventMask, &attr) ;
}

X11Display::~X11Display()
{
  close ();
}

void X11Display::close (void)
{
  //  fprintf(stderr,"on ferme une fenetre\n");
  XUnmapWindow(dpy, win) ;
  XDestroyWindow(dpy, win) ;
  XFlush(dpy) ;  //necessary for immediate effect
  if (name_ != NULL) delete [] name_;
}

bool X11Display::MakeCopy(int b1,int b2)
  // b1,b2: border width. use 3,20 with fvwm
{
  /* for XGetGeometry */
  Window r1;
  int x,y;
  unsigned int bw,dp,w1,h1;
  int s;

  XImage* res;
  if(screencopy==NULL)
    {
      /*
      copydata = (unsigned char *)malloc(width*height*sizeof(int));
      screencopy = XCreateImage(dpy, visual, depth, ZPixmap, 1, (char*)copydata,
				width, height,32,0);
      */
      s=XGetGeometry(dpy,win,&r1,&x,&y,&w1,&h1,&bw,&dp);
      printf("geometrie: width %d, height %d, borderw %d (screen-w/h %d %d)\n",w1,h1,bw,width,height);
      res=screencopy=XGetImage(dpy,win,b1,b2,w1,h1,AllPlanes,ZPixmap);
      return res;
    }
  if(screencopy==NULL)
    {
      fprintf(stderr,"MakeCopy: XCreateImage failed\n");
      return false;
    }
  res=XGetSubImage(dpy,win,b1,b2,width,height,AllPlanes,ZPixmap,screencopy,0,0);
  return res?true:false;
}

unsigned char* X11Display::GetCopyData(int& pw,int& ph)
{
  pw=screencopy->width;
  ph=screencopy->height;
  return (unsigned char*) screencopy->data;
}

void X11Display::Click(int f)
{
  XEvent evt ;
  if(f)XSelectInput(dpy, win, ButtonPressMask) ;
  XMaskEvent(dpy,ButtonPressMask,&evt) ;
  if(f)XSelectInput(dpy, win,NoEventMask) ;
  XFlush(dpy);
  //  return Cons(ScFixNum(evt.xbutton.x), ScFixNum(evt.xbutton.y)) ;
}

void X11Display::ClickP(int f)
{
  XEvent evt ;
  if(f)XSelectInput(dpy, win, ButtonPressMask) ;
  XMaskEvent(dpy,ButtonPressMask,&evt) ;
  if(f)XSelectInput(dpy, win,NoEventMask) ;
//   return Cons(ScFixNum(evt.xbutton.x),
// 	      Cons(ScFixNum(evt.xbutton.y),
// 		   Cons(ScFixNum(evt.xbutton.button),
// 			Cons(ScFixNum(evt.xbutton.state),ScNil))));
}

void X11Display::ClickLine(int f)
{
  XEvent evt ;
  int x1,y1,x2,y2;
  int init=1;
  Window root,child;
  int root_x,root_y;
  int posx,posy;
  unsigned int kb;
  //  XGCValues gcvals;
  //  int savefn;
  //  XGetGCValues(dpy,gc,GCFunction,&gcvals);
  //  savefn=gcvals.function;

  if(f)XSelectInput(dpy, win, ButtonPressMask | ButtonReleaseMask
		    | ButtonMotionMask | PointerMotionHintMask) ;
  XSetFunction(dpy,gc,GXxor);

  // get initial point of line
  XMaskEvent(dpy,ButtonPressMask,&evt) ;
  x1=evt.xbutton.x;
  y1=evt.xbutton.y;

  // draw transient line to current mouse position
  for(XMaskEvent(dpy,ButtonReleaseMask | ButtonMotionMask,&evt);
      evt.type==MotionNotify;
      XMaskEvent(dpy,ButtonReleaseMask | ButtonMotionMask,&evt))
    // handle ButtonMotion
    {
      while(XCheckMaskEvent(dpy,ButtonMotionMask,&evt)); // skip pending motions
      if(XQueryPointer(dpy,win,&root,&child,&root_x,&root_y,
		      &posx,&posy,&kb))
	{
	  if(!init)
	    XDrawLine(dpy,win,gc,x1,y1,x2,y2);
	  else init=0;
	  if(evt.xmotion.window==win)
	    {
	      x2=posx;
	      y2=posy;
	      XDrawLine(dpy,win,gc,x1,y1,x2,y2);
	      XFlush(dpy);
	    }
	}
	  // else out of window
    }
  // for-loop exit on ButtonRelease defining second point
  x2=evt.xbutton.x;
  y2=evt.xbutton.y;
  
  if(f)XSelectInput(dpy, win,NoEventMask) ;
  XDrawLine(dpy,win,gc,x1,y1,x2,y2);
  XSetFunction(dpy,gc,xfunct);
//   return Cons(Cons(ScFixNum(x1),ScFixNum(y1)),
// 	      Cons(Cons(ScFixNum(x2),ScFixNum(y2)),
// 		   ScNil));
}

void X11Display::Resize(int w,int h)
{
  if((w>width) || (h>height))
    {XResizeWindow(dpy,win,w,h);
    width=w;
    height=h;
    if(screencopy) XDestroyImage(screencopy);
    }
}

void X11Display::DisplayImage(void)
{
  if (ximage->data != NULL)
    XPutImage(dpy,win,gc,ximage,0,0,0,0,ximage->width, ximage->height);
}

void X11Display::DisplayImage(unsigned char *dta){
  ximage->data = (char*)dta ;
  XPutImage(dpy,win,gc,ximage,0,0,0,0,ximage->width, ximage->height);
}

void X11Display::DisplayImage(unsigned char *dta,int dst_x,int dst_y,int w,int h)
{
  ximage->data = (char*)dta ;
  ximage->width=w;
  ximage->height=h;
  ximage->bytes_per_line=w*4;
  XPutImage(dpy,win,gc,ximage,0,0,dst_x,dst_y,w, h);
}

void X11Display::DisplayImage(XImage *xim, int src_x, int src_y,
				   int dst_x,int dst_y,int w, int h)
{
  XPutImage(dpy,win,gc,xim,src_x,src_y,dst_x,dst_y,w, h);
}

void X11Display::SetName(const char * n)
{
  if (name_ != NULL)
    delete [] name_;

  name_ = new char [strlen(n)+1];
  strcpy (name_, n);
  XChangeProperty(dpy, win, XA_WM_NAME, XA_STRING, 8, PropModeReplace,
		  (unsigned char *)n, strlen(n)) ;
}

//
// @CLASS
//   X11Display
// @NAME
//  ObjectInfo
// @SYNOPSIS
//  char* X11Display::ObjectInfo(void)
// @DESCRIPTION
//  imprime dans une chaine de characteres une information sur l'objet
//
// char * X11Display::ObjectInfo (void) 
// {
//   sprintf(objectInfoBfr, "<X11Display: %s %ix%i>", 
// 	  GetName(), GetWidth(), GetHeight());
//   return objectInfoBfr;
// }

//
// @CLASS
//   X11Display
// @NAME
//  WindowWithName
// @SYNOPSIS
//  Window X11Display::WindowWithName(char *name) ;
// @DESCRIPTION
//  Retourne l'id d'une fenetre dont le nom est name
//
Window X11Display::WindowWithName(char *name, Window top){
  Window *children, dummy;
  unsigned int nchildren;
  unsigned int i;
  Window w=0;
  char *window_name;

  if(!top) top=DefaultRootWindow(dpy) ;

  if (XFetchName(dpy, top, &window_name) && !strncmp(window_name, name, strlen(name))) 
     return top;

  if (!XQueryTree(dpy, top, &dummy, &dummy, &children, &nchildren)) return 0 ;

  for (i=0; i<nchildren; i++) {
    w = WindowWithName(name, children[i]);
    if (w) break;
  }
  if (children) XFree ((char *)children);
  return w ;
}
