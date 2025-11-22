// Fichier regroupant les fonctions utiles pour les cellules - Partie 1
// Groupe Projet : André, Adam, Clément et Roman

#include <stdlib.h>
#include <stdio.h>
#include "cell.h"

// Implémentation minimale d'une liste chaînée pour transitions (dest, prob).

// create_cell : fabrique une cellule (transition) avec destination et probabilité.
static Cell* create_cell(int dest, float prob) {
    Cell* c = (Cell*)malloc(sizeof(Cell));
    if (!c) { perror("malloc"); exit(EXIT_FAILURE); }
    c->dest = dest;
    c->prob = prob;
    c->next = NULL;
    return c;
}

// list_init : met la liste à l'état vide.
void list_init(List* l) {
    if (l) l->head = NULL;
}

// list_add_front : insertion en tête (O(1)), pas de déduplication.
void list_add_front(List* l, int dest, float prob) {
    if (!l) return;
    Cell* c = create_cell(dest, prob);
    c->next = l->head;
    l->head = c;
}

// list_free : libère chaque cellule et réinitialise la tête.
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

