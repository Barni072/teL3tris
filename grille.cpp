#include <cstdlib>
#include "grille.h"
#include "tetrominos.h"

// Taille de la grille de jeu, en blocs
const int LARG = 10;
const int HAUT = 20;

/* Renvoie la couleur du bloc de coordonnées (i,j) de la grille de jeu (de largeur LARG) contenue dans e */
int blocG(etat* e,int i,int j){
	return e->g[i+j*LARG];
}

/* Remplace la valeur du bloc de coordonnées (i,j) de la grille de jeu principale contenue dans e par clr */
void ecritBlocG(etat* e,int i,int j,int clr){
	e->g[i+j*LARG] = clr;
	return;
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
		// Mélange de Fisher-Yates (cf Wikipedia) pour une permutation aléatoire "effcicace" :
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

/* Place le prochain tétromino à son "point de départ" sur la grille de jeu et effectue les initialisations requises
 * Permettra éventuellement d'affiner un peu les points de départ des différents tétrominos
 * Pourrait servir à vérifier s'il y a une collision en posant le tétromino, et déclencher ainsi la fin de la partie... */
void placeTetromino(etat* e){
	e -> x = 3;
	e -> y = 0;
	e -> rota = 0;
	e -> iDescente = 0;
	e -> descenteRapide = false;	// Pour éviter les accidents stupides si la touche reste enfoncée
	e -> affiche = true;
	return;
}

/* Prend le premier tétromino suivant, et le met en haut de la grille, "prêt à tomber"
 * Les autres "tétrominos suivants" sont ensuite avancés d'une case dans le tableau des suivants */
void tetrominoSuivant(etat* e){		// IL FAUDRA AJOUTER UNE VÉRIFICATION DES COLLISIONS !
	e -> idTetro = e -> suivants[0];
	int i = 0;
	while(i < 13 && e -> suivants[i] != VIDE){
		e -> suivants[i] = e -> suivants[i+1];
		i += 1;
	}
	placeTetromino(e);
	e -> suivants[13] = VIDE;
	e -> reserveDispo = true;
	prochainSac(e);
	return;
}

/* Initialisa proprement la structure d'état */
void initEtat(etat* e){
	e -> idTetro = VIDE;
	e -> reserve = VIDE;
	for(int i = 0;i < 14;i++) e->suivants[i] = VIDE;
	e -> g = new int[HAUT*LARG];
	for(int i = 0;i < HAUT*LARG;i++){
		e->g[i] = 0;
	}
	e -> fermeture = false;
	e -> delaiDescente = 500;	// TEMPORAIRE, correspondra au niveau 1 (?)
	e -> descenteRapide = false;
	e -> lignes = 0;
	// Génère les 14 premiers tétrominos :
	prochainSac(e);
	prochainSac(e);
	tetrominoSuivant(e);
	return;
}

/* Détruit proprement la structure d'état */
void detruireEtat(etat* e){
	delete[] e -> g;
	return;
}

/* Envoie le tétromino courant dans la réserve (si possible), et le remplace par celui de la réserve (ou par le suivant lors de la 1ère utilisation de la réserve) */
void reserve(etat* e){		// IL FAUDRA AJOUTER UNE VÉRIFICATION DES COLLISIONS !
	if(e -> reserveDispo){
		if(e -> reserve == VIDE){	// Cas particulier de la première utilisation de la réserve
			e -> reserve = e -> idTetro;
			tetrominoSuivant(e);
		}else{
			int tmp = e -> reserve;
			e -> reserve = e -> idTetro;
			e -> idTetro = tmp;
			placeTetromino(e);
		}
		e -> reserveDispo = false;
	}
	return;
}

/* Vérifie si un mouvement du tétromino courant (translation de dx cases vers la droite et dy cases vers le 
 * bas, accompagnée de drot rotations horaires) est valide, mais n'effectue pas le déplacement en lui-même */
bool collision(etat* e,int dx,int dy,int drot){
	int x = e->x + dx;
	int y = e->y + dy;
	int rota = ((e->rota + drot)%4 +4) %4;	// Bricolage pour avoir un entier entre 0 et 3...
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(x+i < 0 || x+i >= LARG){	// Cas où x+i est hors de la grille
				if(blocT(tetro(e->idTetro,rota),i,j) != VIDE) return false;
			}else if(y+j < 0 || y+j >= HAUT){	// Cas où y+j est hors de la grille
				if(blocT(tetro(e->idTetro,rota),i,j) != VIDE) return false;
			}else{		// Cas général, dans la grille
				if(blocT(tetro(e->idTetro,rota),i,j) != VIDE && blocG(e,e->x + dx + i,e->y + dy + j) != VIDE){
					return false;
				}
			}
		}
	}
	return true;
}

/* Translate le tétromino courant, si c'est possible
 * L'entier dir correspond à une direction et doit être bien choisi
 * (0 : haut ; 1 : droite ; 2 : bas ; 3 : gauche) */
bool translation(etat* e,int dir){
	switch(dir){
		case 0:		// HAUT
			if(collision(e,0,-1,0)){
				e->y -= 1;
				e -> affiche = true;
				return true;
			}
			break;
		case 1:		// DROITE
			if(collision(e,1,0,0)){
				e->x += 1;
				e -> affiche = true;
				return true;
			}
			break;
		case 2:		// BAS
			if(collision(e,0,1,0)){
				e->y += 1;
				e -> affiche = true;
				return true;
			}
			break;
		case 3:		// GAUCHE
			if(collision(e,-1,0,0)){
				e->x -= 1;
				e -> affiche = true;
				return true;
			}
			break;
		default:	// Direction invalide
			// Afficher un message d'erreur ?
			break;
	}
	return false;
}

/* Vérifie si des lignes complétées existent dans la grille
 * Si oui, les enlève, incrémente le compteur de lignes, et n'incrémente pas encore le score...
 * GROS PROBLÈMES QUAND 2 LIGNES D'AFFILÉE SONT COMPLÉTÉES */
void enleveLignes(etat* e){
	int nbLignes = 0;
	bool plein;
	for(int j = HAUT-1;j >= 0;j--){
		plein = true;
		// On vérifie si la j-ième ligne est pleine :
		for(int i = 0;i < LARG;i++){
			if(blocG(e,i,j) == VIDE){
				plein = false;
				break;
			}
		}
		if(plein){
			nbLignes += 1;
			// On fait descendre les lignes supérieures :
			for(int k = j-1;k >= 0;k--){
				for(int i = 0;i < LARG;i++){
					ecritBlocG(e,i,k+1,blocG(e,i,k));
				}
			}
			// On remet bien des blocs VIDEs sur la ligne la plus haute :
			for(int i = 0;i < LARG;i++){
				ecritBlocG(e,i,0,VIDE);
			}
			j += 1;		// Permet de détecter le cas où 2 lignes "d'affilée" sont complétées (sinon, on saute la 2ème...)
		}
	}
	e -> lignes += nbLignes;	// Le compteur de lignes n'a pas encore été testé...
	// + Il faudra incrémenter le score ici
	return;
}

/* Place les blocs du tétromino courant dans e->g, et appelle le tétromino suivant */
void fixeTetromino(etat* e){
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			int clr = blocT(tetro(e->idTetro,e->rota),i,j);
			if(clr != VIDE) ecritBlocG(e,e->x + i,e->y + j,clr);
		}
	}
	enleveLignes(e);
	tetrominoSuivant(e);
	return;
}

/* Descente automatique du tetromino courant
 * Ne gère pas le timing, donc doit être appelée "au bon moment" */
void descenteAuto(etat* e){
	if(!translation(e,2)){
		fixeTetromino(e);
	}
	return;
}

/* Descend le tétromino courant autant que possible immédiatement, et passe au suivant */
void descenteImmediate(etat* e){
	while(translation(e,2));
	fixeTetromino(e);
	return;
}

/* Fonction auxilliaire de la fonction rotation, effectue la rotation (change l'état) en prenant
 * en compte les wallkicks et en supposant que les tests de collisions ont déjà été faits */
void appliqueRotation(etat* e,int wkx,int wky,bool sens){
	if(sens) e -> rota = (((e->rota + 1) % 4)+4)%4;	// Formule un peu suspecte mais requise, pour toujours avoir e->rota entre 0 et 3
	else e -> rota = (((e->rota -1) % 4)+4)%4;		// Idem
	e -> x += wkx;
	e -> y += wky;
	e -> affiche = true;
	return;
}

/* Modifie l'état e pour que le tétromino courant soit considéré comme ayant tourné, si c'est possible (sinon, ne fait rien)
 * Le booléen sens est vrai SSI la rotation demandée est dans le sens horaire
 * ON IMPLÉMENTERA LES WALL KICKS ICI */
void rotation(etat* e,bool sens){
	int drot;
	if(sens) drot = 1;
	else drot = 3;
	// OU BIEN : int drot = 3 - (2 * sens);	?	(Alternative à tester...)
	if(collision(e,0,0,drot)){
		appliqueRotation(e,0,0,sens);
	}else if(collision(e,1,0,drot)){	// Wallkick vers la droite
		appliqueRotation(e,1,0,sens);
	}else if(collision(e,-1,0,drot)){	// Wallkick vers la gauche
		appliqueRotation(e,-1,0,sens);
	}else if(collision(e,0,1,drot)){	// Wallkick vers le bas
		appliqueRotation(e,0,1,sens);
	}else if(collision(e,1,-1,drot)){	// Wallkick vers le haut
		appliqueRotation(e,0,-1,sens);
	}else if(e->idTetro == I && collision(e,-2,0,drot)){	// Double wallkick à gauche, seulement pour le tétromino I
		appliqueRotation(e,-2,0,sens);
	}	// Si tout échoue, on considère que la rotation a échoué, et on ne fait rien
	return;
}

/* Active ou désactive la descente rapide, selon la valeur du booléen rapide */
void descenteRapide(etat* e,bool rapide){
	e -> descenteRapide = rapide;
	return;
}
