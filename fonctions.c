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
    if (from < 1 || from > g->size || to < 1 || to > g->size) return;
    list_add_front(&g->lists[from], to, prob);
}

void print_list(const List* l) {
    const Cell* it = l ? l->head : NULL;
    printf("[@head]");
    while (it) {
        // Format demandé : @-> (7, 0.7500)
        printf(" @-> (%d, %.4f) ", it->dest, it->prob);
        it = it->next;
    }
    printf("\n");
}

void print_adj(const AdjList* g) {
    if (!g || !g->lists) return;
    for (int i = 1; i <= g->size; ++i) {
        printf("Liste pour le sommet %d : ", i);
        print_list(&g->lists[i]);
    }
}

AdjList readGraph(const char* filename) {
    FILE* file = fopen(filename, "rt");
    if (!file) { 
        // Si échec, on essaie de remonter d'un dossier (cas ../data/)
        char path[100];
        sprintf(path, "../data/%s", filename);
        file = fopen(path, "rt");
        if (!file) {
             perror("Erreur lecture fichier"); exit(EXIT_FAILURE); 
        }
    }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) exit(EXIT_FAILURE);

    AdjList g = adj_create(nbvert);
    int u, v;
    float p;
    while (fscanf(file, "%d %d %f", &u, &v, &p) == 3) {
        add_edge(&g, u, v, p);
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
        if (sum < 0.99 || sum > 1.01) ok = false;
    }
    return ok;
}

char* getId(int num) {
    char* s = (char*)malloc(10);
    sprintf(s, "%d", num);
    return s;
}

void export_mermaid(const AdjList* g, const char* outFile) {
    FILE* f = fopen(outFile, "wt");
    if (!f) return;
    fprintf(f, "graph LR\n");
    for (int i = 1; i <= g->size; ++i) {
        const Cell* it = g->lists[i].head;
        while (it) {
            fprintf(f, "    %d -->|%.2f| %d\n", i, it->prob, it->dest);
            it = it->next;
        }
    }
    fclose(f);
}