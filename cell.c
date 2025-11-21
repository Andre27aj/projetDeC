// Fichier regroupant les fonctions utiles pour les cellules - Partie 1
// Groupe Projet : André, Adam, Clément et Roman

#include <stdlib.h>
#include "cell.h"

// Fonction de création d'une cellule
static Cell* create_cell(int dest, float prob) {
    Cell* c = (Cell*)malloc(sizeof(Cell));
    if (!c) { perror("malloc"); exit(EXIT_FAILURE); }
    c->dest = dest;
    c->prob = prob;
    c->next = NULL;
    return c;
}

void list_init(List* l) {
    if (l) l->head = NULL;
}

void list_add_front(List* l, int dest, float prob) {
    if (!l) return;
    Cell* c = create_cell(dest, prob);
    c->next = l->head;
    l->head = c;
}

void list_free(List* l) {
    if (!l) return;
    Cell* it = l->head;
    while (it) {
        Cell* nxt = it->next;
        free(it);
        it = nxt;
    }
    l->head = NULL;
}

