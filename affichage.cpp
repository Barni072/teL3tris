 #include <SDL2/SDL.h>
 #include <SDL2/SDL_ttf.h>
 #include "grille.h"
 #include "tetrominos.h"
 
// Variables pour SDL
SDL_Window* fenetre;
SDL_Renderer* rndr;
 
// Taille d'un bloc, en pixels
const int TLBC = 25;
 
 /* Affiche une grille avec ou sans quadrillage selon le booléen quadri
 * La grille est blanche, le quadrillage est gris
 * x et y sont les coordonnées du pixel en haut à gauche de la grille
 * La largeur et la hauteur sont exprimées EN NOMBRE DE BLOCS */
void dessineGrille(int x,int y,int largeur,int hauteur,bool quadri){
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = largeur * TLBC + 1;
	rect.h = hauteur * TLBC + 1;
	SDL_SetRenderDrawColor(rndr,255,255,255,255);
	SDL_RenderDrawRect(rndr,&rect);
	if(quadri){
		SDL_SetRenderDrawColor(rndr,127,127,127,255);
		for(int i = 1;i < largeur;i++){
			int x_col = x + i * TLBC;
			SDL_RenderDrawLine(rndr,x_col,y+1,x_col,y-1 + hauteur*TLBC);
		}
		for(int j = 1;j < hauteur;j++){
			int y_lin = y + j* TLBC;
			SDL_RenderDrawLine(rndr,x+1,y_lin,x-1 + largeur*TLBC,y_lin);
		}
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
 * Le booléen "gris" force l'affichage du tétromino en gris
 * Si le booléen "creux" est vrai, seuls les contours des blocs du tétromino seront affichés */
void dessineTetro(int idTetro,int offset_x,int offset_y,int rota,bool gris,bool creux){
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
				if(!creux) SDL_RenderFillRect(rndr,&rect);
				else SDL_RenderDrawRect(rndr,&rect);
			}
		}
	}
	return;
}

/* Dessine la grille principale et tous ses blocs (ceux fixes et ceux appartenant au tétromino courant)
 * offset_x et offset_y représentent le décalage entre le bord de la grille et celui de la fenêtre, en pixels */
void dessineGrillePrincipale(etat* e,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,LARG,HAUT,true);
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
	// Affichage du tétromino courant :
	dessineTetro(e->idTetro,offset_x + (e->x)*TLBC,offset_y + (e->y)*TLBC,e->rota,false,false);
	// Affichage du "fantôme" du tétromino courant :
	int dy = offsetFantome(e);
	dessineTetro(e->idTetro,offset_x + TLBC*(e->x),offset_y + TLBC*(e->y + dy),e->rota,false,true);
	return;
}

/* Dessine le tétromino stocké en réserve, s'il existe, dans une grille (avec les offsets, comme d'habitude) */
void dessineReserve(etat* e,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,4,true);
	if(e->reserve != VIDE){
		dessineTetro(e->reserve,offset_x,offset_y,0,!(e->reserveDispo),false);
	}
	return;
}
/* Dessine une grille contenant les nb premiers tétrominos suivants (avec les offsets habituels)
 * Peut en théorie afficher 7 tétrominos, en pratique ça prendrait trop de place */
void dessineSuivants(etat* e,int nb,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,nb*4,true);
	for(int k = 1;k < nb;k++){	// Affichage d'une "ligne plus blanche" entre les emplacements des différents tétrominos
		SDL_SetRenderDrawColor(rndr,255,255,255,255);
		SDL_RenderDrawLine(rndr,offset_x,offset_y + 4*k*TLBC,offset_x + 4*TLBC,offset_y + 4*k*TLBC);
	}
	for(int k = 0;k < nb;k++){
		dessineTetro(e->suivants[k],offset_x,offset_y + 4*k*TLBC,0,false,false);
	}
	return;
}

/* Dessine une grille sans quadrillage, contenant le score et le nombre de lignes supprimmées par le joueur, sous forme de texte
 * Pour l'instant, seule la grille est présente... */
void dessineLignesScore(etat* e,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,2,false);
	TTF_Font* font = TTF_OpenFont("JupiteroidRegular-Rpj6V.ttf",TLBC-5);	// fontspace.com/jupiteroid-font-f90261
	SDL_Surface* txtScore = TTF_RenderText_Shaded(font,"Score : ",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
	SDL_Surface* txtLignes = TTF_RenderText_Shaded(font,"Lignes : ",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
	SDL_Texture* ttxtLignes=SDL_CreateTextureFromSurface(rndr,txtLignes);
	SDL_Texture* ttxtScore=SDL_CreateTextureFromSurface(rndr,txtScore);
	SDL_Rect srcsc{ 0, 0, txtScore->w, txtScore->h };
	SDL_Rect srcln{ 0, 0, txtLignes->w, txtLignes->h };
	SDL_Rect dstsc{ offset_x+3, offset_y+1, txtScore->w, txtScore->h };
	SDL_Rect dstln{ offset_x+3, offset_y+1+TLBC, txtLignes->w, txtLignes->h };
	SDL_RenderCopy(rndr,ttxtLignes , &srcln, &dstln);
	SDL_RenderCopy(rndr,ttxtScore , &srcsc, &dstsc);
	SDL_FreeSurface(txtScore);
	SDL_FreeSurface(txtLignes);
	TTF_CloseFont(font);
	return;
}

void affiche(etat* e){	// Il faudra remplacer les offsets "numériques" par des choses un peu moins hardcodées...
	e -> affiche = false;
	SDL_SetRenderDrawColor(rndr,0,0,0,0);
	SDL_RenderClear(rndr);
	dessineGrillePrincipale(e,5,5);
	dessineReserve(e,280,5);
	dessineSuivants(e,3,280,130);
	dessineLignesScore(e,280,455);
	SDL_RenderPresent(rndr);	// NE SEMBLE TOUJOURS PAS FONCTIONNER SOUS X11...
	return;
}
