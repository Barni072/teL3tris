#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "grille.h"

extern SDL_Window* fenetre;
extern SDL_Renderer* rndr;
extern const int TLBC;
extern const int MARGE;

//void dessineGrille(int x,int y,int largeur,int hauteur,bool quadri);
//int couleurBloc(int clr);
//void dessineTetro(int idTetro,int offset_x,int offset_y,int rota,bool gris);
//void dessineGrillePrincipale(etat* e,int offset_x,int offset_y,bool tetroCourant);
//void dessineReserve(etat* e,int offset_x,int offset_y);
//void dessineSuivants(etat* e,int nb,int offset_x,int offset_y);
//void dessineLignesScore(etat* e,int offset_x,int offset_y);
void affiche(etat* e,bool tetroCourant);
void afficheAnimationLignes(etat* e);

#endif 
