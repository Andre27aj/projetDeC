// Fichier des fonctions

#ifndef FONCTIONS_H
#define FONCTIONS_H

#pragma once
#include <stdbool.h>
#include "cell.h"

typedef struct {
    int size;     // sommets 1..size
    List* lists;  // tableau [0..size], index 0 inutilisé
} AdjList;

AdjList adj_create(int n);
void adj_free(AdjList* g);
void add_edge(AdjList* g, int from, int to, float prob);

void print_list(const List* l);
void print_adj(const AdjList* g);

AdjList readGraph(const char* filename);
bool verify_markov(const AdjList* g);

char* getId(int num); // à free() par l'appelant
void export_mermaid(const AdjList* g, const char* outFile);

#endif //FONCTIONS_H