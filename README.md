# Projet TI301 – Graphes de Markov (Parties 1, 2 et 3)
HADJ ISA Adam | ALBUQUERQUE André | DELPORTE Roman | HEBRAUD Clément

Ce projet implémente en C l’ensemble des outils nécessaires pour analyser un graphe de Markov :
lecture du graphe, vérification, extraction des classes, diagramme de Hasse, calculs matriciels et distributions stationnaires.

---

## 🚀 Fonctionnalités

### Partie 1 — Graphe de Markov
- Lecture d’un fichier texte (liste d’adjacence).
- Vérification des probabilités sortantes (somme ≈ 1).
- Génération d’un fichier Mermaid permettant de dessiner le graphe.

### Partie 2 — Tarjan & Hasse
- Détection des composantes fortement connexes (algorithme de Tarjan).
- Construction du diagramme de Hasse (liens entre classes).
- Identification : classes transitoires, persistantes, états absorbants, irréductibilité.

### Partie 3 — Matrices & distributions
- Construction de la matrice de transition M.
- Multiplications matricielles, diff(M,N), puissance Mⁿ.
- Extraction des sous-matrices par classe.
- Calcul des distributions stationnaires.

---