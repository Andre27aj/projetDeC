// Implémentation des fonctions de gestion du diagramme de Hasse - Partie 2
// Groupe Projet : André, Adam, Clément et Roman
#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"

// Création d'un ensemble de liens vide
t_ensemble_liens* creer_ensemble_liens() {
    t_ensemble_liens* ensemble = (t_ensemble_liens*)malloc(sizeof(t_ensemble_liens));
    if (!ensemble) { perror("malloc ensemble_liens"); exit(EXIT_FAILURE); }

    ensemble->capacite = 10;
    ensemble->nb_liens = 0;
    ensemble->liens = (t_lien*)malloc(ensemble->capacite * sizeof(t_lien));
    if (!ensemble->liens) { perror("malloc liens"); exit(EXIT_FAILURE); }

    return ensemble;
}

// Vérification de l'existence d'un lien
int lien_existe(t_ensemble_liens* ensemble, int depart, int arrivee) {
    if (!ensemble) return 0;

    for (int i = 0; i < ensemble->nb_liens; i++) {
        if (ensemble->liens[i].classe_depart == depart &&
            ensemble->liens[i].classe_arrivee == arrivee) {
            return 1;
        }
    }
    return 0;
}

// Ajout d'un lien
void ajouter_lien(t_ensemble_liens* ensemble, int depart, int arrivee) {
    if (!ensemble) return;

    // Vérifier si on doit agrandir
    if (ensemble->nb_liens >= ensemble->capacite) {
        ensemble->capacite *= 2;
        ensemble->liens = (t_lien*)realloc(ensemble->liens,
                                           ensemble->capacite * sizeof(t_lien));
        if (!ensemble->liens) { perror("realloc liens"); exit(EXIT_FAILURE); }
    }

    ensemble->liens[ensemble->nb_liens].classe_depart = depart;
    ensemble->liens[ensemble->nb_liens].classe_arrivee = arrivee;
    ensemble->nb_liens++;
}

// Recensement des liens entre classes selon l'algorithme du sujet
t_ensemble_liens* recenser_liens(AdjList* g, t_partition* partition) {
    t_ensemble_liens* liens = creer_ensemble_liens();

    // Créer un tableau qui indique pour chaque sommet la classe à laquelle il appartient
    int* sommet_vers_classe = (int*)malloc((g->size + 1) * sizeof(int));
    if (!sommet_vers_classe) { perror("malloc"); exit(EXIT_FAILURE); }

    // Remplir le tableau sommet_vers_classe
    for (int i = 0; i < partition->nb_classes; i++) {
        for (int j = 0; j < partition->classes[i].nb_sommets; j++) {
            int id_sommet = partition->classes[i].sommets[j].identifiant;
            sommet_vers_classe[id_sommet] = i;
        }
    }

    // Pour chaque sommet i du graphe
    for (int i = 1; i <= g->size; i++) {
        int Ci = sommet_vers_classe[i];  // Classe à laquelle appartient i

        // Pour tous les sommets j dans la liste d'adjacence du sommet i
        const Cell* courant = g->lists[i].head;
        while (courant) {
            int j = courant->dest;
            int Cj = sommet_vers_classe[j];  // Classe à laquelle appartient j

            // Si Ci est différent de Cj (arête entre classes)
            if (Ci != Cj) {
                // Si le lien (Ci, Cj) n'existe pas
                if (!lien_existe(liens, Ci, Cj)) {
                    // Ajouter le lien (Ci, Cj)
                    ajouter_lien(liens, Ci, Cj);
                }
            }
            courant = courant->next;
        }
    }

    free(sommet_vers_classe);
    return liens;
}

// Export du diagramme de Hasse au format Mermaid
void export_hasse_mermaid(t_partition* partition, t_ensemble_liens* liens, const char* filename) {
    FILE* f = fopen(filename, "wt");
    if (!f) { perror("fopen hasse"); exit(EXIT_FAILURE); }

    // En-tête Mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "   layout: elk\n");
    fprintf(f, "   theme: neo\n");
    fprintf(f, "   look: neo\n");
    fprintf(f, "---\n\n");
    fprintf(f, "flowchart TD\n");

    // Déclaration des classes (nœuds)
    for (int i = 0; i < partition->nb_classes; i++) {
        fprintf(f, "    %s[\"%s: {",
                partition->classes[i].nom,
                partition->classes[i].nom);

        // Afficher les sommets de la classe
        for (int j = 0; j < partition->classes[i].nb_sommets; j++) {
            fprintf(f, "%d", partition->classes[i].sommets[j].identifiant);
            if (j < partition->classes[i].nb_sommets - 1) {
                fprintf(f, ",");
            }
        }
        fprintf(f, "}\"]\n");
    }
    fprintf(f, "\n");

    // Déclaration des liens (arêtes)
    for (int i = 0; i < liens->nb_liens; i++) {
        fprintf(f, "    %s --> %s\n",
                partition->classes[liens->liens[i].classe_depart].nom,
                partition->classes[liens->liens[i].classe_arrivee].nom);
    }

    fclose(f);
    printf("Diagramme de Hasse exporte dans '%s'\n", filename);
}

// Libération mémoire
void liberer_ensemble_liens(t_ensemble_liens* ensemble) {
    if (ensemble) {
        free(ensemble->liens);
        free(ensemble);
    }
}