#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

// ... (Garde tes fonctions alloc2D, free2D, createEmptyMatrix, createZeroMatrix, freeMatrix, copyMatrix comme avant) ...
// Je remets les fonctions essentielles ici pour être sûr qu'elles matchent

static float **alloc2D(int n, int m) {
    float **a = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; ++i) a[i] = (float *)calloc(m, sizeof(float));
    return a;
}
// ... (Tes fonctions free2D, etc.) ...

t_matrix createZeroMatrix(int n) {
    t_matrix m;
    m.rows = n; m.cols = n;
    m.data = alloc2D(n, n);
    return m;
}

void freeMatrix(t_matrix *m) {
    if (m->data) {
        for(int i=0; i<m->rows; i++) free(m->data[i]);
        free(m->data);
    }
    m->data = NULL;
}

void copyMatrix(t_matrix *dest, const t_matrix *src) {
    for(int i=0; i<src->rows; i++)
        for(int j=0; j<src->cols; j++)
            dest->data[i][j] = src->data[i][j];
}

t_matrix adjListToMatrix(const AdjList *g) {
    t_matrix M = createZeroMatrix(g->size);
    for (int i = 1; i <= g->size; ++i) {
        const Cell *it = g->lists[i].head;
        while (it) {
            M.data[i-1][it->dest-1] = it->prob; // -1 car indices 0-based dans matrice
            it = it->next;
        }
    }
    return M;
}

void printMatrix(const t_matrix *m) {
    for (int i = 0; i < m->rows; ++i) {
        for (int j = 0; j < m->cols; ++j) {
            printf("  %.4f ", m->data[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrices(const t_matrix *A, const t_matrix *B, t_matrix *C) {
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < B->cols; ++j) {
            C->data[i][j] = 0;
            for (int k = 0; k < A->cols; ++k) {
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }
}

float diffMatrices(const t_matrix *M, const t_matrix *N) {
    float diff = 0.0f;
    for (int i = 0; i < M->rows; ++i)
        for (int j = 0; j < M->cols; ++j)
            diff += fabsf(M->data[i][j] - N->data[i][j]);
    return diff;
}

// Fonction pour extraire la sous-matrice d'une classe
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    t_classe *C = &part.classes[compo_index];
    int k = C->nb_sommets;
    t_matrix sub = createZeroMatrix(k);
    
    for(int i=0; i<k; i++) {
        int u = C->sommets[i].identifiant - 1; // indice global
        for(int j=0; j<k; j++) {
            int v = C->sommets[j].identifiant - 1; // indice global
            sub.data[i][j] = matrix.data[u][v];
        }
    }
    return sub;
}