// Fichier .h pour la gestion des graphes de Markov - Partie 2
// Groupe Projet : André, Adam, Clément et Roman
#ifndef MARKOV_H
#define MARKOV_H

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
