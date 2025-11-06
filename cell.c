
#include <stdlib.h>
#include <stdio.h>
#include "cell.h"

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

