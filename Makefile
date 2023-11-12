tetris: main.o affichage.o grille.o entree.o
	g++ main.o affichage.o grille.o entree.o -o tetris -lSDL2
main.o: main.cpp affichage.cpp affichage.h grille.cpp grille.h entree.cpp entree.h
	g++ -c main.cpp
affichage.o: affichage.cpp affichage.h grille.cpp grille.h
	g++ -c affichage.cpp 
grille.o: grille.cpp grille.h
	g++ -c grille.cpp
entree.o: entree.cpp entree.h grille.cpp grille.h
	g++ -c entree.cpp
clean:
	rm -f *.o
	rm -f tetris
