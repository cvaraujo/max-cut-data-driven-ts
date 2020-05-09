//
// Created by carlos on 30/04/20.
//

#ifndef PUREGENETICALGORITHM_SOLUTION_H
#define PUREGENETICALGORITHM_SOLUTION_H

#include "Graph.h"

class Solution {
	Graph* graph;

public:
	vector<int> chg, cx, flipFreq;
    vector<bool> side, gene;
    double weight;

    Solution(Graph *graph);

    int getGene(int i) const;

    void setGene(int i);

    int getChg(int i);
};

#endif