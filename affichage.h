#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "grille.h"

extern SDL_Window* fenetre;
extern SDL_Renderer* rndr;
extern TTF_Font* police;
extern const int TLBC;
extern const int MARGE;

//void dessineGrille(int x,int y,int largeur,int hauteur,bool quadri);
//int couleurBloc(int clr);
//void dessineTetro(int idTetro,int offset_x,int offset_y,int rota,bool gris);
//void dessineGrillePrincipale(etat* e,int offset_x,int offset_y,bool tetroCourant);
//void dessineReserve(etat* e,int offset_x,int offset_y);
//void dessineSuivants(etat* e,int nb,int offset_x,int offset_y);
//void dessineLignesScore(etat* e,int offset_x,int offset_y);
//void dessineChiffre(int i,int offset_x,int offset_y);
//void dessineNombre(int n,int nbChiffres,int offset_x,int offset_y);
//void afficheAux(etat* e,int offset_x,bool clear);
//void afficheAnim(etat* e);
void affiche1J(etat* e);
void affiche2J(etat* e1,etat* e2);

#endif 
