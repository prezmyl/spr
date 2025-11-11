//
// Created by xpolas on 11/11/25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include <queue>
#include <vector>




class Graph {
private:
    std::vector<std::vector<int>> adj;
    std::map<int, bool> visited;
    int v;
    int e;

public:
    Graph(int v, int e);
    void addEdge(int u, int v);
    void BFS(int s);

};

#endif //GRAPH_H
