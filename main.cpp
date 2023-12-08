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

/* Initialise proprement les structures utilisées */
void initialisation1J(etat* e){
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	TTF_Init();
	fenetre = SDL_CreateWindow("teL3tris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,(LARG+5)*TLBC + 2*MARGE,HAUT*TLBC + 2*MARGE,SDL_WINDOW_SHOWN);
	rndr = SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	srand(time(NULL));
	initEtat(e);
	// Récupération du niveau initial dans le fichier de configuration :
	ifstream cfg ("teL3tris.cfg");
	cfg >> e -> niveau;
	changeVitesse(e);
	//cout << "NIVEAU INITIAL : " << e->niveau << endl;		// DEBUG
	cfg.close();
	// Ici : initialisation des commandes ? (APRÈS celle de l'état !)
	return;
}
void initialisation2J(etat* e1,etat* e2){
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	TTF_Init();
	// IL FAUT CHANGER LA TAILLE DE LA FENÊTRE :
	fenetre = SDL_CreateWindow("teL3tris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,2*(LARG+5)*TLBC + 2*(MARGE+TLBC),HAUT*TLBC + 2*MARGE,SDL_WINDOW_SHOWN);
	rndr = SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	srand(time(NULL));
	initEtat(e1);
	initEtat(e2);
	// Récupération du niveau initial dans le fichier de configuration :
	ifstream cfg ("teL3tris.cfg");
	cfg >> e1 -> niveau;
	cfg >> e2 -> niveau;
	changeVitesse(e1);
	changeVitesse(e2);
	cfg.close();
	// Ici : initialisation des commandes ? (APRÈS celle de l'état !)
	return;
}

/* Détruit proprement les structures utilisées */
void fermeture1J(etat* e){
	SDL_DestroyWindow(fenetre);
	SDL_DestroyRenderer(rndr);
	detruireEtat(e);
	SDL_Quit();
	TTF_Quit();
	return;
}
void fermeture2J(etat* e1,etat* e2){
	SDL_DestroyWindow(fenetre);
	SDL_DestroyRenderer(rndr);
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
			// Descente rapide fixée (manuellement...) à la même vitesse que le niveau 20
			descenteAuto(&e);
			e.iDescente = 0;
		}
		appliqueCommandes1J(&e);
		if(e.affiche){		// On ne veut recharger l'affichage que si c'est nécessaire
			if(e.lignesPleines[0] == -1) affiche(&e,true,0);	// Cas où il n'y a pas de lignes pleines : on affiche le jeu normalement
			else{	// Il y a des lignes pleines : on les fait clignoter puis on les enlève
				afficheAnimationLignes(&e);
				supprimeLignes(&e);
				affiche(&e,true,0);
			}
		}
		e.iDescente += 1;
		while(SDL_GetTicks() < ticks + 2){ /* Attente active, 2ms (500Hz) */ }
		ticks = SDL_GetTicks();	
	}
	fermeture1J(&e);
	return;
} 

void jeuDeuxJoueurs(){
	etat e1,e2;
	initialisation2J(&e1,&e2);
	int ticks = SDL_GetTicks();
	while(!(e1.fermeture || e2.fermeture)){
		// IL FAUT RAJOUTER LA SUPPRESSION DES LIGNES !!! (sinon segfault)
		if(e1.iDescente >= e1.delaiDescente || (e1.descenteRapide && e1.iDescente >= 25)){
			// Descente rapide fixée (manuellement...) à la même vitesse que le niveau 20
			descenteAuto(&e1);
			e1.iDescente = 0;
		}
		if(e2.iDescente >= e2.delaiDescente || (e2.descenteRapide && e2.iDescente >= 25)){
			// Descente rapide fixée (manuellement...) à la même vitesse que le niveau 20
			descenteAuto(&e2);
			e2.iDescente = 0;
		}
		appliqueCommandes2J(&e1,&e2);
		// TODO : animation de suppression des lignes en mode 2 joueurs
		if(e1.affiche || e2.affiche){
			affiche2J(&e1,&e2);
		}
		e1.iDescente += 1;
		e2.iDescente += 1;
		while(SDL_GetTicks() < ticks + 2){ /* Attente active, 2ms (500Hz) */ }
		ticks = SDL_GetTicks();
	}
	fermeture2J(&e1,&e2);
	return;
}

int main(){
	jeuUnJoueur();
	//jeuDeuxJoueurs();
	return 0;
}
