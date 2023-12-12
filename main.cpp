#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "affichage.h"
#include "grille.h"
#include "entree.h"
using namespace std;

// Permettra de lire dans le fichier de configuration teL3tris.cfg :
ifstream cfg;

/* Initialise proprement les structures utilisées */
void initialisation1J(etat* e){
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	TTF_Init();
	fenetre = SDL_CreateWindow("teL3tris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,(LARG+5)*TLBC + 2*MARGE,HAUT*TLBC + 2*MARGE,SDL_WINDOW_SHOWN);
	rndr = SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	srand(time(NULL));
	police = TTF_OpenFont("JupiteroidRegular-Rpj6V.ttf",TLBC*2/3 - 4);	// Provenance : fontspace.com/jupiteroid-font-f90261 (domaine public)
	if(police == NULL) cout << "ATTENTION ! Fichier de police de texte non trouvé, affichage des scores impossible..." << endl;
	initEtat(e);
	// Récupération du niveau initial dans le fichier de configuration :
	cfg >> e -> niveau;
	// Si l'utilisateur a entré n'importe quoi, il jouera au niveau 0
	if(e->niveau > 20 || e->niveau < 0) e->niveau = 0;
	changeVitesse(e);
	// Ici : initialisation des commandes ? (APRÈS celle de l'état !)
	return;
}

/* Même fonction que la précédente, mais pour le mode 2 joueurs (donc avec 2 états) */
void initialisation2J(etat* e1,etat* e2){
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	TTF_Init();
	fenetre = SDL_CreateWindow("teL3tris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,2*(LARG+5)*TLBC + 2*(MARGE+TLBC),HAUT*TLBC + 2*MARGE,SDL_WINDOW_SHOWN);
	rndr = SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	police = TTF_OpenFont("JupiteroidRegular-Rpj6V.ttf",TLBC*2/3 - 4);	// Provenance : fontspace.com/jupiteroid-font-f90261 (domaine public)
	if(police == NULL) cout << "ATTENTION ! Fichier de police de texte non trouvé, affichage des scores impossible..." << endl;
	srand(time(NULL));
	initEtat(e1);
	initEtat(e2);
	// Récupération des niveaux initiaux (pas forcément le même pour chaque joueur) dans le fichier de configuration :
	cfg >> e1 -> niveau;
	cfg >> e2 -> niveau;
	// Si un utilisateur a entré n'importe quoi, il jouera au niveau 0
	if(e1->niveau > 20 || e1->niveau < 0) e1->niveau = 0;
	if(e2->niveau > 20 || e2->niveau < 0) e2->niveau = 0;
	changeVitesse(e1);
	changeVitesse(e2);
	cfg.close();
	// Ici : initialisation des commandes ? (APRÈS celle des états !)
	return;
}

/* Détruit proprement les structures utilisées */
void fermeture1J(etat* e){
	SDL_DestroyWindow(fenetre);
	SDL_DestroyRenderer(rndr);
	TTF_CloseFont(police);
	detruireEtat(e);
	SDL_Quit();
	TTF_Quit();
	return;
}

/* Même fonction que la précédente, mais pour le mode 2 joueurs (donc avec 2 états) */
void fermeture2J(etat* e1,etat* e2){
	SDL_DestroyWindow(fenetre);
	SDL_DestroyRenderer(rndr);
	TTF_CloseFont(police);
	detruireEtat(e1);
	detruireEtat(e2);
	SDL_Quit();
	TTF_Quit();
	return;
}

void jeuUnJoueur(){
	etat e;
	initialisation1J(&e);
	int ticks = SDL_GetTicks();
	while(!(e.fermeture)){
		if(e.iDescente >= e.delaiDescente || (e.descenteRapide && e.iDescente >= 25)){
			descenteAuto(&e);
			e.iDescente = 0;
		}
		appliqueCommandes1J(&e);
		if(e.affiche){		// On ne veut recharger l'affichage que si c'est nécessaire
			if(e.lignesPleines[0] == -1) affiche(&e,true,0);	// Cas où il n'y a pas de lignes pleines : on affiche le jeu normalement
			else{	// Il y a des lignes pleines : on les fait clignoter puis on les enlève
				afficheAnimationLignes(&e);
				supprimeLignes1J(&e);
				affiche(&e,true,0);
			}
		}
		e.iDescente += 1;
		while(SDL_GetTicks() < ticks + 2){ /* Attente active, 2ms (500Hz) */ }
		ticks = SDL_GetTicks();	
	}
	// Affichage des résultats du joueur dans le terminal
	cout << "SCORE : " << e.score << endl;
	cout << "LIGNES : " << e.lignes << endl;
	cout << "NIVEAU : " << e.niveau << endl;
	fermeture1J(&e);
	return;
} 

/* Encore incomplet */
void jeuDeuxJoueurs(){
	etat e1,e2;
	initialisation2J(&e1,&e2);
	int ticks = SDL_GetTicks();
	while(!(e1.fermeture && e2.fermeture)){
		if(e1.iDescente >= e1.delaiDescente || (e1.descenteRapide && e1.iDescente >= 25)){
			descenteAuto(&e1);
			e1.iDescente = 0;
		}
		if(e2.iDescente >= e2.delaiDescente || (e2.descenteRapide && e2.iDescente >= 25)){
			descenteAuto(&e2);
			e2.iDescente = 0;
		}
		if(e1.lignesPleines[0] != -1) supprimeLignes2J(&e1,&e2);
		if(e2.lignesPleines[0] != -1) supprimeLignes2J(&e2,&e1);
		appliqueCommandes2J(&e1,&e2);
		// Si un joueur a perdu, il patiente devant un "écran de fin"
		if(e1.fermeture) grillePerdant(&e1);
		if(e2.fermeture) grillePerdant(&e2);
		// TODO : animation de suppression des lignes en mode 2 joueurs
		if(e1.affiche || e2.affiche){
			affiche2J(&e1,&e2);
		}
		e1.iDescente += 1;
		e2.iDescente += 1;
		while(SDL_GetTicks() < ticks + 2){ /* Attente active, 2ms (500Hz) */ }
		ticks = SDL_GetTicks();
	}
	// TODO : Afficher les scores/le gagnant dans le terminal en fin de partie
	fermeture2J(&e1,&e2);
	return;
}

int main(){
	int joueurs = 1;
	cfg.open("teL3tris.cfg");
	cfg >> joueurs;
	if(joueurs == 1) jeuUnJoueur();	// MODE UN JOUEUR (Cas par défaut si le fichier de configuration est absent)
	else if(joueurs == 2)jeuDeuxJoueurs();	// MODE DEUX JOUEURS
	// Sinon, l'utilsateur a mis n'importe quoi dans le fichier de configuration, et il ne se passe rien
	cfg.close();
	return 0;
}
