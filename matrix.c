// matrix.c
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

static float **alloc2D(int n, int m) {
    float **a = (float **)malloc(n * sizeof(float *));
    if (!a) { perror("malloc matrix rows"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; ++i) {
        a[i] = (float *)malloc(m * sizeof(float));
        if (!a[i]) { perror("malloc matrix cols"); exit(EXIT_FAILURE); }
    }
    return a;
}

static void free2D(float **a, int n) {
    if (!a) return;
    for (int i = 0; i < n; ++i) {
        free(a[i]);
    }
    free(a);
}

t_matrix createEmptyMatrix(int n) {
    t_matrix m;
    m.rows = n;
    m.cols = n;
    m.data = alloc2D(n, n);
    return m;
}

t_matrix createZeroMatrix(int n) {
    t_matrix m = createEmptyMatrix(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            m.data[i][j] = 0.0f;
    return m;
}

void freeMatrix(t_matrix *m) {
    if (!m || !m->data) return;
    free2D(m->data, m->rows);
    m->data = NULL;
    m->rows = m->cols = 0;
}

void copyMatrix(t_matrix *dest, const t_matrix *src) {
    if (!dest || !src) return;
    if (dest->rows != src->rows || dest->cols != src->cols) {
        fprintf(stderr, "copyMatrix: tailles incompatibles\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < src->rows; ++i)
        for (int j = 0; j < src->cols; ++j)
            dest->data[i][j] = src->data[i][j];
}

t_matrix adjListToMatrix(const AdjList *g) {
    if (!g || !g->lists) {
        fprintf(stderr, "adjListToMatrix: graphe invalide\n");
        exit(EXIT_FAILURE);
    }
    int n = g->size;
    t_matrix M = createZeroMatrix(n);

    // Sommets numérotés 1..n, lignes/colonnes 0..n-1
    for (int i = 1; i <= n; ++i) {
        const Cell *it = g->lists[i].head;
        while (it) {
            int from = i - 1;
            int to   = it->dest - 1;
            if (from < 0 || from >= n || to < 0 || to >= n) {
                fprintf(stderr, "adjListToMatrix: indice hors limite\n");
                exit(EXIT_FAILURE);
            }
            M.data[from][to] = it->prob;
            it = it->next;
        }
    }
    return M;
}

void printMatrix(const t_matrix *m) {
    if (!m || !m->data) return;
    for (int i = 0; i < m->rows; ++i) {
        for (int j = 0; j < m->cols; ++j) {
            printf("%6.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrices(const t_matrix *A, const t_matrix *B, t_matrix *C) {
    if (!A || !B || !C) return;
    if (A->rows != A->cols || B->rows != B->cols || C->rows != C->cols ||
        A->rows != B->rows || A->rows != C->rows) {
        fprintf(stderr, "multiplyMatrices: tailles incompatibles\n");
        exit(EXIT_FAILURE);
    }
    int n = A->rows;
    // Produit classique C = A * B
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < n; ++k) {
                sum += A->data[i][k] * B->data[k][j];
            }
            C->data[i][j] = sum;
        }
    }
}

float diffMatrices(const t_matrix *M, const t_matrix *N) {
    if (!M || !N) return 0.0f;
    if (M->rows != N->rows || M->cols != N->cols) {
        fprintf(stderr, "diffMatrices: tailles incompatibles\n");
        exit(EXIT_FAILURE);
    }
    float diff = 0.0f;
    for (int i = 0; i < M->rows; ++i) {
        for (int j = 0; j < M->cols; ++j) {
            diff += fabsf(M->data[i][j] - N->data[i][j]);
        }
    }
    return diff;
}

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    if (compo_index < 0 || compo_index >= part.nb_classes) {
        fprintf(stderr, "subMatrix: indice de composante invalide\n");
        exit(EXIT_FAILURE);
    }

    t_classe *classe = &part.classes[compo_index];
    int k = classe->nb_sommets;

    t_matrix sub = createZeroMatrix(k);

    // On suppose que `identifiant` est dans 1..n et correspond aux
    // indices de lignes/colonnes de la matrice globale.
    for (int i = 0; i < k; ++i) {
        int id_i = classe->sommets[i].identifiant; // 1..n
        int row_glob = id_i - 1;
        for (int j = 0; j < k; ++j) {
            int id_j = classe->sommets[j].identifiant; // 1..n
            int col_glob = id_j - 1;
            sub.data[i][j] = matrix.data[row_glob][col_glob];
        }
    }

    return sub;
}
