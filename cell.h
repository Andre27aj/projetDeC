// Fichier .h pour les cellules - Partie 1
// Groupe Projet : André, Adam, Clément et Roman

#ifndef CELL_H
#define CELL_H
#pragma once

typedef struct Cell {
    int dest;
    float prob;
    struct Cell* next;
} Cell;

typedef struct {
    Cell* head;
} List;

void list_init(List* l);
void list_add_front(List* l, int dest, float prob);
void list_free(List* l);


#endif //CELL_H
