#ifndef TETROMINO_H
#define TETROMINO_H

extern const int I;
extern const int O;
extern const int T;
extern const int L;
extern const int J;
extern const int Z;
extern const int S;
extern const int GRIS;
extern const int VIDE;
extern const int TETROMINOS[448];

const int* tetro(int id,int rot);
int blocT(const int* t,int i,int j);

#endif
