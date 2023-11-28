#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>
#include "affichage.h"
#include "grille.h"
#include "entree.h"

/* Initialise proprement les structures utilisées */
void initialisation(etat* e){
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	fenetre = SDL_CreateWindow("Tetris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,550,550,SDL_WINDOW_SHOWN);
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
	return;
}

int main(){
	etat e;		// Pour l'instant pas une variable globale, plusieurs états pourraient coexister selon ce qu'on décide par la suite...
	initialisation(&e);
	int ticks = SDL_GetTicks();
	while(!(e.fermeture)){
		if(e.iDescente >= e.delaiDescente || (e.descenteRapide && e.iDescente >= 5)){
			// ">=" nécessaire pour ne pas rester bloqué lorsque e.delaiDescente change
			// Descente rapide fixée ici à 100 Hz (tous les 5 ticks)
			descenteAuto(&e);
			e.iDescente = 0;
		}
		appliqueCommandes(&e);
		if(e.affiche) affiche(&e);		// Ne recharge l'affichage que si c'est nécessaire
		e.iDescente += 1;
		while(SDL_GetTicks() < ticks + 2){ /* Attente active, 2ms (500Hz) */ }
		ticks = SDL_GetTicks();		
	}
	fermeture(&e);
	return 0;
} 
