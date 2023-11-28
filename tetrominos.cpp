#include "tetrominos.h"

// Identifiants des différents tétrominos, et des couleurs de leurs blocs
const int O = 1;
const int I = 2;
const int T = 3;
const int S = 4;
const int J = 5;
const int Z = 6;
const int L = 7;
const int GRIS = 8;
const int VIDE = 0;		// Signifie : "Il n'y a rien ici"

/* Contient les positions de départ des tétrominos
 * À ne surtout pas utiliser directement */
const int TETROMINOS[448] = {
	0,0,0,0,
	0,O,O,0,
	0,O,O,0,
	0,0,0,0,
	
	0,0,0,0,
	0,O,O,0,
	0,O,O,0,
	0,0,0,0,
	
	0,0,0,0,
	0,O,O,0,
	0,O,O,0,
	0,0,0,0,
	
	0,0,0,0,
	0,O,O,0,
	0,O,O,0,
	0,0,0,0,
	
	
	0,0,0,0,
	0,0,0,0,
	I,I,I,I,
	0,0,0,0,
	
	0,I,0,0,
	0,I,0,0,
	0,I,0,0,
	0,I,0,0,
	
	0,0,0,0,
	0,0,0,0,
	I,I,I,I,
	0,0,0,0,
	
	0,I,0,0,
	0,I,0,0,
	0,I,0,0,
	0,I,0,0,
	
	
	0,T,0,0,
	T,T,T,0,
	0,0,0,0,
	0,0,0,0,
	
	0,T,0,0,
	0,T,T,0,
	0,T,0,0,
	0,0,0,0,
	
	0,0,0,0,
	T,T,T,0,
	0,T,0,0,
	0,0,0,0,
	
	0,T,0,0,
	T,T,0,0,
	0,T,0,0,
	0,0,0,0,
	
	
	0,0,0,0,
	0,S,S,0,
	S,S,0,0,
	0,0,0,0,
	
	S,0,0,0,
	S,S,0,0,
	0,S,0,0,
	0,0,0,0,
	0,0,0,0,
	
	0,S,S,0,
	S,S,0,0,
	0,0,0,0,
	
	S,0,0,0,
	S,S,0,0,
	0,S,0,0,
	0,0,0,0,
	
	
	0,0,0,0,
	J,J,J,0,
	0,0,J,0,
	0,0,0,0,
	
	0,J,0,0,
	0,J,0,0,
	J,J,0,0,
	0,0,0,0,
	
	J,0,0,0,
	J,J,J,0,
	0,0,0,0,
	0,0,0,0,
	
	0,J,J,0,
	0,J,0,0,
	0,J,0,0,
	0,0,0,0,
	
	
	0,0,0,0,
	Z,Z,0,0,
	0,Z,Z,0,
	0,0,0,0,
	
	0,Z,0,0,
	Z,Z,0,0,
	Z,0,0,0,
	0,0,0,0,
	
	0,0,0,0,
	Z,Z,0,0,
	0,Z,Z,0,
	0,0,0,0,
	
	0,Z,0,0,
	Z,Z,0,0,
	Z,0,0,0,
	0,0,0,0,
	
	
	0,0,0,0,
	L,L,L,0,
	L,0,0,0,
	0,0,0,0,
	
	L,L,0,0,
	0,L,0,0,
	0,L,0,0,
	0,0,0,0,
	
	0,0,L,0,
	L,L,L,0,
	0,0,0,0,
	0,0,0,0,
	
	0,L,0,0,
	0,L,0,0,
	0,L,L,0,
	0,0,0,0
};		// NE VRAIMENT PAS UTILISER ÇA DIRECTEMENT	
// Plutôt utiliser la fonction tetro :

/* Renvoie un tableau (constant !) à 16 cases correspondant au tetromino d'indice id
 * rot doit être compris entre 0 et 3, et correspond au nombre de rotations horaires effectuées par le tétromino (modulo 4)
 * Le tableau renvoyé est en fait un pointeur vers un bout du tableau ci-dessus, mais ça revient au même */
const int* tetro(int id,int rot){
	return &TETROMINOS[4*16*(id-1) + 16*rot];
}

/* Renvoie la couleur du bloc de coordonnées (i,j) de la grille t, correspondant à un tétromino (donc de largeur 4) */
int blocT(const int* t,int i,int j){
	return t[i+j*4];
} 
