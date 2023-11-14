#include <cstdlib>
#include "grille.h"

// Taille de la grille de jeu, en blocs
const int LARG = 10;
const int HAUT = 20;

// Identifiants des différents tétrominos, et des couleurs de leurs blocs
const int O = 1;
const int I = 2;
const int T = 3;
const int S = 4;
const int J = 5;
const int Z = 6;
const int L = 7;
const int VIDE = 0;		// Signifie : "Il n'y a rien ici"
// id == 1 : 1 position de rotation
// id % 2 == 0 : 2 positions de rotation
// Sinon : 4 positions de rotation

// Tous sauf O et I : rotation matricielle en gardant seulement les 3 1ers indices !

/* Contient les positions de départ des tétrominos
 * À ne surtout pas utiliser directement */
const int TETROMINOS[112] = {
	0,0,0,0,
	0,O,O,0,
	0,O,O,0,
	0,0,0,0,
	
	0,0,0,0,
	0,0,0,0,
	I,I,I,I,
	0,0,0,0,
	
	0,T,0,0,
	T,T,T,0,
	0,0,0,0,
	0,0,0,0,
	
	0,0,0,0,
	0,S,S,0,
	S,S,0,0,
	0,0,0,0,
	
	0,0,0,0,
	J,J,J,0,
	0,0,J,0,
	0,0,0,0,
	
	0,0,0,0,
	Z,Z,0,0,
	0,Z,Z,0,
	0,0,0,0,
	
	0,0,0,0,
	L,L,L,0,
	L,0,0,0,
	0,0,0,0
};		// NE VRAIMENT PAS UTILISER ÇA DIRECTEMENT	
// Plutôt utiliser cette fonction :
/* Renvoie un tableau (constant !) à 16 cases correspondant au tetromino d'indice id
 * C'est en fait un pointeur vers un bout du tableau ci-dessus, mais ça revient au même */
const int* tetro(int id){
	return &TETROMINOS[16*(id-1)];
}

/* Renvoie la couleur du bloc de coordonnées (i,j) de la grille g quelconque, qui doit être de largeur w */
int blocQ(int* g,int i,int j,int w){
	return g[i+j*w];
}		// En fait, ne sert pas...

/* Renvoie la couleur du bloc de coordonnées (i,j) de la grille de jeu (de largeur LARG) contenue dans e */
int blocG(etat* e,int i,int j){
	//return blocQ(e->g,i,j,LARG);
	return e->g[i+j*LARG];
}

/* Renvoie la couleur du bloc de coordonnées (i,j) de la grille t, correspondant à un tétromino (donc de largeur 4) */
int blocT(const int* t,int i,int j){
	//return blocQ(t,i,j,4);	// Ne fonctionne pas, car blocQ refuse que t soit constant
	return t[i+j*4];
}

/* Fonction auxiliaire pour la fonction suivante
 * Permute les éléments d'indice i et j du tableau t */
void permute(int* t,int i,int j){
	int tmp = t[i];
	t[i] = t[j];
	t[j] = tmp;
	return;
}

/* Met à jour le tableau des tétrominos suivants de e avec le prochain "sac" de 7 tétrominos
 * Ne fait quelque chose que si le tableau des tétrominos suivants contient assez de place
 * Doit être appelée à l'apparition de chaque nouveau tétromino (et avant le début de la partie)
 * Ne sera pas vraiment adapté à un mode 2 joueurs (si on veut que les 2 joueurs aient la même suite de tétrominos) */
void prochainSac(etat* e){
	// Trouve à partir de quel indice i_vide le tableau des tétrominos suivants est vide
	int i_vide = 8;
	for(int i = 0;i <= 7;i++){
		if(e->suivants[i] == VIDE){
			i_vide = i;
			break;
		}
	}
	// Si i_vide >= 8 (en pratique si i_vide == 8), il n'y a pas la place pour un nouveau sac
	if(i_vide < 8){
		// Algorithme de Fisher-Yates (cf Wikipedia) pour une permutation aléatoire "effcicace" :
		int t[7];
		for(int i = 0;i <= 6;i++){
			t[i] = i+1;		// Car les indices des tétominos vont de 1 à 7
		}
		for(int i = 0;i <= 6;i++){
			int j = rand() % 7;
			permute(t,i,j);
		}
		// Rajoute les nouveaux indices dans l'état e :
		for(int i = 0;i <= 6;i++){
			e->suivants[i_vide+i] = t[i];		// Les éléments d'indice < i_vide sont déjà peuplés
		}
	}
	
	return;
}

/* Prend le premier tétromino suivant, et le met en haut de la grille, "prêt à tomber"
 * Les autres "tétrominos suivants" sont ensuite avancés d'une case dans le tableau des suivants */
void tetrominoSuivant(etat* e){
	e -> idTetro = e -> suivants[0];
	int i = 0;
	while(i < 13 && e -> suivants[i] != VIDE){
		e -> suivants[i] = e -> suivants[i+1];
		i += 1;
	}
	e -> x = 3;
	e -> y = 0;
	e -> suivants[13] = VIDE;
	e -> reserveDispo = true;
	prochainSac(e);
	return;
}

void initEtat(etat* e){
	e -> idTetro = VIDE;
	e -> reserve = VIDE;
	for(int i = 0;i < 14;i++) e->suivants[i] = VIDE;
	e -> g = new int[HAUT*LARG];
	for(int i = 0;i < HAUT*LARG;i++){
		e->g[i] = 0;
	}
	e -> fermeture = false;
	// Génère les 14 premiers tétrominos :
	prochainSac(e);
	prochainSac(e);
	tetrominoSuivant(e);
	return;
}

void detruireEtat(etat* e){
	delete[] e -> g;
	return;
}

void reserve(etat* e){
	if(e -> reserveDispo){
		if(e -> reserve == VIDE){	// Cas particulier de la première utilisation de la réserve
			e -> reserve = e -> idTetro;
			tetrominoSuivant(e);
		}else{
			int tmp = e -> reserve;
			e -> reserve = e -> idTetro;
			e -> idTetro = tmp;
		}
		e -> reserveDispo = false;
	}
	return;
}
