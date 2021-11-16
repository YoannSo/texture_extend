CXX=g++
CXXFLAGS=-Wall
LDFLAGS=

# XINCS = -I /usr/X11R6/include
XLIBS= -ltiff -L/usr/X11R6/lib -lX11
# les libs : -lX11 -ltiff

EXEC=tp testpermut
OBJECTS=main_tp.o image_4b.o matrix2d.o permuteur.o textureur.o x11display_code.o raccordeur_simple.o 

usage:
	@echo "Cible possibles :"
	@echo " make tp"
	@echo " make testpermut"
	@echo " make all"

all: $(EXEC)

testpermut: permuteur.o main_testpermut.o
	$(CXX) $(LDFLAGS) -o testpermut $^

tp: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o tp $^ $(XLIBS)

image_4b.o: image_4b.h
main_tp.o: matrix2d.h descripteur_tache.h raccordeur.h permuteur.h raccordeur_simple.h 
matrix2d.o: matrix2d.h
permuteur.o: permuteur.h tools.h
raccordeur_simple.o: raccordeur_simple.h raccordeur.h matrix2d.h
textureur.o: textureur.h matrix2d.h descripteur_tache.h raccordeur.h permuteur.h
x11display_code.o: x11display_code.h

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean: 
	rm -f $(EXEC) *.o

# Differents outils (peut important pour la compréhension)

#lib:
#	g++ -c -o image_4b.o image_4b.cpp
#	g++ -c -o x11display_code.o x11display_code.cpp
#	ar -rcvs libtpalgo.a image_4b.o x11display_code.o

tar:
	@echo "NOT IMPLEMENTED"
#tar -cf ../tpalgo.tar {image_4b,x11display_code}.{hh,cc} \
		textureur.{hh,cc} main_tp.cpp \
		matint2.{hh,cc} Makefile *.tif

scp:
	@echo "NOT IMPLEMENTED"
#scp src2007.tar lux@ensibm:/perms/lux/TPALGO/src.tar

