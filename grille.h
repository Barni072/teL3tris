#ifndef GRILLE_H
#define GRILLE_H

extern const int LARG;
extern const int HAUT;
extern const int I;
extern const int O;
extern const int T;
extern const int L;
extern const int J;
extern const int Z;
extern const int S;
extern const int GRIS;
extern const int VIDE;
extern const int TETROMINOS[112];

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
	int delaiDescente;		// Délai actuel de la descente automatique, en milisecondes
	bool descenteRapide;
};
typedef s_etat etat;

const int* tetro(int id);
int blocQ(int* g,int i,int j,int w);
int blocG(etat* e,int i,int j);
int blocT(const int* t,int i,int j);
void ecritBlocG(etat* e,int i,int j,int clr);
//void permute(int* t,int i,int j);
void prochainSac(etat* e);
void tetrominoSuivant(etat* e);
void initEtat(etat* e);
void detruireEtat(etat* e);
void reserve(etat* e);
//bool collision(etat* e,int dx,int dy,int drot);
bool translation(etat*e,int dir);
void fixeTetromino(etat* e);
void descenteAuto(etat* e);
void descenteImmediate(etat* e);

#endif
