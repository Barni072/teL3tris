#include <cstdlib>
#include "grille.h"
#include "tetrominos.h"

#include <iostream>
using namespace std;

// Taille de la grille de jeu, en blocs
const int LARG = 10;
const int HAUT = 20;

// Tétrominos suivants
const int NB_SUIVANTS = 7000;		// Il est préférable que NB_SUIVANTS soit un multiple de 2*7=14
int* SUIVANTS;

// Durée d'un sixième de l'animation de suppression des lignes, comptée en itérations de la boucle principale
const int TMPS_ANIM = 50;

/* Renvoie la couleur du bloc de coordonnées (i,j) de la grille de jeu (de largeur LARG) contenue dans e */
int blocG(etat* e,int i,int j){
	return e->g[i+j*LARG];
}

/* Remplace la valeur du bloc de coordonnées (i,j) de la grille de jeu principale contenue dans e par clr */
void ecritBlocG(etat* e,int i,int j,int clr){
	e->g[i+j*LARG] = clr;
	return;
}

/* Renvoie la couleur du bloc de coordonnées (i,j) de e->copieLignesAnimation (de largeur LARG) */
int blocC(etat* e,int i,int j){
	return e->copieLignesAnimation[i+j*LARG];
}

/* Remplace la valeur du bloc de coordonnées (i,j) de e->copieLignesAnimation par clr */
void ecritBlocC(etat* e,int i,int j,int clr){
	e->copieLignesAnimation[i+j*LARG] = clr;
	return;
}



/* Change e->fermeture pour déclencher la fin du programme
 * (Ne s'occupe pas de détruire proprement les structures utilisées, mais d'autres fonctions auront l'occasion de le faire) */
void finPartie(etat* e){
	e -> fermeture = true;
	return;
}




/* Vérifie si un mouvement du tétromino courant (translation de dx cases vers la droite et dy cases vers le 
 * bas, accompagnée de drot rotations horaires) est valide, MAIS N'EFFECTUE PAS LE DÉPLACEMENT
 * (Renvoie vrai si le mouvement est valide, et faux en cas de collision) */
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
 * Renvoie un booléen correspondant à la réussite de l'opération
 * L'entier dir correspond à une direction et doit être bien choisi :
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
		default:	// Direction invalide, on ne fait rien
			break;
	}
	return false;
}

/* Fonction auxilliaire de la fonction rotation, effectue la rotation (c'est cette fonction qui change l'état) en prenant
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
 * Si la rotation est bloquée par un mur ou un des blocs de la grille, on essaie de translater légèrement le tétromino courant ("wallkick") */
void rotation(etat* e,bool sens){
	int drot;
	if(sens) drot = 1;
	else drot = 3;
	if(collision(e,0,0,drot)){
		appliqueRotation(e,0,0,sens);
	}else if(collision(e,1,0,drot)){	// Wallkick vers la droite
		appliqueRotation(e,1,0,sens);
	}else if(collision(e,-1,0,drot)){	// Wallkick vers la gauche
		appliqueRotation(e,-1,0,sens);
	}else if(collision(e,0,1,drot)){	// Wallkick vers le bas
		appliqueRotation(e,0,1,sens);
	/*}else if(collision(e,1,-1,drot)){	// Wallkick vers le haut
		appliqueRotation(e,0,-1,sens);*/	// Permettrait de faire durer la partie indéfiniment...
	}else if(collision(e,1,1,drot)){	// Wallkick bas-droite
		appliqueRotation(e,1,1,sens);
	}else if(collision(e,-1,1,drot)){	// Wallkick bas-gauche
		appliqueRotation(e,-1,1,sens);
	}else if(e->idTetro == I && collision(e,-2,0,drot)){	// Double wallkick à gauche, seulement pour le tétromino I
		appliqueRotation(e,-2,0,sens);
	}else if(e->idTetro == I && collision(e,0,2,drot)){		// Double wallkick bas, seulement pour le tétromino I
		appliqueRotation(e,0,2,sens);
	}	// Si tout échoue, on considère que la rotation a échoué, et on ne fait rien
	return;
}

/* Descente automatique du tetromino courant, et incrémente le score en descente rapide
 * Ne gère pas le timing, donc doit être appelée "au bon moment"
 * Fixe le tétromino s'il ne peut pas descendre (donc déclenche indirectement l'appel du tétromino suivant et la détection des lignes pleines) */
void descenteAuto(etat* e){
	if(!translation(e,2)){
		fixeTetromino(e);
	}else if(e -> descenteRapide){
		e -> score += 1;
		// On veut ajouter un point seulement lorsque la descente actuelle est rapide, et a effectivement lieu
	}
	return;
}

/* Descend le tétromino courant autant que possible immédiatement, donne quelques points, et passe au suivant */
void descenteImmediate(etat* e){
	e -> iDescente = 0;	// Sinon, le tétromino suivant ferait sa première descente plus tôt que prévu
	while(translation(e,2)) e->score += 2;	// On augmente le score de 2 points par bloc descendu
	fixeTetromino(e);
	return;
}

/* Active ou désactive la descente rapide, selon la valeur du booléen rapide */
void descenteRapide(etat* e,bool rapide){
	e -> descenteRapide = rapide;
	return;
}





/* Fonction auxiliaire pour prochainSac
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
 * ANCIENNE IMPLÉMENTATION (sans le tableau SUIVANTS, incompatible avec le mode 2 joueurs) */
/*void prochainSac(etat* e){
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
		for(int i = 6;i >= 0;i--){
			int j = rand() % (i+1);
			permute(t,i,j);
		}
		// Rajoute les nouveaux indices dans l'état e :
		for(int i = 0;i <= 6;i++){
			e->suivants[i_vide+i] = t[i];		// Les éléments d'indice < i_vide sont déjà peuplés
		}
	}
	
	return;
}*/

/* Modifie le tableau SUIVANTS pour ajouter un "sac" de 7 tétrominos à partir de l'indice n */
void genereSac(int n){
	// Mélange de Fisher-Yates (cf Wikipedia) pour une permutation aléatoire "efficace" :
	int t[7];
	for(int i = 0;i <= 6;i++){
		t[i] = i+1;		// Car les indices des tétominos vont de 1 à 7
	}
	for(int i = 6;i >= 0;i--){
		int j = rand() % (i+1);
		permute(t,i,j);
	}
	// Copie du sac ainsi généré dans SUIVANTS :
	for(int i = 0;i <= 6;i++){
		SUIVANTS[n+i] = t[i];
	}
	return;
}




/* Fait correspondre le "niveau", et surtout la vitesse de descente automatique des tétrominos, au nombre de lignes actuel
 * Les vitesses de descente sont calqués sur la version GB du jeu, à quelques arrondis près */
void changeVitesse(etat* e){
	switch(e->niveau){
		case(0):
			e->delaiDescente = 442;
			break;
		case(1):
			e->delaiDescente = 408;
			break;
		case(2):
			e->delaiDescente = 375;
			break;
		case(3):
			e->delaiDescente = 342;
			break;
		case(4):
			e->delaiDescente = 308;
			break;
		case(5):
			e->delaiDescente = 275;
			break;
		case(6):
			e->delaiDescente = 217;
			break;
		case(7):
			e->delaiDescente = 183;
			break;
		case(8):
			e->delaiDescente = 142;
			break;
		case(9):
			e->delaiDescente = 92;
			break;
		case(10):
			e->delaiDescente = 83;
			break;
		case(11):
			e->delaiDescente = 75;
			break;
		case(12):
			e->delaiDescente = 67;
			break;
		case(13):
			e->delaiDescente = 58;
			break;
		case(14):
			e->delaiDescente = 50;
			break;
		case(15):
			e->delaiDescente = 50;
			break;
		case(16):
			e->delaiDescente = 42;
			break;
		case(17):
			e->delaiDescente = 42;
			break;
		case(18):
			e->delaiDescente = 33;
			break;
		case(19):
			e->delaiDescente = 33;
			break;
		default:	// Niveau 20 et supérieurs, la vitesse reste fixe
			e->delaiDescente = 25;
			break;
	}
	return;
}

/* Place le prochain tétromino à son "point de départ" sur la grille de jeu et effectue les initialisations requises
 * Permettra éventuellement d'affiner un peu les points de départ des différents tétrominos
 * Pourrait servir à vérifier s'il y a une collision en posant le tétromino, et déclencher ainsi la fin de la partie... */
void placeTetromino(etat* e){
	e -> x = 3;
	// Les différents tétrominos apparaissent à des hauteurs différentes, de façon à être "tout en haut" de l'aire de jeu
	if(e->idTetro == I){
		e -> y = -2;
	}else if(e -> idTetro == T){
		e -> y = 0;
	}else{
		e -> y = -1;
	}
	e -> rota = 0;
	e -> iDescente = 0;
	e -> descenteRapide = false;	// Pour éviter les accidents stupides si la touche reste enfoncée
	if(!collision(e,0,0,0)) finPartie(e);	// Si on ne peut pas placer le bloc, la partie est terminée
	// Éventuellement rajouter des wallkicks latéraux ici...
	e -> affiche = true;
	return;
}

/* Prend le premier tétromino suivant, et le met en haut de la grille, "prêt à tomber"
 * Les autres "tétrominos suivants" sont ensuite avancés d'une case dans le tableau des suivants */
void tetrominoSuivant(etat* e){
	//e -> idTetro = e -> suivants[0];		// ANCIEN
	e -> idTetro = SUIVANTS[e -> idProchain];
	e -> idProchain += 1;	// TODO : Ajouter une vérification pour quand idProchain devient très grand...
	e -> idProchain = e ->idProchain%NB_SUIVANTS;	// Peu élégant, et demande que 7 | NB_SUIVANTS...
	/*int i = 0;
	while(i < 13 && e -> suivants[i] != VIDE){
		e -> suivants[i] = e -> suivants[i+1];
		i += 1;
	}*/		// ANCIEN
	placeTetromino(e);
	//e -> suivants[13] = VIDE;		// ANCIEN
	e -> reserveDispo = true;
	//prochainSac(e);		// ANCIEN
	return;
}





/* Fonction auxiliaire de detecteLignes, déclenche l'animation de suppression des lignes, en changeant e->progresAnimationLignes */
void declencheAnimationLignes(etat* e){
	e -> progresAnimationLignes = TMPS_ANIM * 6;
	return;
}

/* Remplit e->lignesPleines contenant par les ordonnées des lignes pleines, DANS L'ORDRE CROISSANT
 * Ne vide pas ces lignes, et n'incrémente pas le compteur de lignes (UTILISER SUPPRIMELIGNES POUR ÇA)
 * Copie aussi les lignes pleines dans le tableau e->copieLignesAnimation
 * Il peut y avoir au maximum 4 lignes remplies à la fois, d'où la taille de e->lignesPleines */
void detecteLignes(etat* e){
	int k = 0;
	for(int j = 0;j < HAUT;j++){
		bool plein = true;
		// On vérifie si la j-ième ligne est pleine :
		for(int i = 0;i < LARG;i++){
			if(blocG(e,i,j) == VIDE){
				plein = false;
				break;
			}
		}
		if(plein){
			e -> lignesPleines[k] = j;
			// Copie dans le tableau e->copieLignesAnimation :
			for(int i = 0;i < LARG;i++){
				ecritBlocC(e,i,k,blocG(e,i,e->lignesPleines[k]));
			}
			k += 1;
		}
	}
	if(k != 0){		// càd : si au moins une ligne pleine a été détectée
		declencheAnimationLignes(e);
	}
	return;
}

/* Supprime de la grille les lignes indiquées dans e->lignesPleines (qui doivent y être PAR ORDONNÉES CROISSANTES)
 * Effectue également les incrémentations nécessaires (score, lignes...), et réinitialise e->lignesPleines */
void supprimeLignes1J(etat* e){
	int nbLignes = 0;
	for(int k = 0;k < 4;k++){
		if(e->lignesPleines[k] != -1){	// Sinon, il n'y a pas de lignes à supprimmer
			nbLignes += 1;
			// On translate toutes les lignes supérieures vers le bas
			for(int l = e->lignesPleines[k]-1;l >= 0;l--){
				for(int i = 0;i < LARG;i++){
					ecritBlocG(e,i,l+1,blocG(e,i,l));
				}
			}
			// On remet bien des blocs VIDEs sur la ligne la plus haute :
			for(int i = 0;i < LARG;i++){
				ecritBlocG(e,i,0,VIDE);
			}
		}
	}
	e -> lignes += nbLignes;
	// Réinitialise e->lignesPleines
	for(int k = 0;k < 4;k++){
		e -> lignesPleines[k] = -1;
	}
	switch(nbLignes){	// Incrémentation du score
		case(1):
			e -> score += 40*(e->niveau+1);
			break;
		case(2):
			e -> score += 100*(e->niveau+1);
			break;
		case(3):
			e -> score += 300*(e->niveau+1);
			break;
		case(4):
			e -> score += 1200*(e->niveau+1);
			break;
	}
	if(e->lignes >= 10*(e->niveau+1)){
		e->niveau += 1;
		changeVitesse(e);
	}
	return;
}

/* Même fonction que la supprimeLignes1J, mais adaptée au mode 2 joueurs :
 * Effectue la suppression des lignes pour le joueur e (exactement comme en mode 1 joueur), et déclenche l'envoi d'attaques au joueur adv */
void supprimeLignes2J(etat* e,etat* adv){
	int nbLignes = 0;
	for(int k = 0;k < 4;k++){
		if(e->lignesPleines[k] != -1){	// Sinon, il n'y a pas de lignes à supprimmer
			nbLignes += 1;
			// On translate toutes les lignes supérieures vers le bas
			for(int l = e->lignesPleines[k]-1;l >= 0;l--){
				for(int i = 0;i < LARG;i++){
					ecritBlocG(e,i,l+1,blocG(e,i,l));
				}
			}
			// On remet bien des blocs VIDEs sur la ligne la plus haute :
			for(int i = 0;i < LARG;i++){
				ecritBlocG(e,i,0,VIDE);
			}
		}
	}
	e -> lignes += nbLignes;
	// Réinitialise e->lignesPleines
	for(int k = 0;k < 4;k++){
		e -> lignesPleines[k] = -1;
	}
	switch(nbLignes){	// Incrémentation du score et envoi des attaques à l'adversaire
		case(1):
			e -> score += 40*(e->niveau+1);
			// Pas d'attaque
			break;
		case(2):
			e -> score += 100*(e->niveau+1);
			adv -> attaquesRecues += 1;
			break;
		case(3):
			e -> score += 300*(e->niveau+1);
			adv -> attaquesRecues += 2;
			break;
		case(4):
			e -> score += 1200*(e->niveau+1);
			adv -> attaquesRecues += 4;
			break;
	}
	if(e->lignes >= 10*(e->niveau+1)){
		e->niveau += 1;
		changeVitesse(e);
	}
	//e->progresAnimationLignes = -1;	//TEST
	return;
}

/* Rajoute en bas de la grille de jeu le nombre de lignes d'attaques contenu dans e->attaquesRecues
 * (En mode 1 joueur, e->attaquesRecues vaut 0 et ceci n'a aucun effet)
 * S'il n'y a pas la place, le joueur a perdu (mais n'aura pas le temps de voir/comprendre pourquoi...)
 * (En pratique, cette fonction fait n'importe quoi) */
void recoitAttaques(etat* e){
	// Décide de l'emplacement du "trou" dans les lignes d'attaque :
	int trou = rand()%LARG;
	//cout << e -> attaquesRecues << endl << trou << endl << endl;	// DEBUG
	// Monte les blocs déjà présents :
	for(int i = 0;i < LARG;i++){
		for(int j = HAUT-1;j >= 0;j--){
			if(!(blocG(e,i,j) != VIDE && j-(e->attaquesRecues) < 0)){	// Sinon, un bloc non vide serait translaté en dehors de la grille, et on ne peut pas faire ça (le joueur a donc perdu)
				ecritBlocG(e,i,j-(e->attaquesRecues),blocG(e,i,j));
			}else{
				e -> fermeture = true;
			}
		}
	}
	// Rajoute les nouvelles lignes en bas de la grille :
	for(int i = 0;i < LARG;i++){
		if(i != trou){
			for(int k = 1;k <= e->attaquesRecues;k++){
				ecritBlocG(e,i,HAUT-k,GRIS);
			}
		}
	}
	e -> attaquesRecues = 0;
	return;
}

/* Envoie le tétromino courant dans la réserve (si possible), et le remplace par celui de la réserve (ou par le suivant lors de la 1ère utilisation de la réserve) */
void reserve(etat* e){
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

/* Place les blocs du tétromino courant dans e->g, et appelle le tétromino suivant
 * Appelle indirectement la détection des lignes pleines, mais ne cherche pas à les enlever */
void fixeTetromino(etat* e){
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			int clr = blocT(tetro(e->idTetro,e->rota),i,j);
			if(clr != VIDE) ecritBlocG(e,e->x + i,e->y + j,clr);
		}
	}
	detecteLignes(e);
	//if(e->attaquesRecues != 0) recoitAttaques(e);		// OULÀLÀ
	tetrominoSuivant(e);
	return;
}

/* Renvoie un entier correspondant aux nombre de cases que le tétromino courant doit descendre avant
 * d'arriver à la position où il ne pourra plus descendre (ie une case au dessus de la collision)
 * (Fonction auxiliaire à l'affichage d'un "fantôme" du tétromino courant à cette position) */
int offsetFantome(etat* e){
	int i = 1;
	while(collision(e,0,i,0)){
		i += 1;
	}
	return i-1;
}

/* Remplit la grille de jeu avec un motif douteux à damiers, pour signifier que le joueur a fini
 * (Seulement utile/visible en mode deux joueurs) */
void grillePerdant(etat* e){
	for(int i = 0;i < LARG;i++){
		for(int j = 0;j < HAUT;j++){
			if((i+j)%2 == 0) ecritBlocG(e,i,j,BRILLE);
			else ecritBlocG(e,i,j,VIDE);
		}
	}
	e -> idTetro = VIDE;
	e -> reserve = VIDE;
	/*for(int i = 0;i < 14;i++){
		e -> suivants[i] = VIDE;
	}*/		// Il faudra faire sans ça...
	return;
}



/* Initialise proprement la structure d'état */
void initEtat(etat* e){
	e -> idTetro = VIDE;
	e -> idProchain = 1;
	e -> reserve = VIDE;
	e -> reserveDispo = true;
	//for(int i = 0;i < 14;i++) e->suivants[i] = VIDE;		// ANCIEN
	e -> g = new int[HAUT*LARG];
	e -> copieLignesAnimation = new int[4*LARG];
	for(int i = 0;i < HAUT*LARG;i++){
		e->g[i] = 0;
	}
	e -> fermeture = false;
	e -> descenteRapide = false;
	for(int k = 0;k < 4;k++){
		e -> lignesPleines[k] = -1;
	}
	e -> attaquesRecues = 0;
	e -> progresAnimationLignes = -1;	// L'animation de suppression des lignes n'a aucune raison d'être activée en début de partie
	e -> lignes = 0;
	e -> score = 0;
	e -> niveau = 0;	// On garde ça au cas où, mais en théorie le niveau devrait être récupéré dans le fichier de configuration
	changeVitesse(e);	// Idem
	/*// Génère les 14 premiers tétrominos :
	prochainSac(e);
	prochainSac(e);*/	// ANCIEN
	tetrominoSuivant(e);
	return;
}

/* Détruit proprement la structure d'état */
void detruireEtat(etat* e){
	delete[] e -> g;
	delete[] e -> copieLignesAnimation;
	return;
}

void initSuivants(){
	SUIVANTS = new int[NB_SUIVANTS];
	for(int n = 0;n < NB_SUIVANTS;n += 7){
		genereSac(n);
	}
	return;
}

void detruireSuivants(){
	delete[] SUIVANTS;
	return;
}
