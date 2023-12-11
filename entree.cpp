#include <SDL2/SDL.h>
#include "entree.h"
#include "grille.h"
 
SDL_Event evnt;
 
/* Doit être appelé à chaque itération de la boucle de jeu principale, vérifie les commandes et déclenche les actions associées */
void appliqueCommandes1J(etat* e){
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
						case(SDLK_l):	// L
							// Rotation directe
							rotation(e,false);
							break;
						case(SDLK_p):	// P
							// Rotation indirecte
							rotation(e,true);
							break;
						case(SDLK_c):	// C
							reserve(e);
							break;
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
// TODO : Refaire cette fonction avec des commandes non hardcodées, et implémenter une pause ?

void appliqueCommandes2J(etat* e1,etat* e2){
	if(SDL_PollEvent(&evnt)){
		switch(evnt.type){
				// Fermeture du jeu
				case(SDL_QUIT):
				case(SDL_WINDOWEVENT_CLOSE):
					e1 -> fermeture = true;
					e2 -> fermeture = true;
					// C'est redondant, mais tant pis
					break;
				// Touche enfoncée
				case(SDL_KEYDOWN):
					switch(evnt.key.keysym.sym){
						case(SDLK_q):	// Q
							// Translation gauche e1
							translation(e1,3);
							break;
						case(SDLK_d):	// D
							// Translation droite e1
							translation(e1,1);
							break;
						case(SDLK_s):	// S
							// Descente rapide e1
							descenteRapide(e1,true);
							break;
						case(SDLK_z):	// Z
							// Descente immédiate e1
							descenteImmediate(e1);
							break;
						case(SDLK_c):	// C
							// Rotation directe e1
							rotation(e1,false);
							break;
						case(SDLK_f):	// F
							// Rotation indirecte e1
							rotation(e1,true);
							break;
						case(SDLK_x):	// X
							reserve(e1);
							break;
						
						case(SDLK_j):	// J
							// Translation gauche e2
							translation(e2,3);
							break;
						case(SDLK_l):	// L
							// Translation droite e2
							translation(e2,1);
							break;
						case(SDLK_k):	// K
							// Descente rapide e2
							descenteRapide(e2,true);
							break;
						case(SDLK_i):	// I
							// Descente immédiate e2
							descenteImmediate(e2);
							break;
						case(SDLK_COLON):	// ':'
							// Rotation directe e2
							rotation(e2,false);
							break;
						case(SDLK_m):	// M
							// Rotation indirecte e2
							rotation(e2,true);
							break;
						case(SDLK_SEMICOLON):	// ';'
							reserve(e2);
							break;
						
						default:
							break;
					}		
					break;
			case(SDL_KEYUP):
				// Fin de la descente rapide : 
				if(evnt.key.keysym.sym == SDLK_s) descenteRapide(e1,false);
				else if(evnt.key.keysym.sym == SDLK_k) descenteRapide(e2,false);
			}
	}
	return;
}
