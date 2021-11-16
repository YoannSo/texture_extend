#include"matrix2d.h"


// implementation

//#ifndef RAVI_INTERFACE

MatInt2::MatInt2(int d1,int d2)
{
  pdata = NULL;
  if(d1>0 && d2>0)
    {
      dim1=d1;
      dim2=d2;
      pdata=new int[dim1*dim2];
    }
}

MatInt2::~MatInt2()
{
  if (pdata) delete[]pdata;
}

int MatInt2::index(int n1, int n2) {return n1*dim2+n2;}

bool MatInt2::checkdim(int n1,int n2,const char* msg)
{
  if(n1>=0 && n1<dim1 && n2>=0 && n2<dim2)
    return true;
  else {
    if(msg)fprintf(stderr,
		   "Mauvais indice(s) MatInt2: %s (%d,%d) (dimensions: (%d,%d)\n",
		   msg,n1,n2,dim1,dim2);
    return false;
  }
}

int MatInt2::nLignes()
{
  return dim1;
}

int MatInt2::nColonnes()
{
  return dim2;
}

int MatInt2::get(int n1,int n2)
{ return checkdim(n1,n2,"Get")
    ?
    pdata[index(n1,n2)]
    : 0; }

void MatInt2::set(int n1,int n2, int val)
{
  if(checkdim(n1,n2,"set"))
     pdata[index(n1,n2)]=val;
}

void MatInt2::display()
{display(0,dim1-1,0,dim2-1);}

void MatInt2::display(int d1a,int d1b, int d2a,int d2b)
{
  if(checkdim(d1a,d2a,"display"))
    for(int n1=d1a;n1<=d1b&&n1<dim1;n1++)
      {
	fprintf(stdout,"%d : ",n1);
	for(int n2=d2a;n2<=d2b&&n2<dim2;n2++)
	  if(checkdim(n1,n2,NULL))
	    fprintf(stdout,"%d ",get(n1,n2));
	fprintf(stdout,"\n");
      }
}

MatDouble2::MatDouble2(int d1,int d2)
{
  pdata = NULL;
  if(d1>0 && d2>0)
    {
      dim1=d1;
      dim2=d2;
      pdata=new double[dim1*dim2];
    }
}

MatDouble2::~MatDouble2()
{
  if (pdata) delete[]pdata;
}

int MatDouble2::index(int n1, int n2) {return n1*dim2+n2;}

bool MatDouble2::checkdim(int n1,int n2,const char* msg)
{
  if(n1>=0 && n1<dim1 && n2>=0 && n2<dim2)
    return true;
  else {
    if(msg)fprintf(stderr,
		   "Mauvais indice(s) MatDouble2: %s (%d,%d) (dimensions: (%d,%d)\n",
		   msg,n1,n2,dim1,dim2);
    return false;
  }
}

int MatDouble2::nLignes()
{
  return dim1;
}

int MatDouble2::nColonnes()
{
  return dim2;
}

double MatDouble2::get(int n1,int n2)
{ return checkdim(n1,n2,"get")
    ?
    pdata[index(n1,n2)]
    : 0.0; }

void MatDouble2::set(int n1,int n2, double val)
{
  if(checkdim(n1,n2,"set"))
     pdata[index(n1,n2)]=val;
}

void MatDouble2::display()
{display(0,dim1-1,0,dim2-1);}

void MatDouble2::display(int d1a,int d1b, int d2a,int d2b)
{
  if(checkdim(d1a,d2a,"display"))
    for(int n1=d1a;n1<=d1b&&n1<dim1;n1++)
      {
	fprintf(stdout,"%d : ",n1);
	for(int n2=d2a;n2<=d2b&&n2<dim2;n2++)
	  if(checkdim(n1,n2,NULL))
	    fprintf(stdout,"%G ",get(n1,n2));
	fprintf(stdout,"\n");
      }
}


