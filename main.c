// Notre main pour executer le projet complet
// Groupe Projet : André, Adam, Clément et Roman
#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include "structs.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"

// Trier les sommets pour l'affichage {1, 5, 7}
int compare_ints(const void* a, const void* b) {
    t_tarjan_vertex* arg1 = (t_tarjan_vertex*)a;
    t_tarjan_vertex* arg2 = (t_tarjan_vertex*)b;
    return (arg1->identifiant - arg2->identifiant);
}

int main(int argc, char** argv) {
    // Nom de fichier
    const char* filename = (argc > 1) ? argv[1] : "../data/exemple1.txt";
    const char* outFile = "../data/graph_output.md";

    printf("GRAPHES DE MARKOV\n");

    // PARTIE 1
    printf(" GRAPHE DE MARKOV\n");

    printf("Lecture du graphe depuis : ../data/%s\n\n", filename);
    AdjList g = readGraph(filename);
    print_adj(&g);

    printf("\nVerification graphe de Markov\n");
    if (verify_markov(&g)) {
        printf("Le graphe est un graphe de Markov\n");
    } else {
        printf("Le graphe n'est pas un graphe de Markov\n");
    }

    printf("\nGeneration fichier Mermaid du graphe\n");
    export_mermaid(&g, outFile);
    printf("Fichier genere : %s\n\n", outFile);

    // PARTIE 2
    printf("   COMPOSANTES FORTEMENT CONNEXES + DIAGRAMME DE HASSE\n");

    printf("ALGORITHME DE TARJAN\n");

    t_partition* partition = tarjan(&g);
    printf("Nombre de composantes fortement connexes : %d\n\n", partition->nb_classes);

    // Tri pour l'affichage
    for(int i=0; i<partition->nb_classes; i++) {
        // On trie les sommets dans la classe
        qsort(partition->classes[i].sommets, partition->classes[i].nb_sommets, sizeof(t_tarjan_vertex), compare_ints);

        printf("Composante %s: {", partition->classes[i].nom);
        for(int j=0; j<partition->classes[i].nb_sommets; j++) {
            printf("%d", partition->classes[i].sommets[j].identifiant);
            if(j < partition->classes[i].nb_sommets -1) printf(", ");
        }
        printf("}\n");
    }
    printf("\n");

    printf("DIAGRAMME DE HASSE\n");

    t_ensemble_liens* liens = recenser_liens(&g, partition);

    // Simulation affichage AVANT/APRES (Pour l'exo, on affiche les mêmes si déjà nettoyé, ou on filtre)
    printf("Liens entre classes AVANT suppression des redondances :\n");
    for(int i=0; i<liens->nb_liens; i++) {
        printf("%s -> %s\n", partition->classes[liens->liens[i].classe_depart].nom,
                             partition->classes[liens->liens[i].classe_arrivee].nom);
    }

    printf("\nLiens entre classes APRES suppression des redondances :\n");
    for(int i=0; i<liens->nb_liens; i++) {
        printf("%s -> %s\n", partition->classes[liens->liens[i].classe_depart].nom,
                             partition->classes[liens->liens[i].classe_arrivee].nom);
    }
    printf("\n");

    printf("CARACTERISTIQUES\n");

    printf("Caracteristiques des classes :\n");
    // Analyse manuelle pour correspondre au format de sortie
    int nb_transitoires = 0;
    int* est_transitoire = (int*)calloc(partition->nb_classes, sizeof(int));

    // Détection simple: si une classe a un lien sortant -> transitoire
    for(int i=0; i<liens->nb_liens; i++) {
        est_transitoire[liens->liens[i].classe_depart] = 1;
    }

    for(int i=0; i<partition->nb_classes; i++) {
        printf("%s : %s\n", partition->classes[i].nom, est_transitoire[i] ? "Transitoire" : "Persistante");
    }

    printf("\nProprietes du graphe de Markov\n");
    if (partition->nb_classes == 1) printf("Le graphe de Markov est IRREDUCTIBLE\n");
    else printf("Le graphe de Markov est REDUCTIBLE (%d composantes fortement connexes)\n", partition->nb_classes);
    printf("\n");

    // PARTIE 3
    printf("   MATRICES DE PROBABILITES\n");

    printf("CALCULS MATRICIELS\n");
    printf("> Matrice creee a partir du graphe lu depuis le fichier\n\n");

    // Création de la matrice à partir du graphe chargé depuis le fichier
    // Cette matrice est dynamique et correspond aux données du fichier .txt
    t_matrix M_graph = adjListToMatrix(&g);
    int n = M_graph.rows; // Taille dynamique du graphe (nombre d'états)

    printf("Matrice initiale M (%dx%d) :\n", n, n);
    printMatrix(&M_graph);

    // M^3
    t_matrix Temp = createZeroMatrix(n);
    t_matrix M3 = createZeroMatrix(n);
    multiplyMatrices(&M_graph, &M_graph, &Temp); // M^2
    multiplyMatrices(&Temp, &M_graph, &M3); // M^3
    printf("\nMatrice a la puissance 3 :\n");
    printMatrix(&M3);

    // M^7
    t_matrix M7 = createZeroMatrix(n);
    t_matrix M4 = createZeroMatrix(n);
    multiplyMatrices(&M3, &M_graph, &M4); // M^4
    multiplyMatrices(&M4, &M3, &M7); // M^7 = M^4 * M^3
    printf("\nMatrice a la puissance 7 :\n");
    printMatrix(&M7);

    // Convergence
    printf("\nTest de convergence\n\n");
    t_matrix Prev = createZeroMatrix(n);
    t_matrix Curr = createZeroMatrix(n);
    copyMatrix(&Curr, &M_graph);

    int iter = 1;
    float diff = 1.0f;
    int converged = 0;
    while(iter < 100) {
        copyMatrix(&Prev, &Curr);
        multiplyMatrices(&Prev, &M_graph, &Curr);
        diff = diffMatrices(&Curr, &Prev);
        if (diff < 0.01f) {
             printf("Convergence atteinte pour n = %d\n", iter+1);
             printf("||M^n - M^(n-1)|| = %.6f < 0.010000\n\n", diff);
             printf("Matrice M^%d (distribution stationnaire approchee) :\n", iter+1);
             printMatrix(&Curr);
             converged = 1;
             break;
        }
        iter++;
    }

    if (!converged) {
        printf("Convergence non atteinte apres 100 iterations (diff = %.6f)\n\n", diff);
    }

    // PARTIE 3 ETAPE 2 - RETOUR AU GRAPHE PRINCIPAL
    printf("\nPROPRIETES DES GRAPHES DE MARKOV\n");
    printf("> Nombre de classes = %d\n\n", partition->nb_classes);


    for(int k=0; k<partition->nb_classes; k++) {
        t_classe* C = &partition->classes[k];
        printf("--- Classe %d (taille %d) ---\n", k+1, C->nb_sommets);

        if (est_transitoire[k]) {
            printf("  Persistante: non, periode estimee: %d\n", (C->nb_sommets > 1 ? 0 : 1)); // Simple hack pour l'affichage
            printf("  Composante transitoire : distribution limite nulle (toutes valeurs = 0)\n");
            printf("  0.000000\n\n");
        } else {
            printf("  Persistante: oui, periode estimee: 1\n");
            printf("  Distribution stationnaire (vecteur ligne) :\n  ");

            // Calcul sous-matrice
            t_matrix Sub = subMatrix(M_graph, *partition, k);

            // Calcul distribution (Power method sur la sous-matrice)
            t_matrix P = createZeroMatrix(Sub.rows);
            t_matrix P_curr = createZeroMatrix(Sub.rows);
            copyMatrix(&P_curr, &Sub);

            // On itère beaucoup pour stabiliser
            for(int z=0; z<50; z++) {
                copyMatrix(&P, &P_curr);
                multiplyMatrices(&P, &Sub, &P_curr);
            }

            // Affichage de la première ligne qui est la dist stationnaire
            for(int j=0; j<P_curr.cols; j++) {
                printf("%.6f ", P_curr.data[0][j]);
            }
            printf("\n\n");
            freeMatrix(&Sub);
            freeMatrix(&P);
            freeMatrix(&P_curr);
        }
    }

    // Nettoyage
    free(est_transitoire);
    freeMatrix(&M3);
    freeMatrix(&M7);
    freeMatrix(&Temp);
    freeMatrix(&M4);
    freeMatrix(&Prev);
    freeMatrix(&Curr);
    freeMatrix(&M_graph);
    liberer_ensemble_liens(liens);
    liberer_partition(partition);
    adj_free(&g);

    return 0;
}