// Fichier source pour la gestion des graphes de Markov - Partie 2
// Groupe Projet : André, Adam, Clément et Roman
#include <stdlib.h>
#include <stdio.h>
#include "markov.h"

Cellule *creerCellule(int arrivee, float proba) {
    Cellule *c = (Cellule *)malloc(sizeof(Cellule));
    if (!c) {
        perror("Erreur d'allocation mémoire pour cellule");
        exit(EXIT_FAILURE);
    }
    c->sommet_arrivee = arrivee;
    c->proba = proba;
    c->suiv = NULL;
    return c;
}

Liste creerListe(void) {
    Liste l;
    l.head = NULL;
    return l;
}

void ajouterCellule(Liste *l, int arrivee, float proba) {
    Cellule *nouvelle = creerCellule(arrivee, proba);
    nouvelle->suiv = l->head;
    l->head = nouvelle;
}

ListeAdj creerListeAdj(int taille) {
    ListeAdj g;
    g.taille = taille;
    g.tab = (Liste *)malloc(taille * sizeof(Liste));
    if (!g.tab) {
        perror("Erreur d'allocation mémoire pour tableau de listes");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < taille; i++)
        g.tab[i] = creerListe();
    return g;
}

void afficherListe(const Liste *l) {
    Cellule *tmp = l->head;
    while (tmp) {
        printf("-> (%d, %.2f) ", tmp->sommet_arrivee, tmp->proba);
        tmp = tmp->suiv;
    }
    printf("\n");
}

void afficherListeAdj(const ListeAdj *g) {
    for (int i = 0; i < g->taille; i++) {
        printf("Sommet %d : ", i + 1);
        afficherListe(&g->tab[i]);
    }
}

ListeAdj readMarkovGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Erreur lecture nombre de sommets");
        exit(EXIT_FAILURE);
    }

    ListeAdj g = creerListeAdj(nbvert);
    int depart, arrivee;
    float proba;

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        ajouterCellule(&g.tab[depart - 1], arrivee, proba);
    }

    fclose(file);
    return g;
}

int estGrapheMarkov(const ListeAdj *g) {
    int valide = 1;
    for (int i = 0; i < g->taille; i++) {
        float somme = 0.0;
        Cellule *tmp = g->tab[i].head;
        while (tmp) {
            somme += tmp->proba;
            tmp = tmp->suiv;
        }
        if (somme < 0.99 || somme > 1.01) {
            printf("Sommet %d : somme = %.2f\n", i + 1, somme);
            valide = 0;
        } else {
            printf("Sommet %d : somme = %.2f\n", i + 1, somme);
        }
    }

    if (valide)
        printf("\nLe graphe est un graphe de Markov.\n");
    else
        printf("\nLe graphe n’est pas un graphe de Markov.\n");

    return valide;
}

char *getIdMarkov(int num) {
    static char id[10];
    int i = 0;
    num--;
    while (num >= 0) {
        id[i++] = 'A' + (num % 26);
        num = num / 26 - 1;
    }
    id[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char tmp = id[j];
        id[j] = id[i - j - 1];
        id[i - j - 1] = tmp;
    }
    return id;
}

void genererMermaid(const ListeAdj *g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Erreur ouverture fichier Mermaid");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\n");
    fprintf(f, "flowchart LR\n");

    for (int i = 0; i < g->taille; i++) {
        fprintf(f, "%s((%d))\n", getIdMarkov(i + 1), i + 1);
    }
    fprintf(f, "\n");

    for (int i = 0; i < g->taille; i++) {
        Cellule *tmp = g->tab[i].head;
        while (tmp) {
            fprintf(f, "%s -->|%.2f|%s\n", getIdMarkov(i + 1), tmp->proba, getIdMarkov(tmp->sommet_arrivee));
            tmp = tmp->suiv;
        }
    }

    fclose(f);
    printf("Fichier Mermaid généré : %s\n", filename);
}
