 #include <SDL2/SDL.h>
 #include "grille.h"
 
 // Variables pour SDL
SDL_Window* fenetre;
SDL_Renderer* rndr;
 
// Taille d'un bloc, en pixels
const int TLBC = 25;
 
 /* Affiche une grille avec quadrillage, destinée à être remplie par des blocs
 * La grille est blanche, le quadrillage est gris
 * x et y sont les coordonnées du pixel en haut à gauche de la grille
 * La largeur et la hauteur sont exprimées en nombre de blocs */
void dessineGrille(int x,int y,int largeur,int hauteur){
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = largeur * TLBC + 1;
	rect.h = hauteur * TLBC + 1;
	SDL_SetRenderDrawColor(rndr,255,255,255,255);
	SDL_RenderDrawRect(rndr,&rect);
	SDL_SetRenderDrawColor(rndr,127,127,127,255);
	for(int i = 1;i < largeur;i++){
		int x_col = x + i * TLBC;
		SDL_RenderDrawLine(rndr,x_col,y+1,x_col,y-1 + hauteur*TLBC);
	}
	for(int j = 1;j < hauteur;j++){
		int y_lin = y + j* TLBC;
		SDL_RenderDrawLine(rndr,x+1,y_lin,x-1 + largeur*TLBC,y_lin);
	}
	return;
}

/* Fait un SDL_SetRenderDrawColor avec la couleur associée à l'indice clr */
void couleurBloc(int clr){
	switch(clr){
		case 1:		// O, jaune
			SDL_SetRenderDrawColor(rndr,254,251,52,255);
			break;
		case 2:		// I, bleu clair
			SDL_SetRenderDrawColor(rndr,1,237,250,255);
			break;
		case 3:		// T, violet
			SDL_SetRenderDrawColor(rndr,221,10,178,255);
			break;
		case 4:		// S, vert
			SDL_SetRenderDrawColor(rndr,0,255,0,255);
			break;
		case 5:		// J, bleu foncé
			SDL_SetRenderDrawColor(rndr,0,0,255,255);
			break;
		case 6:		// Z, rouge
			SDL_SetRenderDrawColor(rndr,255,0,0,255);
			break;
		case 7:		// L, orange
			SDL_SetRenderDrawColor(rndr,250,70,25,255);
			break;
		default:	// Erreur, on affiche du blanc pour que ce soit visible...
			SDL_SetRenderDrawColor(rndr,255,255,255,255);
			break;
		}
}

/* Affiche la grille principale et tous ses blocs (ceux fixes et ceux appartenant au tétromino courant)
 * x et y sont les coordonnées du pixel en haut à gauche de la grille */
void grillePrincipale(etat* e){
	int offset_x = 5;	// Choisi en vrac arbitrairement
	int offset_y = 5;	// Idem
	dessineGrille(offset_x,offset_y,LARG,HAUT);
	// Affichage des blocs fixes :
	for(int i = 0;i < LARG;i++){
		for(int j = 0;j < HAUT;j++){
			int clr = blocG(e,i,j);
			if(clr != VIDE){		// On veut afficher un bloc seulement s'il y en a un
				SDL_Rect rect;
				rect.x = offset_x + i*TLBC+1;
				rect.y = offset_y + j*TLBC+1;
				rect.w = TLBC-1;
				rect.h = TLBC-1;
				couleurBloc(clr);
				//SDL_SetRenderDrawColor(rndr,0,255,0,255);	// 1ÈRE VERSION, tous les blocs sont verts
				SDL_RenderFillRect(rndr,&rect);
			}
		}
	}
	// Affichage des blocs du tétromino courant :
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			int clr = blocT(tetro(e->id_tetro),i,j);
			if(clr != VIDE){
				SDL_Rect rect;
				rect.x = offset_x + ((e->x)+i)*TLBC+1;
				rect.y = offset_y + ((e->y)+j)*TLBC+1;
				rect.w = TLBC-1;
				rect.h = TLBC-1;
				couleurBloc(clr);
				SDL_RenderFillRect(rndr,&rect);
			}
		}
	}
	return;
}

/* TEMPORAIRE ! Affiche un peu en vrac un exemple de ce qui est déjà implémenté */
void afficheTemp(etat* e){
	SDL_SetRenderDrawColor(rndr,0,0,0,0);
	SDL_RenderClear(rndr);
	// T
	e->g[176] = 3;
	e->g[165] = 3;
	e->g[175] = 3;
	e->g[185] = 3;
	// S
	e->g[163] = 4;
	e->g[173] = 4;
	e->g[174] = 4;
	e->g[184] = 4;
	// Z
	e->g[183] = 6;
	e->g[182] = 6;
	e->g[171] = 6;
	e->g[172] = 6;
	// O
	e->g[197] = 1;
	e->g[198] = 1;
	e->g[187] = 1;
	e->g[188] = 1;
	// L
	e->g[194] = 7;
	e->g[195] = 7;
	e->g[196] = 7;
	e->g[186] = 7;
	// J
	e->g[181] = 5;
	e->g[191] = 5;
	e->g[192] = 5;
	e->g[193] = 5;
	// I
	e->g[160] = 2;
	e->g[170] = 2;
	e->g[180] = 2;
	e->g[190] = 2;
	grillePrincipale(e);
	SDL_RenderPresent(rndr);		// MARCHE SEULEMENT SOUS WAYLAND !!
	return;
}
