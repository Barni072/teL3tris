#include <SDL2/SDL.h>
#include "entree.h"
#include "grille.h"
 
 SDL_Event evnt;
 
/* Devra être appelé à chaque "tick", vérifie les commandes et déclenche les actions associées */
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
						/*case(SDLK_r):	// R
							// DEBUG, force le tétromino suivant, il faudra enlever ça à terme
							tetrominoSuivant(e);
							break;*/
						case(SDLK_q):	// Q
							// Translation gauche
							translation(e,3);
							break;
						case(SDLK_d):	// D
							// Translation droite
							translation(e,1);
							break;
						case(SDLK_s):	// S
							// Descente rapide
							descenteRapide(e,true);
							break;
						case(SDLK_z):	// Z
							// Descente immédiate
							descenteImmediate(e);
							break;
						case(SDLK_a):	// A
							// Rotation directe
							rotation(e,false);
							break;
						case(SDLK_e):	// E
							// Rotation indirecte
							rotation(e,true);
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
			case(SDL_KEYUP):
				if(evnt.key.keysym.sym == SDLK_s) descenteRapide(e,false);	// Fin de la descente rapide
			}
	}
	return;
}
// TODO : Refaire cette fonction avec des commandes non hardcodées ?
