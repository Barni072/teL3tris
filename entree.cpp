#include <SDL2/SDL.h>
#include "entree.h"
#include "grille.h"
 
 SDL_Event evnt;
 
/* TEMPORAIRE ! */
void attendClavierVrac(){
	bool boucle = true;
	//SDL_Event evnt;
	while(boucle){
		if (SDL_PollEvent(&evnt)){
			switch(evnt.type){
				case(SDL_QUIT):
				case(SDL_WINDOWEVENT_CLOSE):
					boucle = false;
					break;
				case(SDL_KEYDOWN):
					boucle = false;
					break;
			}
		}
	}
	return;
}	// N'est plus utilisé à ce stade

/* TEMPORAIRE AUSSI, pourra et devra être amélioré, principalement avec l'ajout de DAS pour les translations, et potentiellement des commandes non hardcodées...
 * Devra être appelé à chaque "tick" ; vérifie les commandes et déclenche les actions associées */
void appliqueCommandes(etat* e){
	if(SDL_PollEvent(&evnt)){
		switch(evnt.type){
				// Fermeture du jeu
				case(SDL_QUIT):
				case(SDL_WINDOWEVENT_CLOSE):
					e -> fermeture = true;
					break;
				// Touche enfoncée
				case(SDL_KEYDOWN):
					switch(evnt.key.keysym.sym){
						case(SDLK_r):	// R
							// DEBUG, force le tétromino suivant, il faudra enlever ça à terme
							tetrominoSuivant(e);
							break;
						case(SDLK_q):	// Q
							// Translation gauche
							break;
						case(SDLK_d):	// D
							// Translation droite
							break;
						case(SDLK_s):	// S
							// Descente rapide
							break;
						case(SDLK_z):	// Z
							// Descente immédiate
							break;
						case(SDLK_a):	// A
							// Rotation directe
							break;
						case(SDLK_e):	// E
							// Rotation indirecte
							break;
						case(SDLK_c):	// C
							reserve(e);
							break;
						//case(SDLK		// Aucune idée de la façon dont sélectionner la touche espace...
							// Pause ?
							//break;
						default:
							break;
					}		
					break;
			}
	}
	return;
}

