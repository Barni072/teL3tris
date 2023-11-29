#ifndef GRILLE_H
#define GRILLE_H

extern const int LARG;
extern const int HAUT;

struct s_etat{
	int* g;		// Grille de jeu, contient uniquement les blocs fixes
	int idTetro;	// Indice du tetromino courant
	int x;		// Abscisse du tetromino courant, vers la droite
	int y;		// Ordonnée du tetromino courant, VERS LE BAS
	int rota;	// Nombre de rotations effectuées par le tetromino courant
	int suivants[14];	// Indices des tetrominos à venir, assez grand pour 2 sacs
	int reserve; 	// Indice du tétromino actuellement en réserve (VIDE s'il n'y en a pas)
	bool reserveDispo;		// Si vrai, le tétromino courant ne provient pas de la réserve, et il est autorisé de l'y envoyer
	bool fermeture;		// Si vrai, la fermeture (propre) du jeu interviendra sous peu
	bool affiche;		// Doit être passé à vrai après tout changement destiné à être affiché, permet de ne pas rafraîchir l'affichage à chaque tick
	int delaiDescente;		// Délai actuel de la descente automatique, en milisecondes
	int iDescente;		// Compteur d'itérations de la boucle principale, la tétromino courant descent automatiquement quand ceci atteint delaiDescente
	bool descenteRapide;	// Vrai SSI le jeu est actuellement en mode descente rapide SSI la touche de descente rapide est actuellement enfoncée (mais désactivé automatiquement lorsque le tétromino courant change)
	int lignes;		// Compteur de lignes
};
typedef s_etat etat;

int blocG(etat* e,int i,int j);
void ecritBlocG(etat* e,int i,int j,int clr);
//void permute(int* t,int i,int j);
void prochainSac(etat* e);
//void enleveLignes(etat* e);
// void placeTetromino(etat* e);
void tetrominoSuivant(etat* e);
void initEtat(etat* e);
void detruireEtat(etat* e);
void reserve(etat* e);
//bool collision(etat* e,int dx,int dy,int drot);
bool translation(etat*e,int dir);
void fixeTetromino(etat* e);
void descenteAuto(etat* e);
void descenteImmediate(etat* e);
//void appliqueRotation(etat* e,int wkx,int wky,bool sens);
void rotation(etat* e,bool sens);
void descenteRapide(etat* e,bool rapide);
int offsetFantome(etat* e);

#endif
