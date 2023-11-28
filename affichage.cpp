 #include <SDL2/SDL.h>
 #include "grille.h"
 #include "tetrominos.h"
 
// Variables pour SDL
SDL_Window* fenetre;
SDL_Renderer* rndr;
 
// Taille d'un bloc, en pixels
const int TLBC = 25;
 
 /* Affiche une grille avec quadrillage, destinée à être remplie par des blocs
 * La grille est blanche, le quadrillage est gris
 * x et y sont les coordonnées du pixel en haut à gauche de la grille
 * La largeur et la hauteur sont exprimées EN NOMBRE DE BLOCS */
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
		case 8:		// GRIS, sert pour la réserve lorsqu'elle n'est pas utilisable, et éventuellement pour les blocs de "garbage" en 1v1
			SDL_SetRenderDrawColor(rndr,64,64,64,255);
			break;
		default:	// Erreur, on affiche du blanc pour que ce soit visible...
			SDL_SetRenderDrawColor(rndr,255,255,255,255);
			break;
		}
}

/* Dessine "en vrac" le tétromino d'indice idTetro, à offset_x+1 pixels du bord gauche de la fenêtre,
 * et offset_y+1 pixels du bord haut de la fenêtre, sans mettre tout seul de grille autour
 * rota correspond au nombre de rotations effectuées par le tétromino
 * (Les "+1" servent à ne pas chevaucher le quadrillage) 
 * Le booléen "gris" force l'affichage du tétromino en gris */
void afficheTetro(int idTetro,int offset_x,int offset_y,int rota,bool gris){
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			int clr = blocT(tetro(idTetro,rota),i,j);
			if(clr != VIDE){
				if(gris) clr = GRIS;	// Hack fumeux pour griser la réserve lorsqu'elle n'est pas utilisable
				SDL_Rect rect;
				rect.x = offset_x+1 + i*TLBC;
				rect.y = offset_y+1 + j*TLBC;
				rect.w = TLBC-1;
				rect.h = TLBC-1;
				couleurBloc(clr);
				SDL_RenderFillRect(rndr,&rect);
			}
		}
	}
	return;
}

/* Dessine la grille principale et tous ses blocs (ceux fixes et ceux appartenant au tétromino courant)
 * offset_x et offset_y représentent le décalage entre le bord de la grille et celui de la fenêtre, en pixels */
void afficheGrillePrincipale(etat* e,int offset_x,int offset_y){
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
	afficheTetro(e->idTetro,offset_x + (e->x)*TLBC,offset_y + (e->y)*TLBC,e->rota,false);
	return;
}

/* Dessine le tétromino stocké en réserve, s'il existe, dans une grille (avec les offsets, comme d'habitude) */
void afficheReserve(etat* e,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,4);
	if(e->reserve != VIDE){
		afficheTetro(e->reserve,offset_x,offset_y,0,!(e->reserveDispo));
	}
	return;
}
/* Dessine une grille contenant les nb premiers tétrominos suivants (avec les offsets habituels)
 * Peut en théorie afficher 7 tétrominos, en pratique ça prendrait trop de place */
void afficheSuivants(etat* e,int nb,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,nb*4);
	for(int k = 1;k < nb;k++){	// Affichage d'une "ligne plus blanche" entre les emplacements des différents tétrominos
		SDL_SetRenderDrawColor(rndr,255,255,255,255);
		SDL_RenderDrawLine(rndr,offset_x,offset_y + 4*k*TLBC,offset_x + 4*TLBC,offset_y + 4*k*TLBC);
	}
	for(int k = 0;k < nb;k++){
		afficheTetro(e->suivants[k],offset_x,offset_y + 4*k*TLBC,0,false);
	}
	return;
}

/* TEMPORAIRE ! Affiche un peu en vrac un exemple de ce qui est déjà implémenté
 * Forçait des blocs à réapparaître en bas de la grille... */
/*void afficheTemp(etat* e){
	e -> affiche = false;
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
	afficheGrillePrincipale(e,5,5);
	afficheReserve(e,280,5);
	afficheSuivants(e,3,280,130);
	SDL_RenderPresent(rndr);		// MARCHE SEULEMENT SOUS WAYLAND !!
	return;
}*/

void affiche(etat* e){
	e -> affiche = false;
	SDL_SetRenderDrawColor(rndr,0,0,0,0);
	SDL_RenderClear(rndr);
	afficheGrillePrincipale(e,5,5);
	afficheReserve(e,280,5);
	afficheSuivants(e,3,280,130);
	SDL_RenderPresent(rndr);	// NE SEMBLE TOUJOURS PAS FONCTIONNER SOUS X11...
	return;
}
