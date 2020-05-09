//
// Created by carlos on 26/04/18.
//

#include "TabuSearch.h"

TabuSearch::TabuSearch(Graph *graph, int maxIter, int toShuffle, int shuffleSize, int R) {
    if (graph != nullptr) {
    	this->graph = graph;
        this->R = R;

        this->shuffleSize = shuffleSize;
        this->toShuffle = toShuffle;
        this->maxIter = maxIter;

        this->eliteFreq = vector<int>(graph->getN());
        this->eliteSol = vector<Solution *>();

        p = {1, 2, 1, 4, 1, 2, 1, 8, 1, 2, 1, 4, 1, 2, 1};
	    interval[0] = 800;

	    int last = 100, indexLast = 0;
	    for (int i = 0; i < 15; i++) {
	        a[i] = (0.125 * p[i] * 200);
	        if (i != 14) interval[i + 1] = interval[i] + 4 * 1000 * 0.125 * p[i];
	    }
	}
}

double TabuSearch::solve(int time) {
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();

	fillingEliteList(time);

	int worstIndex, i, j;
	double worst;
	bool add;

	// cout << "solve" << endl; 
	while(chrono::duration_cast<chrono::seconds>(end - start).count() < time) {
		j = int(random() % eliteSol.size());
		// cout << "j: " << j << endl;

		currentSol = new Solution(*eliteSol[j]);
		incumbent = new Solution(*currentSol);

		memoryBasedPerturbation(currentSol);
		TS(currentSol);

		worst = eliteSol[0]->weight;
		worstIndex = 0;
		add = true;
		for (i = 1; i < eliteSol.size(); i++) {
			if (eliteSol[i]->weight == incumbent->weight) {
				add = false;
				break;
			} else if(worst > eliteSol[i]->weight) {
				worst = eliteSol[i]->weight;
				worstIndex = i;
			}
		}

		if(add) {
			updateEliteFreq(eliteSol[worstIndex], incumbent);
			auto it = eliteSol.begin() + worstIndex;
			eliteSol.erase(it);
			eliteSol.push_back(incumbent);
		}

		// cout << "Inc: " << incumbent->weight << endl;
		// for (i = 0; i < eliteSol.size(); i++) {
		// 	cout << eliteSol[i]->weight << ", ";
		// }	
		// cout << endl;
		// getchar();
		bestImprov = !bestImprov;
		end = chrono::steady_clock::now();
	}

	double best = eliteSol[0]->weight;
	for (i = 1; i < eliteSol.size(); i++) {
		if (best < eliteSol[i]->weight) 
			best = eliteSol[i]->weight;
	}

	return best;
}

void TabuSearch::fillingEliteList(int time) {
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();
	this->currentSol = new Solution(graph);
	this->incumbent = new Solution(*currentSol);
	int i = 0, j;

	while(i < R && chrono::duration_cast<chrono::seconds>(end - start).count() < time/3) {
		// cout << "Before: " << currentSol->weight << ", ";
		TS(currentSol);
		// cout << "After: " << currentSol->weight << endl;

		if (eliteSol.empty() || incumbent->weight != eliteSol[int(eliteSol.size())-1]->weight) {
			this->eliteSol.push_back(incumbent);
			updateEliteFreq(nullptr, incumbent);
			i++;
		}

		j = int(random() % eliteSol.size());
		
		this->currentSol = new Solution(*eliteSol[j]);
		this->incumbent = new Solution(*currentSol);

		memoryBasedPerturbation(currentSol);

		bestImprov = !bestImprov;
		end = chrono::steady_clock::now();
		// getchar();
	}
	
}

void TabuSearch::TS(Solution *solution) {
	int n = graph->getN();

    int count = 0;
	makeTL(a[0]);
	int last = 500, indexLast = 0;

	for (int i = 0; i < 30*n; i++) {
    	updateTL(i, last, indexLast, a, interval);
    	neighborhoodMove(solution);

        if (incumbent->weight < solution->weight) { 
            incumbent = new Solution(*solution);
            // cout << incumbent->weight << endl;
        }
	}
}

void TabuSearch::updateEliteFreq(Solution *remove, Solution *add) {
	if (remove != nullptr) {
		for (int i = 0; i < graph->getN(); i++) {
			if (remove->gene[i]) eliteFreq[i] -=1;
			if (add->gene[i]) eliteFreq[i] +=1;	
		}
	} else {
		for (int i = 0; i < graph->getN(); i++) 
			if (add->gene[i]) eliteFreq[i] +=1;	
	}
}

void TabuSearch::makeCL() {
    CL = vector<int>();
    for (int i = 0; i < graph->getN(); i++) 
    	CL.push_back(i);
}

void TabuSearch::makeTL(int tenure) {
    TL = vector<int>();
    for (int i = 0; i < tenure; i++) 
    	TL.push_back(-1);
}

void TabuSearch::updateTL(int i, int &last, int &indexLast, vector<int> &a, vector<int> &interval) {
    if (indexLast < 14) {
        if (i == last - 1) {
            last += interval[++indexLast];
            int newTLSize = a[indexLast];
            if (TL.size() < newTLSize)
                for (int j = TL.size(); j < newTLSize; j++) 
                	TL.push_back(-1);
            else 
            	TL.erase(TL.begin() + newTLSize, TL.end());
        }
    }

}

void TabuSearch::updateCL(Solution *solution) {
    CL = vector<int>();
    for (int i = 0; i < graph->getN(); i++)
        if ((solution->cx[i] < (graph->getC(i) / 2) && solution->gene[i] == 0) ||
            (solution->cx[i] > (graph->getC(i) / 2) && solution->gene[i] == 1) || 
            solution->chg[i] >= 0) {
            CL.push_back(i);
        }

}

bool sortRev(const pair<int, double> &a, const pair<int, double> &b) {
	return (a.second < b.second);
}

void TabuSearch::memoryBasedPerturbation(Solution *solution) {
	int n = graph->getN(), i, j;

	int maxFreq = max_element(solution->flipFreq.begin(), solution->flipFreq.end()).operator*();

	double scoreValue;
	vector<pair<int, double>> score = vector<pair<int, double>>();

	for (i = 0; i < n; i++) {
		scoreValue = (eliteFreq[i] * (int(eliteSol.size()) - eliteFreq[i])) / pow(int(eliteSol.size()), 2);
		scoreValue += 0.3 * (1 - (solution->flipFreq[i] / maxFreq)); 
		score.push_back(make_pair(i, scoreValue));//solution->flipFreq[i]));
	}

	sort(score.begin(), score.end(), sortRev);

	// cout << score[0].second << endl;
	// cout << score[score.size()-1].second << endl;
	// getchar();
	// Lambda = 1.2, strength = n/4

	// double p = 0, deno = 0;

	// for (i = 1; i <= n; i++) {
	// 	deno += pow(i, -1.2);
	// }
	// cout << "Deno: " << deno << endl;

	for (j = 0; j < n/5; j++) {
		// p = pow(j, -1.2) / deno;
		// cout << "P: " << p << endl;
		if (drand48() <= 0.6)
			solution->setGene(score[j].first);//(int(random() % n));
	}

	// getchar();
}


void TabuSearch::neighborhoodMove(Solution *solution) {
    double maxDeltaCost = numeric_limits<int>::min();
    int bestCand = -1, deltaCost;
    vector<int>::iterator it;
    updateCL(solution);

    random_shuffle(CL.begin(), CL.end());

    for (auto i : CL) {
        deltaCost = solution->getChg(i);
        it = find(TL.begin(), TL.end(), i);
        if (it != TL.end() || deltaCost >= 0) {
        	if (!bestImprov) {
	            bestCand = i;
	            break;
        	} else if (maxDeltaCost < deltaCost) {
                bestCand = i;
                maxDeltaCost = deltaCost;
            }
        }
    }

    TL.erase(TL.begin());
    if (bestCand != -1) {
        TL.push_back(bestCand);
        it = find(CL.begin(), CL.end(), bestCand);
        CL.erase(it);
        solution->setGene(bestCand);
    } else {
        TL.push_back(-1);
    }
}



/*
double TabuSearch::solve(int time) {
	int n = graph->getN();
	vector<int> p = vector<int>(15);
	vector<int> a = vector<int>(15);
	vector<int> interval = vector<int>(15);

	p = {1, 2, 1, 4, 1, 2, 1, 8, 1, 2, 1, 4, 1, 2, 1};
	interval[0] = 100;

	int last = 100, indexLast = 0;
	for (int i = 0; i < 15; i++) {
	    a[i] = (0.125 * p[i] * 200);
	    if (i != 14) interval[i + 1] = interval[i] + 4 * 200 * 0.125 * p[i];
	}

	cout << "Initial Solution = " << incumbent->weight << endl;

    int count = 0;
	makeTL(a[0]);

	for (int i = 0; i < maxIter; i++) {
    	updateTL(i, last, indexLast, a, interval);
    	neighborhoodMove();

        if (incumbent->weight < currentSol->weight) {
            incumbent = new Solution(*currentSol);
            // count = 0;
        } 
        // else count++;
        
        // if (currentSol->weight > eliteSol[0]->weight && currentSol->weight != 
        // 		eliteSol[int(eliteSol.size()-1)]->weight) {
        // 	if (eliteSol.size() >= 8) {
        // 		reduceEliteFreq(eliteSol[0]);
        //     	eliteSol.erase(eliteSol.begin());
        // 	}
        //     eliteSol.push_back(new Solution(*currentSol));
        //     increaseEliteFreq(currentSol);
        // }

		cout << i << "st iteration = " << incumbent->weight << endl;
		// getchar();
	}
	return incumbent->weight;
}
*/