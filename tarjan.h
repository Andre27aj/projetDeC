#ifndef TARJAN_H
#define TARJAN_H

#include "structs.h"
#include "fonctions.h"

// ========== FONCTIONS DE PILE ==========
t_pile* creer_pile(int capacite);
int pile_est_vide(t_pile* pile);
void empiler(t_pile* pile, int valeur);
int depiler(t_pile* pile);
void liberer_pile(t_pile* pile);

// ========== FONCTIONS POUR CLASSES ==========
t_classe creer_classe_vide(const char* nom);
void ajouter_sommet_classe(t_classe* classe, t_tarjan_vertex sommet);
void afficher_classe(t_classe classe);
void liberer_classe(t_classe* classe);

// ========== FONCTIONS POUR PARTITION ==========
t_partition* creer_partition_vide();
void ajouter_classe_partition(t_partition* partition, t_classe classe);
void afficher_partition(t_partition* partition);
void liberer_partition(t_partition* partition);

// ========== ALGORITHME DE TARJAN ==========
t_partition* tarjan(AdjList* g);

// ========== ANALYSE DES CARACTÉRISTIQUES ==========
void analyser_caracteristiques(t_partition* partition, t_ensemble_liens* liens);

#endif