 #include <SDL2/SDL.h>
 #include <SDL2/SDL_ttf.h>
 #include "grille.h"
 #include "tetrominos.h"
 #include "affichage.h"
 #include <sstream>
 #include <iostream>
 using namespace std;
 
// Variables pour SDL
SDL_Window* fenetre;
SDL_Renderer* rndr;

// Police d'affichage pour SDL_ttf
TTF_Font* police;

const int TLBC = 30;	// Taille d'un bloc, en pixels (le texte est difficile à lire si cette taille est trop petite)
const int MARGE = 8;	// Taille des marges entre les grilles et les bords de la fenêtre, en pixels

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
		case 8:		// GRIS, sert pour la réserve lorsqu'elle n'est pas utilisable, et pour les blocs d'attaque en mode 2 joueurs
			SDL_SetRenderDrawColor(rndr,64,64,64,255);
			break;
		case 9:		// BRILLE, sert pour l'animation de suppression des lignes
			SDL_SetRenderDrawColor(rndr,200,200,200,255);
			break;
		default:	// Erreur, on affiche du blanc pour que ce soit visible...
			SDL_SetRenderDrawColor(rndr,255,255,255,255);
			break;
		}
}

/* Dessine "en vrac" le tétromino d'indice idTetro, à offset_x+1 pixels du bord gauche de la fenêtre,
 * et offset_y+1 pixels du bord haut de la fenêtre, sans mettre tout seul de grille autour
 * (Les "+1" servent à ne pas chevaucher le quadrillage) 
 * rota correspond au nombre de rotations horaires horaires effectuées par le tétromino
 * Le booléen "gris" force l'affichage du tétromino en gris
 * Si le booléen "creux" est vrai, seuls les contours des blocs du tétromino seront affichés */
void dessineTetro(int idTetro,int offset_x,int offset_y,int rota,bool gris,bool creux){
	if(idTetro != VIDE){		// Sinon, il n'y a rien à faire
		for(int i = 0;i < 4;i++){
			for(int j = 0;j < 4;j++){
				int clr = blocT(tetro(idTetro,rota),i,j);
				if(clr != VIDE){
					if(gris) clr = GRIS;	// Force l'affichage en gris si c'est demandé (sert pour la réserve lorsqu'elle n'est pas utilisable)
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
	}
	return;
}

/* Dessine la grille principale et tous ses blocs (ceux fixes et ceux appartenant au tétromino courant)
 * offset_x et offset_y représentent le décalage entre le bord de la grille et celui de la fenêtre, en pixels
 * Le tétromino courant et son fantôme sont affichés SSI tetroCourant est vrai */
void dessineGrillePrincipale(etat* e,int offset_x,int offset_y,bool tetroCourant){
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
				SDL_RenderFillRect(rndr,&rect);
			}
		}
	}
	if(tetroCourant){
		// Affichage du tétromino courant :
		dessineTetro(e->idTetro,offset_x + (e->x)*TLBC,offset_y + (e->y)*TLBC,e->rota,false,false);
		// Affichage du "fantôme" du tétromino courant :
		int dy = offsetFantome(e);
		dessineTetro(e->idTetro,offset_x + TLBC*(e->x),offset_y + TLBC*(e->y + dy),e->rota,false,true);
	}
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
 * Peut en théorie afficher 7 jusqu'à tétrominos, en pratique ça prendrait trop de place dans l'affichage */
void dessineSuivants(etat* e,int nb,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,nb*4,true);
	for(int k = 1;k < nb;k++){	// Affichage d'une "ligne plus blanche" entre les emplacements des différents tétrominos
		SDL_SetRenderDrawColor(rndr,255,255,255,255);
		SDL_RenderDrawLine(rndr,offset_x,offset_y + 4*k*TLBC,offset_x + 4*TLBC,offset_y + 4*k*TLBC);
	}
	for(int k = 0;k < nb;k++){
		//dessineTetro(e->suivants[k],offset_x,offset_y + 4*k*TLBC,0,false,false);	// ANCIENNE IMPLÉMENTATION DES TÉTROMINOS SUIVANTS
		dessineTetro(SUIVANTS[(e->idProchain)+k],offset_x,offset_y + 4*k*TLBC,0,false,false);
	}
	return;
}

/* Dessine le chiffre i (compris entre 0 et 9) à l'emplacement défini par les offsets */
void dessineChiffre(int i,int offset_x,int offset_y){
	SDL_Surface* txti;
	switch(i){	// Cette indigne abomination est nécessaire car SDL_RenderText_Shaded veut absolument un texte constant en argument...
		case(1):
			txti = TTF_RenderText_Shaded(police,"1",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(2):
			txti = TTF_RenderText_Shaded(police,"2",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(3):
			txti = TTF_RenderText_Shaded(police,"3",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(4):
			txti = TTF_RenderText_Shaded(police,"4",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(5):
			txti = TTF_RenderText_Shaded(police,"5",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(6):
			txti = TTF_RenderText_Shaded(police,"6",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(7):
			txti = TTF_RenderText_Shaded(police,"7",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(8):
			txti = TTF_RenderText_Shaded(police,"8",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		case(9):
			txti = TTF_RenderText_Shaded(police,"9",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
		default:	// Si i n'est pas entre 1 et 9, on affiche un 0
			txti = TTF_RenderText_Shaded(police,"0",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
			break;
	}
	SDL_Texture* texti = SDL_CreateTextureFromSurface(rndr,txti);
	SDL_Rect srci{0,0,txti->w,txti->h};
	SDL_Rect dsti{offset_x,offset_y,txti->w,txti->h};
	SDL_RenderCopy(rndr,texti,&srci,&dsti);
	SDL_DestroyTexture(texti);
	SDL_FreeSurface(txti);
	return;
}

/* Dessine les nbChiffres derniers chiffres de l'entier n, à la position définie par les offsets */
void dessineNombre(int n,int nbChiffres,int offset_x,int offset_y){
	for(int i = 0;i < nbChiffres;i++){
		dessineChiffre(n%10,offset_x+((nbChiffres-i-1)*TLBC/4),offset_y);
		n /= 10;
	}
	return;
}

/* Dessine une grille sans quadrillage, contenant le score et le nombre de lignes supprimmées par le joueur, sous forme de texte */
void dessineLignesScore(etat* e,int offset_x,int offset_y){
	dessineGrille(offset_x,offset_y,4,2,false);
	if(police != NULL){		// On ne veut pas essayer d'écrire si l'ouverture de la police d'écriture a échoué
		SDL_Surface* txtScore = TTF_RenderText_Shaded(police,"Score : ",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
		SDL_Surface* txtNiveau = TTF_RenderText_Shaded(police,"Niveau : ",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
		SDL_Surface* txtLignes = TTF_RenderText_Shaded(police,"Lignes : ",SDL_Color{255,255,255,0},SDL_Color{0,0,0,0});
		SDL_Texture* ttxtScore = SDL_CreateTextureFromSurface(rndr,txtScore);
		SDL_Texture* ttxtNiveau = SDL_CreateTextureFromSurface(rndr,txtNiveau);
		SDL_Texture* ttxtLignes = SDL_CreateTextureFromSurface(rndr,txtLignes);	
		SDL_Rect srcScr{0,0,txtScore->w,txtScore->h};
		SDL_Rect srcNiv{0,0,txtNiveau->w,txtNiveau->h};
		SDL_Rect srcLin{0,0,txtLignes->w,txtLignes->h};
		SDL_Rect dstScr{offset_x+3,offset_y+1,txtScore->w,txtScore->h};
		SDL_Rect dstNiv{offset_x+3,offset_y+1+2*TLBC/3,txtLignes->w,txtLignes->h};
		SDL_Rect dstLin{offset_x+3,offset_y+1+4*TLBC/3,txtLignes->w,txtLignes->h};
		SDL_RenderCopy(rndr,ttxtScore,&srcScr,&dstScr);
		SDL_RenderCopy(rndr,ttxtNiveau,&srcNiv,&dstNiv);
		SDL_RenderCopy(rndr,ttxtLignes,&srcLin,&dstLin);
		dessineNombre(e->score,6,offset_x+txtScore->w + TLBC/6,offset_y+1);
		dessineNombre(e->niveau,2,offset_x+txtNiveau->w + TLBC/6,offset_y+1+2*TLBC/3);
		dessineNombre(e->lignes,3,offset_x+txtLignes->w + TLBC/6,offset_y+1+4*TLBC/3);
		SDL_FreeSurface(txtScore);
		SDL_FreeSurface(txtNiveau);
		SDL_FreeSurface(txtLignes);
	}
	return;
}

/* Fonction auxilliaire de affiche1J et affiche2J, s'occupe de dessiner l'aire de jeu et de rafraîchir la fenêtre
 * Si clear est vrai, nettoie la fenêtre avant de dessiner dedans, puis l'envoie à l'affichage une fois le dessin effectué */
void afficheAux(etat* e,int offset_x,bool clear){
	if(clear){
		SDL_SetRenderDrawColor(rndr,0,0,0,0);
		SDL_RenderClear(rndr);
	}
	dessineGrillePrincipale(e,MARGE + offset_x,MARGE,(e->progresAnimationLignes == -1));		// On ne veut pas afficher le tétromino courant si l'animation de suppression des lignes est en cours
	dessineReserve(e,(LARG+1)*TLBC + MARGE + offset_x,MARGE);
	dessineLignesScore(e,(LARG+1)*TLBC + MARGE + offset_x,5*TLBC + MARGE);
	dessineSuivants(e,3,(LARG+1)*TLBC + MARGE + offset_x,8*TLBC + MARGE);
	if(clear){
		SDL_RenderPresent(rndr);	// NE SEMBLE TOUJOURS PAS FONCTIONNER SOUS X11
	}
	return;
}

/* Autre fonction auxiliaire de affiche1J et affiche2J, s'occupe de l'animation de suppression des lignes,
 * et ne contient pas d'attente active (donc ne bloque pas l'autre joueur en mode 2 joueurs) */
void afficheAnim(etat* e){
	if((e -> progresAnimationLignes) % (TMPS_ANIM*2) == 0){
		// Passage en "couleur qui BRILLE" des lignes pleines
		for(int k = 0;k < 4;k++){
			if(e -> lignesPleines[k] != -1){		// Il peut tout à fait y avoir moins de 4 lignes pleines, et il y a alors moins de blocs à changer de couleur
				for(int i = 0;i < LARG;i++){
					ecritBlocG(e,i,e->lignesPleines[k],BRILLE);
				}
			}
		}
		e -> affiche = true;
	}else if((e -> progresAnimationLignes) % TMPS_ANIM == 0){
		// Repassage en "couleurs normales", récupérées dans le tableau prévu à cet effet (e->copieLignesAnimation) :
		for(int k = 0;k < 4;k++){
			if(e -> lignesPleines[k] != -1){	// Même remarque qu'une dizaine de lignes au dessus
				for(int i = 0;i < LARG;i++){
					ecritBlocG(e,i,e->lignesPleines[k],blocC(e,i,k));
				}
			}
		}
		e -> affiche = true;
	}
	e -> progresAnimationLignes -= 1;
	return;
}

/* Détermine si la fenêtre doit être rafraîchie, et ne la fait rafraîchir que si
 * e->affiche est vrai, OU si l'animation de suppression des lignes doit avancer */
void affiche1J(etat* e){
	if(e -> affiche){	// Sinon, il n'y a rien de nouveau à afficher...
		e -> affiche = false;
		afficheAux(e,0,true);
	}else if((e -> progresAnimationLignes) != -1){	// ... sauf dans le cas particulier de l'animation de suppression des lignes :
		afficheAnim(e);
	}	// Sinon, on attend
	return;
}

/* Version 2 joueurs de la fonction d'affichage, tient avec du scotch */
void affiche2J(etat* e1,etat* e2){
	int offsetJ2 = TLBC * (LARG+7);
	
	if(e1->progresAnimationLignes != -1) afficheAnim(e1);
	if(e2->progresAnimationLignes != -1) afficheAnim(e2);
	
	if((e1 -> affiche) || (e2 -> affiche)){
		SDL_SetRenderDrawColor(rndr,0,0,0,0);
		SDL_RenderClear(rndr);
		e1 -> affiche = false;
		afficheAux(e1,0,false);
		e2 -> affiche = false;
		afficheAux(e2,offsetJ2,false);
		SDL_RenderPresent(rndr);
	}
	return;
}
