// Fichier .h pour la gestion des matrices - Partie 3
// Groupe Projet : André, Adam, Clément et Roman
#ifndef MATRIX_H
#define MATRIX_H

#include "fonctions.h"
#include "structs.h"

typedef struct {
    int rows;
    int cols;
    float **data;
} t_matrix;

// Création / destruction
t_matrix createEmptyMatrix(int n);
t_matrix createZeroMatrix(int n);
void freeMatrix(t_matrix *m);

// Copie
void copyMatrix(t_matrix *dest, const t_matrix *src);

// Construction à partir du graphe
t_matrix adjListToMatrix(const AdjList *g);

// Affichage (debug)
void printMatrix(const t_matrix *m);

// Multiplication C = A * B (toutes 3 de taille n x n)
void multiplyMatrices(const t_matrix *A, const t_matrix *B, t_matrix *C);

// diff(M,N) = somme\_ij |m\_ij - n\_ij|
float diffMatrices(const t_matrix *M, const t_matrix *N);

/**
 * @brief Extrait la sous-matrice correspondant à une composante
 *        (classe) de la partition.
 * @param matrix Matrice d'adjacence globale (n x n).
 * @param part   Partition Tarjan.
 * @param compo_index Indice de la classe à extraire (0..nb\_classes-1).
 * @return t_matrix nouvelle matrice k x k (k = taille de la classe).
 */
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);

#endif
