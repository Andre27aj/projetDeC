#ifndef HASSE_H
#define HASSE_H

#include "structs.h"
#include "fonctions.h"

// Recensement des liens entre classes
t_ensemble_liens* recenser_liens(AdjList* g, t_partition* partition);

// Vérification de l'existence d'un lien
int lien_existe(t_ensemble_liens* ensemble, int depart, int arrivee);

// Ajout d'un lien
void ajouter_lien(t_ensemble_liens* ensemble, int depart, int arrivee);

// Export du diagramme de Hasse au format Mermaid
void export_hasse_mermaid(t_partition* partition, t_ensemble_liens* liens, const char* filename);

// Libération mémoire
void liberer_ensemble_liens(t_ensemble_liens* ensemble);
t_ensemble_liens* creer_ensemble_liens();

#endif