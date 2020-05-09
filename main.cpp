#include <sstream>
#include <chrono>
#include <fstream>
#include "Graph.h"
#include "TabuSearch.h"
/*
void checkSolution(Graph *graph, TabuSearch *individual) {
    int i, j, objective = 0;
    for (i = 0; i < graph->getN(); i++) {
        for (auto e : graph->graph[i]) {
            j = e.first;
            if (individual->getGene(i) != individual->getGene(j)) {
                objective += e.second;
            }
        }
    }
    cout << "Objective made from solution: " << objective / 2 << endl;
}
*/

// ./mc g1.rud output toShuffle shuffleSize maxIter time
int main(int argc, const char *argv[]) {
    srand(time(NULL));
    srandom(time(NULL));
    srand48(time(NULL));

    int toShuffle, shuffleSize, maxIter, time;

    auto *graph = new Graph(argv[1]);

    stringstream convertShuffle(argv[3]), convertSizeShuffle(argv[4]), converMaxIter(argv[5]), convertTime(argv[6]);

    convertTime >> time;
    converMaxIter >> maxIter;
    convertShuffle >> toShuffle;
    convertSizeShuffle >> shuffleSize;

    auto *ts = new TabuSearch(graph, maxIter, toShuffle, shuffleSize, 8);
    double best = ts->solve(time);

    ofstream solution;
    solution.open(argv[2]);

    solution << best << endl;

    return 0;
}