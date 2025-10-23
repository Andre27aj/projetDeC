#include "markov.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
    ListeAdj g = readGraph("exemple1.txt");
    afficherListeAdj(&g);
    estGrapheMarkov(&g);
    genererMermaid(&g, "graphe_mermaid.txt");
    return 0;
}
