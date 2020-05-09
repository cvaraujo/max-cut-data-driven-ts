//
// Created by carlos on 26/04/18.
//

#include "Includes.h"

#ifndef PUREGENETICALGORITHM_DATA_H
#define PUREGENETICALGORITHM_DATA_H

class Graph {
    vector<int> C, chg;
    int n, m;

public:
    vector<vector<pair<int, int>>> graph;

    explicit Graph(string path);

    int getEdge(int line, int column);

    double getC(int index);

    int getChg(int index);

    int getN() const;

};


#endif //PUREGENETICALGORITHM_DATA_H
