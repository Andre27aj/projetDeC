#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "fonctions.h"
#include "tarjan.h"
#include "hasse.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void setup_console_utf8(void) {
    setlocale(LC_ALL, ".UTF-8");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

static const char* ask_input_file(void) {
    int choice = 0;
    printf("Selectionnez le fichier d'entree:\n");
    printf("  1) exemple1.txt\n");
    printf("  2) exemple_invalide.txt\n");
    printf("  3) exemple2.txt\n");
    printf("  4) exemple_invalide2.txt\n");
    printf("  5) exemple_valid_step3.txt\n");
    printf("Votre choix: ");
    fflush(stdout);
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Entree invalide, defaut: exemple1.txt\n");
        return "exemple1.txt";
    }
    switch (choice) {
        case 2: return "exemple_invalide.txt";
        case 3: return "exemple2.txt";
        case 4: return "exemple_invalide2.txt";
        case 5: return "exemple_valid_step3.txt";
        default: return "exemple1.txt";
    }
}

int main(int argc, char** argv) {
    setup_console_utf8();

    int force_menu = (argc >= 2 && strcmp(argv[1], "--menu") == 0);
    const char* in  = (!force_menu && argc >= 2) ? argv[1] : ask_input_file();
    const char* out = (argc >= 3) ? argv[2] : "partie_1.txt";

    // ========== PARTIE 1 ==========
    printf("\nPARTIE 1\n\n");

    // Lecture du graphe
    AdjList g = readGraph(in);
    printf("Graphe charge depuis '%s'\n\n", in);

    // Affichage de la liste d'adjacence
    printf("Liste d'adjacence:\n");
    print_adj(&g);
    printf("\n");

    // Vérification que c'est un graphe de Markov
    printf("Verification du graphe de Markov:\n");
    int ok = verify_markov(&g);
    printf("\n");

    if (!ok) {
        printf("Le graphe n'est pas valide. Arret du programme.\n");
        adj_free(&g);
        return 1;
    }

    // Export Mermaid pour la partie 1
    export_mermaid(&g, out);
    printf("\n");

    // ========== PARTIE 2 ==========
    printf("\nPARTIE 2\n\n");

    // Étape 1 : Algorithme de Tarjan
    printf("    Etape 1: Algorithme de Tarjan    \n\n");
    t_partition* partition = tarjan(&g);

    printf("Partition en composantes fortement connexes:\n");
    afficher_partition(partition);
    printf("\n");

    // Étape 2 : Recensement des liens et diagramme de Hasse
    printf("    Etape 2: Diagramme de Hasse    \n\n");
    t_ensemble_liens* liens = recenser_liens(&g, partition);

    printf("Liens entre classes:\n");
    if (liens->nb_liens == 0) {
        printf("  Aucun lien entre classes\n");
    } else {
        for (int i = 0; i < liens->nb_liens; i++) {
            printf("  %s --> %s\n",
                   partition->classes[liens->liens[i].classe_depart].nom,
                   partition->classes[liens->liens[i].classe_arrivee].nom);
        }
    }
    printf("\n");

    // Export du diagramme de Hasse
    const char* hasse_file = "hasse.txt";
    export_hasse_mermaid(partition, liens, hasse_file);
    printf("\n");

    // Étape 3 : Analyse des caractéristiques
    printf("    Etape 3: Caracteristiques du graphe    \n");
    analyser_caracteristiques(partition, liens);
    printf("\n");

    // Libération mémoire
    liberer_ensemble_liens(liens);
    liberer_partition(partition);
    adj_free(&g);

    printf("Fin du programme.\n");
    return 0;
}