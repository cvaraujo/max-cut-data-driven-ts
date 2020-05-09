//
// Created by carlos on 30/04/20.
//

#include "Solution.h"

Solution::Solution(Graph *graph) {
	this->weight = 0.0;
	this->graph = graph;

	int n = graph->getN(), j, i;
    chg = vector<int>(n);
    side = vector<bool>(n);
    gene = vector<bool>(n);
    cx = vector<int>(n);
    flipFreq = vector<int>(graph->getN());


    for (i = 0; i < graph->getN(); i++) {
        chg[i] = graph->getChg(i), gene[i] = false, side[i] = false, cx[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        if (drand48() <= 0.5) {
            gene[i] = true, side[i] = true;
            weight += chg[i], chg[i] = -chg[i];
            for (auto e : graph->graph[i]) {
                j = e.first;
                cx[j] += e.second;
                chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
            }
        }
    }
}

int Solution::getGene(int i) const {
    return gene[i];
}

void Solution::setGene(int i) {
    int j;
    flipFreq[i]++;
    gene[i] = !gene[i], side[i] = !side[i];
    weight += chg[i], chg[i] = -chg[i];
    for (auto e : graph->graph[i]) {
        j = e.first;
        if (gene[i]) cx[j] += e.second;
        else cx[j] -= e.second;
        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
    }
}

int Solution::getChg(int i) {
	return chg[i];
}