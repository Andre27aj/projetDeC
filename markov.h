#ifndef MARKOV_H
#define MARKOV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Cellule {
    int sommet_arrivee;
    float proba;
    struct Cellule *suiv;
} Cellule;

typedef struct Liste {
    Cellule *head;
} Liste;

typedef struct ListeAdj {
    Liste *tab;
    int taille;
} ListeAdj;

Cellule *creerCellule(int arrivee, float proba);
Liste creerListe(void);
void ajouterCellule(Liste *l, int arrivee, float proba);
ListeAdj creerListeAdj(int taille);
void afficherListe(const Liste *l);
void afficherListeAdj(const ListeAdj *g);
ListeAdj readMarkovGraph(const char *filename);
int estGrapheMarkov(const ListeAdj *g);
char *getIdMarkov(int num);
void genererMermaid(const ListeAdj *g, const char *filename);

#endif
