//
// Created by xpolas on 11/11/25.
//

#include "Graph.h"

#include <iostream>

Graph::Graph(int v, int e) : v(v), e(e) {}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void Graph::BFS(int s) {
    if (visited[s] == false) {
        visited[s] = true;
    }

    std::queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        std::cout << u << std::endl;
        //check condition
        q.pop();
        for (auto v : adj[u]) {
            if (visited[v] == false) {
                q.push(v);
            }
        }
    }
}
