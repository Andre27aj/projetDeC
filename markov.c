#include <stdlib.h>
#include <stdio.h>
#include "markov.h"

// Alloue et initialise une cellule pour une transition (arrivee, proba)
Cellule *creerCellule(int arrivee, float proba) {
    Cellule *c = (Cellule *)malloc(sizeof(Cellule));
    if (!c) { perror("Erreur d'allocation mémoire pour cellule"); exit(EXIT_FAILURE); }
    c->sommet_arrivee = arrivee;
    c->proba = proba;
    c->suiv = NULL;
    return c;
}

// Retourne une liste vide
Liste creerListe(void) {
    Liste l; l.head = NULL; return l;
}

// Insère une nouvelle transition en tête
void ajouterCellule(Liste *l, int arrivee, float proba) {
    Cellule *nouvelle = creerCellule(arrivee, proba);
    nouvelle->suiv = l->head;
    l->head = nouvelle;
}

// Crée la structure de listes d'adjacence de taille donnée
ListeAdj creerListeAdj(int taille) {
    ListeAdj g;
    g.taille = taille;
    g.tab = (Liste *)malloc(taille * sizeof(Liste));
    if (!g.tab) { perror("Erreur d'allocation mémoire pour tableau de listes"); exit(EXIT_FAILURE); }
    for (int i = 0; i < taille; i++) g.tab[i] = creerListe();
    return g;
}

// Affiche une liste de transitions sortantes
void afficherListe(const Liste *l) {
    Cellule *tmp = l->head;
    while (tmp) {
        printf("-> (%d, %.2f) ", tmp->sommet_arrivee, tmp->proba);
        tmp = tmp->suiv;
    }
    printf("\n");
}

// Affiche tout le graphe (toutes les listes)
void afficherListeAdj(const ListeAdj *g) {
    for (int i = 0; i < g->taille; i++) {
        printf("Sommet %d : ", i + 1);
        afficherListe(&g->tab[i]);
    }
}

// Lecture d'un graphe de Markov depuis fichier: nb_sommets puis triples (depart arrivee proba)
ListeAdj readMarkovGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) { perror("Impossible d'ouvrir le fichier"); exit(EXIT_FAILURE); }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) { perror("Erreur lecture nombre de sommets"); exit(EXIT_FAILURE); }

    ListeAdj g = creerListeAdj(nbvert);
    int depart, arrivee; float proba;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
        ajouterCellule(&g.tab[depart - 1], arrivee, proba);
    fclose(file);
    return g;
}

// Vérifie la contrainte Markov: somme des probabilités sortantes ~= 1 pour chaque sommet
int estGrapheMarkov(const ListeAdj *g) {
    int valide = 1;
    for (int i = 0; i < g->taille; i++) {
        float somme = 0.0f;
        Cellule *tmp = g->tab[i].head;
        while (tmp) { somme += tmp->proba; tmp = tmp->suiv; }
        printf("Sommet %d : somme = %.2f\n", i + 1, somme);
        if (somme < 0.99f || somme > 1.01f) valide = 0;
    }
    if (valide) printf("\nLe graphe est un graphe de Markov.\n");
    else printf("\nLe graphe n’est pas un graphe de Markov.\n");
    return valide;
}

// Génère un identifiant alphabétique (A, B, ..., Z, AA, AB, ...)
char *getIdMarkov(int num) {
    static char id[10];
    int i = 0; num--;
    while (num >= 0) {
        id[i++] = 'A' + (num % 26);
        num = num / 26 - 1;
    }
    id[i] = '\0';
    for (int j = 0; j < i / 2; j++) { char tmp = id[j]; id[j] = id[i - j - 1]; id[i - j - 1] = tmp; }
    return id;
}

// Exporte le graphe sous forme Mermaid (noeuds + arcs avec poids)
void genererMermaid(const ListeAdj *g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) { perror("Erreur ouverture fichier Mermaid"); exit(EXIT_FAILURE); }
    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\nflowchart LR\n");
    for (int i = 0; i < g->taille; i++) fprintf(f, "%s((%d))\n", getIdMarkov(i + 1), i + 1);
    fprintf(f, "\n");
    for (int i = 0; i < g->taille; i++) {
        Cellule *tmp = g->tab[i].head;
        while (tmp) {
            fprintf(f, "%s -->|%.2f|%s\n",
                    getIdMarkov(i + 1), tmp->proba, getIdMarkov(tmp->sommet_arrivee));
            tmp = tmp->suiv;
        }
    }
    fclose(f);
    printf("Fichier Mermaid généré : %s\n", filename);
}
