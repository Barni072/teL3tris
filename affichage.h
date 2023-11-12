#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "grille.h"

extern SDL_Window* fenetre;
extern SDL_Renderer* rndr;
extern const int TLBC;

//void dessineGrille(int x,int y,int largeur,int hauteur);
//int couleurBloc(int clr);
//void grillePrincipale(etat* e);
void afficheTemp(etat* e);

#endif 
