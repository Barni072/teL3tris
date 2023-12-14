teLtr3is: main.o affichage.o grille.o entree.o tetrominos.o
	g++ main.o affichage.o grille.o entree.o tetrominos.o -o teLtr3is -lSDL2 -lSDL2_ttf
main.o: main.cpp affichage.h grille.h entree.h
	g++ -c main.cpp
affichage.o: affichage.cpp affichage.h grille.h tetrominos.h
	g++ -c affichage.cpp
grille.o: grille.cpp grille.h tetrominos.h
	g++ -c grille.cpp
entree.o: entree.cpp entree.h grille.h
	g++ -c entree.cpp
tetrominos.o: tetrominos.cpp tetrominos.h
	g++ -c tetrominos.cpp
clean:
	rm -f *.o
	rm -f teLtr3is
