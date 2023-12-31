#ifndef GRILLE_H
#define GRILLE_H

extern const int LARG;
extern const int HAUT;
extern const int TMPS_ANIM;
extern const int NB_SUIVANTS;
extern int* SUIVANTS;

struct s_etat{
	int* g;		// Grille de jeu, contient uniquement les blocs fixes
	int idTetro;	// Indice du tetromino courant (dans le tableau des tétrominos)		// PLUS NÉCESSAIRE
	int x;		// Abscisse du tetromino courant, vers la droite
	int y;		// Ordonnée du tetromino courant, VERS LE BAS
	int rota;	// Nombre de rotations horaires effectuées par le tetromino courant, modulo 4
	
	int idProchain;		// Indice du prochain tétromino (le 1er tétromino suivant) dans le tableau SUIVANTS
	//int suivants[14];	// Indices des tetrominos à venir, assez grand pour 2 sacs	// ANCIENNE IMPLÉMENTATION
	int reserve; 	// Indice du tétromino actuellement en réserve (VIDE s'il n'y en a pas)
	bool reserveDispo;		// Si vrai, le tétromino courant ne provient pas de la réserve, et il est autorisé de l'y envoyer
	bool descenteRapide;	// Vrai SSI le jeu est actuellement en mode descente rapide SSI la touche de descente rapide est actuellement enfoncée (mais désactivé automatiquement lorsque le tétromino courant change)

	int delaiDescente;		// Délai actuel de la descente automatique, en milisecondes
	int iDescente;		// Compteur d'itérations de la boucle principale, le tétromino courant descend automatiquement quand ce compteur atteint delaiDescente
	int progresAnimationLignes;	// Contient le nombre de frames restantes avant la fin de l'animation de suppression des lignes ou -1 si l'animation n'est pas en cours
	int lignesPleines[4];	// Contient les ordonnées des éventuelles lignes pleines, ou des -1 s'il n'y a pas assez de lignes pleines, sert à déclencher l'animation de suppression des lignes
	int* copieLignesAnimation;	// Contient une copie de la couleur des blocs des lignes pleines (solution la plus simple pour les faire clignoter)
	
	int lignes;		// Compteur de lignes
	int niveau;		// Augmente avec le nombre de lignes, devrait pouvoir être sélectionné par l'utilisateur
	int score;		// Calculé à peu près comme dans la version GB, en rajoutant un bonus d'un point par case en descente rapide, et de 2 points par case en descente immédiate
	
	bool fermeture;		// Si vrai, le joueur a perdu ou a demandé la fermeture du jeu
	bool affiche;		// Doit être passé à vrai après tout changement destiné à être affiché, permet de ne pas rafraîchir l'affichage à chaque frame
	
	int attaquesRecues;		// Nombre de lignes d'attaques qui seront rajoutées en bas de la grille de jeu avec l'arrivée du tétromino suivant (évidemment toujours 0 en mode 1 joueur)
};
typedef s_etat etat;

int blocG(etat* e,int i,int j);
void ecritBlocG(etat* e,int i,int j,int clr);
//void permute(int* t,int i,int j);
void prochainSac(etat* e);
//void detecteLignes(etat* e);
void supprimeLignes1J(etat* e);
void supprimeLignes2J(etat* e,etat* adv);
//void placeTetromino(etat* e);
//void tetrominoSuivant(etat* e);
void initEtat(etat* e);
void detruireEtat(etat* e);
void reserve(etat* e);
//bool collision(etat* e,int dx,int dy,int drot);
bool translation(etat*e,int dir);
//void recoitAttaques(etat* e);
void fixeTetromino(etat* e);
void descenteAuto(etat* e);
void descenteImmediate(etat* e);
//void appliqueRotation(etat* e,int wkx,int wky,bool sens);
void rotation(etat* e,bool sens);
void descenteRapide(etat* e,bool rapide);
int offsetFantome(etat* e);
//void finPartie(etat* e);
void changeVitesse(etat* e);
void grillePerdant(etat* e);
int blocC(etat* e,int i,int j);
void ecritBlocC(etat* e,int i,int j,int clr);
//void lanceAnimationLignes(etat* e);
void initSuivants();
void detruireSuivants();
//void genereSac(int n);

#endif
