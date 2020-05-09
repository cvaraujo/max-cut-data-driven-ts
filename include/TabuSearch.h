//
// Created by carlos on 26/04/18.
//

#ifndef PUREGENETICALGORITHM_GENE_H
#define PUREGENETICALGORITHM_GENE_H

#include "Graph.h"
#include "Solution.h"
#include<chrono>

class TabuSearch {
    vector<int> CL, RCL, TL, eliteFreq;
    vector<int> p = vector<int>(15);
    vector<int> a = vector<int>(15);
    vector<int> interval = vector<int>(15);

    vector<Solution *> eliteSol;
    Solution * incumbent;
    Solution * currentSol;
    int iter, maxIter, shuffleSize, toShuffle, ternure, R;
    Graph *graph;
    bool bestImprov;

public:
    TabuSearch(Graph *graph, int maxIter, int toShuffle, int shuffleSize, int R);

    double solve(int time);

    void fillingEliteList(int time);

    void TS(Solution *solution);

    void updateEliteFreq(Solution *remove, Solution *add);

    void makeCL();

    void makeTL(int tenure);

    void updateTL(int i, int &last, int &indexLast, vector<int> &a, vector<int> &interval);

    void updateCL(Solution *solution);

    void memoryBasedPerturbation(Solution *solution);

    void neighborhoodMove(Solution *solution);
};


#endif //PUREGENETICALGORITHM_GENE_H
