#ifndef STRUCTS_H
#define STRUCTS_H

// Structure pour un sommet dans l'algorithme de Tarjan
typedef struct {
    int identifiant;
    int numero;
    int numero_accessible;
    int dans_pile;
} t_tarjan_vertex;

// Structure pour une pile
typedef struct {
    int* data;
    int top;
    int capacite;
} t_pile;

// Structure pour une classe (composante fortement connexe)
typedef struct {
    char nom[10];
    t_tarjan_vertex* sommets;
    int nb_sommets;
    int capacite;
} t_classe;

// Structure pour une partition (ensemble de classes)
typedef struct {
    t_classe* classes;
    int nb_classes;
    int capacite;
} t_partition;

// Structure pour un lien entre deux classes
typedef struct {
    int classe_depart;
    int classe_arrivee;
} t_lien;

// Structure pour un ensemble de liens
typedef struct {
    t_lien* liens;
    int nb_liens;
    int capacite;
} t_ensemble_liens;

#endif