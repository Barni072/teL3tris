#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "grille.h"

extern SDL_Window* fenetre;
extern SDL_Renderer* rndr;
extern const int TLBC;

//void dessineGrille(int x,int y,int largeur,int hauteur);
//int couleurBloc(int clr);
//void afficheTetro(int idTetro,int offset_x,int offset_y,int rota,bool gris);
//void afficheGrillePrincipale(etat* e,int offset_x,int offset_y);
//void afficheReserve(etat* e,int offset_x,int offset_y);
//void afficheSuivants(etat* e,int nb,int offset_x,int offset_y);
void affiche(etat* e);

#endif 
