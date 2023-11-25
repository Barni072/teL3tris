tetris: main.o affichage.o grille.o entree.o tetrominos.o
	g++ main.o affichage.o grille.o entree.o tetrominos.o -o tetris -lSDL2
main.o: main.cpp affichage.cpp affichage.h grille.cpp grille.h entree.cpp entree.h
	g++ -c main.cpp
affichage.o: affichage.cpp affichage.h grille.cpp grille.h tetrominos.cpp tetrominos.h
	g++ -c affichage.cpp
grille.o: grille.cpp grille.h tetrominos.cpp tetrominos.h
	g++ -c grille.cpp
entree.o: entree.cpp entree.h grille.cpp grille.h
	g++ -c entree.cpp
tetrominos.o: tetrominos.cpp tetrominos.h
	g++ -c tetrominos.cpp
clean:
	rm -f *.o
	rm -f tetris
