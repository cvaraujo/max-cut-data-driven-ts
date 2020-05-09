#include <fstream>
#include "Graph.h"

Graph::Graph(string path) {
    ifstream file;
    file.open(path, fstream::in);

    file >> n >> m;

    int v1, v2, w;

    graph = vector<vector<pair<int, int>>>(n);
    C = vector<int>(n);
    chg = vector<int>(n);

    for (int i = 0; i < m; i++) {
        file >> v1 >> v2 >> w;
        v1--, v2--;
        graph[v1].push_back(make_pair(v2, w)), graph[v2].push_back(make_pair(v1, w));
        chg[v1] += w, chg[v2] += w;
    }

    for (v1 = 0; v1 < n; v1++) {
        C[v1] = 0;
        for (auto p : graph[v1]) {
            C[v1] += p.second;
        }
    }

    file.close();
    cout << "Graph loaded!" << endl;
}

int Graph::getEdge(int line, int column) {
    for (auto p : graph[line])
        if (p.first == column)
            return p.second;
    return 0;
}

int Graph::getChg(int index) {
    return chg[index];
}

int Graph::getN() const {
    return n;
}

double Graph::getC(int index) {
    return C[index];
}
