//
// Created by roros on 23/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fonctions.h"

AdjList adj_create(int n) {
    AdjList g;
    g.size = (n > 0) ? n : 0;
    g.lists = (List*)malloc((g.size + 1) * sizeof(List));
    if (!g.lists) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i <= g.size; ++i) list_init(&g.lists[i]);
    return g;
}

void adj_free(AdjList* g) {
    if (!g || !g->lists) return;
    for (int i = 1; i <= g->size; ++i) list_free(&g->lists[i]);
    free(g->lists);
    g->lists = NULL;
    g->size = 0;
}

void add_edge(AdjList* g, int from, int to, float prob) {
    if (!g || !g->lists) return;
    if (from < 1 || from > g->size || to < 1 || to > g->size) {
        fprintf(stderr, "Arête ignorée: sommet hors borne (%d -> %d)\n", from, to);
        return;
    }
    if (prob <= 0.0f || prob > 1.0f) {
        fprintf(stderr, "Avertissement: probabilité invalide %.6f sur (%d -> %d)\n", prob, from, to);
    }
    list_add_front(&g->lists[from], to, prob);
}

void print_list(const List* l) {
    const Cell* it = l ? l->head : NULL;
    printf("[head @]");
    while (it) {
        printf(" -> (%d, %.2f) @", it->dest, it->prob);
        it = it->next;
    }
    printf("\n");
}

void print_adj(const AdjList* g) {
    if (!g || !g->lists) return;
    for (int i = 1; i <= g->size; ++i) {
        printf("Liste pour le sommet %d:", i);
        print_list(&g->lists[i]);
    }
}

AdjList readGraph(const char* filename) {
    FILE* file = fopen(filename, "rt");
    if (!file) { perror("Could not open file for reading"); exit(EXIT_FAILURE); }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1 || nbvert <= 0) {
        fprintf(stderr, "Could not read number of vertices or invalid value\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    AdjList g = adj_create(nbvert);

    int depart, arrivee;
    float proba;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        add_edge(&g, depart, arrivee, proba);
    }

    fclose(file);
    return g;
}

bool verify_markov(const AdjList* g) {
    if (!g || !g->lists) return false;
    bool ok = true;
    for (int i = 1; i <= g->size; ++i) {
        double sum = 0.0;
        const Cell* it = g->lists[i].head;
        while (it) { sum += it->prob; it = it->next; }
        if (!(sum >= 0.99 - 1e-6 && sum <= 1.0 + 1e-6)) {
            printf("la somme des probabilités du sommet %d est %.2f\n", i, (float)sum);
            ok = false;
        }
    }
    if (ok) {
        printf("Le graphe est un graphe de Markov\n");
    } else {
        printf("Le graphe n’est pas un graphe de Markov\n");
    }
    return ok;
}

// 1->A, 2->B, ..., 26->Z, 27->AA, etc.
char* getId(int num) {
    if (num <= 0) num = 1;
    int n = num;
    int len = 0;
    while (n > 0) { len++; n = (n - 1) / 26; }
    char* s = (char*)malloc((len + 1) * sizeof(char));
    if (!s) { perror("malloc"); exit(EXIT_FAILURE); }
    s[len] = '\0';
    n = num;
    for (int i = len - 1; i >= 0; --i) {
        int rem = (n - 1) % 26;
        s[i] = (char)('A' + rem);
        n = (n - 1) / 26;
    }
    return s;
}

void export_mermaid(const AdjList* g, const char* outFile) {
    if (!g || !g->lists) return;
    FILE* f = fopen(outFile, "wt");
    if (!f) { perror("Could not open output file"); exit(EXIT_FAILURE); }

    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "   layout: elk\n");
    fprintf(f, "   theme: neo\n");
    fprintf(f, "   look: neo\n");
    fprintf(f, "---\n\n");
    fprintf(f, "flowchart LR\n");

    for (int i = 1; i <= g->size; ++i) {
        char* id = getId(i);
        fprintf(f, "%s((%d))\n", id, i);
        free(id);
    }
    fprintf(f, "\n");

    for (int i = 1; i <= g->size; ++i) {
        char* fromId = getId(i);
        const Cell* it = g->lists[i].head;
        while (it) {
            char* toId = getId(it->dest);
            fprintf(f, "%s   -->|%.2f|%s\n", fromId, it->prob, toId);
            free(toId);
            it = it->next;
        }
        free(fromId);
    }

    fclose(f);
    printf("Export Mermaid écrit dans `%s`\n", outFile);
}

