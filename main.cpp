#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include <cstdlib>
#include "affichage.h"
#include "grille.h"
#include "entree.h"

/* Initialise proprement les structures utilisées */
void initialisation(etat* e){
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	TTF_Init();
	fenetre = SDL_CreateWindow("Tetris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,(LARG+5)*TLBC + 2*MARGE,HAUT*TLBC + 2*MARGE,SDL_WINDOW_SHOWN);
	rndr = SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	srand(time(NULL));
	initEtat(e);
	return;
}

/* Détruit proprement les structures utilisées */
void fermeture(etat* e){
	SDL_DestroyWindow(fenetre);
	SDL_DestroyRenderer(rndr);
	detruireEtat(e);
	SDL_Quit();
	TTF_Quit();
	return;
}

int main(){
	etat e;		// Pour l'instant pas une variable globale, plusieurs états pourraient coexister selon ce qu'on décide par la suite...
	initialisation(&e);
	int ticks = SDL_GetTicks();
	while(!(e.fermeture)){
		if(e.iDescente >= e.delaiDescente || (e.descenteRapide && e.iDescente >= 25)){
			// ">=" nécessaire pour ne pas rester bloqué lorsque e.delaiDescente change
			// Descente rapide fixée (manuellement...) la même vitesse que le niveau 20
			descenteAuto(&e);
			e.iDescente = 0;
		}
		appliqueCommandes(&e);
		if(e.affiche){		// On ne veut recharger l'affichage que si c'est nécessaire
			if(e.lignesPleines[0] == -1) affiche(&e,true);	// Cas où il n'y a pas de lignes pleines, on affiche le jeu normalement
			else{	// Il y a des lignes pleines, on les fait clignoter puis on les enlève
				afficheAnimationLignes(&e);
				supprimeLignes(&e);
				affiche(&e,true);
			}
		}
		e.iDescente += 1;
		while(SDL_GetTicks() < ticks + 2){ /* Attente active, 2ms (500Hz) */ }
		ticks = SDL_GetTicks();		
	}
	fermeture(&e);
	return 0;
} 
