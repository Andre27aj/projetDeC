#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"
#include "structs.h"

// Variables globales pour l'algorithme de Tarjan
static int numero_actuel = 0;

// Déclaration anticipée de la fonction parcours
static void parcours(int sommet, AdjList* g, t_tarjan_vertex* vertices,
                     t_pile* pile, t_partition* partition);

// ========== FONCTIONS DE PILE ==========
t_pile* creer_pile(int capacite) {
    t_pile* pile = (t_pile*)malloc(sizeof(t_pile));
    if (!pile) { perror("malloc pile"); exit(EXIT_FAILURE); }

    pile->capacite = capacite;
    pile->top = -1;
    pile->data = (int*)malloc(capacite * sizeof(int));
    if (!pile->data) { perror("malloc pile->data"); exit(EXIT_FAILURE); }

    return pile;
}

int pile_est_vide(t_pile* pile) {
    return (pile->top == -1);
}

void empiler(t_pile* pile, int valeur) {
    if (pile->top >= pile->capacite - 1) {
        fprintf(stderr, "Erreur: pile pleine\n");
        exit(EXIT_FAILURE);
    }
    pile->data[++pile->top] = valeur;
}

int depiler(t_pile* pile) {
    if (pile_est_vide(pile)) {
        fprintf(stderr, "Erreur: pile vide\n");
        exit(EXIT_FAILURE);
    }
    return pile->data[pile->top--];
}

void liberer_pile(t_pile* pile) {
    if (pile) {
        free(pile->data);
        free(pile);
    }
}

// ========== FONCTIONS POUR CLASSES ==========
t_classe creer_classe_vide(const char* nom) {
    t_classe classe;
    strncpy(classe.nom, nom, sizeof(classe.nom) - 1);
    classe.nom[sizeof(classe.nom) - 1] = '\0';

    classe.capacite = 10;
    classe.nb_sommets = 0;
    classe.sommets = (t_tarjan_vertex*)malloc(classe.capacite * sizeof(t_tarjan_vertex));
    if (!classe.sommets) { perror("malloc classe.sommets"); exit(EXIT_FAILURE); }

    return classe;
}

void ajouter_sommet_classe(t_classe* classe, t_tarjan_vertex sommet) {
    if (classe->nb_sommets >= classe->capacite) {
        classe->capacite *= 2;
        classe->sommets = (t_tarjan_vertex*)realloc(classe->sommets,
                                                     classe->capacite * sizeof(t_tarjan_vertex));
        if (!classe->sommets) { perror("realloc classe.sommets"); exit(EXIT_FAILURE); }
    }
    classe->sommets[classe->nb_sommets++] = sommet;
}

void afficher_classe(t_classe classe) {
    printf("Composante %s: {", classe.nom);
    for (int i = 0; i < classe.nb_sommets; i++) {
        printf("%d", classe.sommets[i].identifiant);
        if (i < classe.nb_sommets - 1) printf(",");
    }
    printf("}\n");
}

void liberer_classe(t_classe* classe) {
    if (classe) {
        free(classe->sommets);
        classe->sommets = NULL;
    }
}

// ========== FONCTIONS POUR PARTITION ==========
t_partition* creer_partition_vide() {
    t_partition* partition = (t_partition*)malloc(sizeof(t_partition));
    if (!partition) { perror("malloc partition"); exit(EXIT_FAILURE); }

    partition->capacite = 10;
    partition->nb_classes = 0;
    partition->classes = (t_classe*)malloc(partition->capacite * sizeof(t_classe));
    if (!partition->classes) { perror("malloc partition.classes"); exit(EXIT_FAILURE); }

    return partition;
}

void ajouter_classe_partition(t_partition* partition, t_classe classe) {
    if (partition->nb_classes >= partition->capacite) {
        partition->capacite *= 2;
        partition->classes = (t_classe*)realloc(partition->classes,
                                                 partition->capacite * sizeof(t_classe));
        if (!partition->classes) { perror("realloc partition.classes"); exit(EXIT_FAILURE); }
    }
    partition->classes[partition->nb_classes++] = classe;
}

void afficher_partition(t_partition* partition) {
    for (int i = 0; i < partition->nb_classes; i++) {
        afficher_classe(partition->classes[i]);
    }
}

void liberer_partition(t_partition* partition) {
    if (partition) {
        for (int i = 0; i < partition->nb_classes; i++) {
            liberer_classe(&partition->classes[i]);
        }
        free(partition->classes);
        free(partition);
    }
}

// ========== ALGORITHME DE TARJAN ==========

// Fonction parcours (récursive)
static void parcours(int sommet, AdjList* g, t_tarjan_vertex* vertices,
                     t_pile* pile, t_partition* partition) {
    // Initialiser le sommet
    vertices[sommet].numero = numero_actuel;
    vertices[sommet].numero_accessible = numero_actuel;
    numero_actuel++;
    empiler(pile, sommet);
    vertices[sommet].dans_pile = 1;

    // Parcourir les successeurs
    const Cell* courant = g->lists[sommet].head;
    while (courant) {
        int successeur = courant->dest;

        if (vertices[successeur].numero == -1) {
            // Successeur non visité
            parcours(successeur, g, vertices, pile, partition);
            if (vertices[successeur].numero_accessible < vertices[sommet].numero_accessible) {
                vertices[sommet].numero_accessible = vertices[successeur].numero_accessible;
            }
        } else if (vertices[successeur].dans_pile) {
            // Successeur dans la pile
            if (vertices[successeur].numero < vertices[sommet].numero_accessible) {
                vertices[sommet].numero_accessible = vertices[successeur].numero;
            }
        }
        courant = courant->next;
    }

    // Si c'est la racine d'une composante fortement connexe
    if (vertices[sommet].numero_accessible == vertices[sommet].numero) {
        char nom_classe[10];
        snprintf(nom_classe, sizeof(nom_classe), "C%d", partition->nb_classes + 1);
        t_classe nouvelle_classe = creer_classe_vide(nom_classe);

        int sommet_depile;
        do {
            sommet_depile = depiler(pile);
            vertices[sommet_depile].dans_pile = 0;
            ajouter_sommet_classe(&nouvelle_classe, vertices[sommet_depile]);
        } while (sommet_depile != sommet);

        ajouter_classe_partition(partition, nouvelle_classe);
    }
}

// Fonction principale de Tarjan
t_partition* tarjan(AdjList* g) {
    // Initialiser la partition vide
    t_partition* partition = creer_partition_vide();

    // Initialiser les t_tarjan_vertex
    t_tarjan_vertex* vertices = (t_tarjan_vertex*)malloc((g->size + 1) * sizeof(t_tarjan_vertex));
    if (!vertices) { perror("malloc vertices"); exit(EXIT_FAILURE); }

    for (int i = 1; i <= g->size; i++) {
        vertices[i].identifiant = i;
        vertices[i].numero = -1;
        vertices[i].numero_accessible = -1;
        vertices[i].dans_pile = 0;
    }

    // Initialiser la pile vide
    t_pile* pile = creer_pile(g->size + 1);

    // Réinitialiser le numéro actuel
    numero_actuel = 0;

    // Lancer les parcours
    for (int i = 1; i <= g->size; i++) {
        if (vertices[i].numero == -1) {
            parcours(i, g, vertices, pile, partition);
        }
    }

    // Libérer les ressources temporaires
    liberer_pile(pile);
    free(vertices);

    return partition;
}

// ========== ANALYSE DES CARACTÉRISTIQUES ==========
void analyser_caracteristiques(t_partition* partition, t_ensemble_liens* liens) {
    // Déterminer quelles classes ont des liens sortants
    int* a_lien_sortant = (int*)calloc(partition->nb_classes, sizeof(int));
    if (!a_lien_sortant) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < liens->nb_liens; i++) {
        a_lien_sortant[liens->liens[i].classe_depart] = 1;
    }

    // Affichage des classes transitoires
    printf("\nClasses transitoires:\n");
    int nb_transitoires = 0;
    for (int i = 0; i < partition->nb_classes; i++) {
        if (a_lien_sortant[i]) {
            printf("  %s\n", partition->classes[i].nom);
            nb_transitoires++;
        }
    }
    if (nb_transitoires == 0) printf("  Aucune\n");

    // Affichage des classes persistantes
    printf("\nClasses persistantes:\n");
    int nb_persistantes = 0;
    for (int i = 0; i < partition->nb_classes; i++) {
        if (!a_lien_sortant[i]) {
            printf("  %s\n", partition->classes[i].nom);
            nb_persistantes++;
        }
    }
    if (nb_persistantes == 0) printf("  Aucune\n");

    // Affichage des états absorbants
    printf("\nEtats absorbants:\n");
    int nb_absorbants = 0;
    for (int i = 0; i < partition->nb_classes; i++) {
        if (!a_lien_sortant[i] && partition->classes[i].nb_sommets == 1) {
            printf("  %d\n", partition->classes[i].sommets[0].identifiant);
            nb_absorbants++;
        }
    }
    if (nb_absorbants == 0) printf("  Aucun\n");

    // Graphe irréductible
    printf("\nGraphe de Markov:\n");
    if (partition->nb_classes == 1) {
        printf("  Le graphe est irreductible\n");
    } else {
        printf("  Le graphe n'est pas irreductible\n");
    }

    free(a_lien_sortant);
}