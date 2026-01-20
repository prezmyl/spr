// UVA 796 – Critical Links
//
// This program reads one or more datasets describing an undirected computer network.
// Each dataset begins with an integer `n` followed by `n` lines.  Line k (for
// 0 ≤ k < n) has the form:
//
//    server_index (number_of_connections) neighbor1 neighbor2 …
//
// Servers are identified by integers 0..n-1.  Connections are bi‑directional and
// may be repeated in the input.  The goal is to find all critical links
// (bridges) in the network: edges whose removal disconnects the graph.  After
// processing a dataset the program prints the number of critical links and
// lists them in ascending order, one per line, formatted as “u - v”.  A blank
// line separates consecutive datasets.  Input terminates with a line
// containing 0.

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

/// A simple undirected graph with adjacency lists.  Duplicate edges are ignored.
class Graph {
public:
    explicit Graph(int n = 0) : adj_(n) {}

    /// Resize the graph to hold n vertices.  All edges are cleared.
    void resize(int n) {
        adj_.assign(n, std::vector<int>());
    }

    /// Add an undirected edge between u and v, ignoring duplicates and self‑loops.
    void addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= static_cast<int>(adj_.size()) ||
            v >= static_cast<int>(adj_.size()) || u == v) {
            return;
        }
        // Check if v is already a neighbor of u
        auto &au = adj_[u];
        if (std::find(au.begin(), au.end(), v) == au.end()) {
            au.push_back(v);
        }
        // Check if u is already a neighbor of v
        auto &av = adj_[v];
        if (std::find(av.begin(), av.end(), u) == av.end()) {
            av.push_back(u);
        }
    }

    /// Return number of vertices.
    int size() const {
        return static_cast<int>(adj_.size());
    }

    /// Return adjacency list of vertex u.
    const std::vector<int> & neighbors(int u) const {
        return adj_[u];
    }

private:
    std::vector<std::vector<int>> adj_;
};

/// Finds all bridges (critical links) in an undirected graph using Tarjan’s
/// algorithm.  Bridges are returned as pairs (u, v) with u < v.
class BridgeFinder {
public:
    explicit BridgeFinder(const Graph &g)
        : g_(g), time_(0), disc_(g.size(), -1), low_(g.size(), -1),
          visited_(g.size(), false), parent_(g.size(), -1) {}

    /// Compute and return all bridges in the graph.  Each bridge is a pair
    /// (u, v) with u < v, sorted lexicographically.
    std::vector<std::pair<int, int>> findBridges() {
        int n = g_.size();
        for (int u = 0; u < n; ++u) {
            if (!visited_[u]) {
                dfs(u);
            }
        }
        // Sort the bridges in ascending order by (u, v)
        std::sort(bridges_.begin(), bridges_.end());
        return bridges_;
    }

private:
    void dfs(int u) {
        visited_[u] = true;
        disc_[u] = low_[u] = time_++;
        for (int v : g_.neighbors(u)) {
            if (!visited_[v]) {
                parent_[v] = u;
                dfs(v);
                // After visiting v, update low[u]
                low_[u] = std::min(low_[u], low_[v]);
                // If the lowest reachable discovery time from v is greater
                // than the discovery time of u, (u, v) is a bridge
                if (low_[v] > disc_[u]) {
                    int a = std::min(u, v);
                    int b = std::max(u, v);
                    bridges_.emplace_back(a, b);
                }
            } else if (v != parent_[u]) {
                // Back edge
                low_[u] = std::min(low_[u], disc_[v]);
            }
        }
    }

    const Graph &g_;
    int time_;
    std::vector<int> disc_;
    std::vector<int> low_;
    std::vector<bool> visited_;
    std::vector<int> parent_;
    std::vector<std::pair<int, int>> bridges_;
};

/// Application class orchestrating I/O for problem 796.
class CriticalLinksApp {
public:
    void run(std::istream &in, std::ostream &out) {
        bool first = true;
        while (true) {
            int n;
            if (!(in >> n)) {
                break;
            }
            if (n == 0) {
                break;
            }
            Graph graph;
            graph.resize(n);
            // Consume the trailing newline after n
            std::string line;
            std::getline(in, line);
            // Read n lines describing the connections
            for (int i = 0; i < n; ++i) {
                std::getline(in, line);
                if (line.empty()) {
                    --i;
                    continue;
                }
                // Parse line: id (k) v1 v2 ... vk
                std::stringstream ss(line);
                int id;
                ss >> id;
                char ch;
                int k;
                // parse "(k)"
                ss >> ch; // '('
                ss >> k;
                ss >> ch; // ')'
                for (int c = 0; c < k; ++c) {
                    int v;
                    ss >> v;
                    graph.addEdge(id, v);
                }
            }
            // Find bridges
            BridgeFinder finder(graph);
            auto bridges = finder.findBridges();
            // Print blank line between cases
            if (!first) {
                out << "\n";
            }
            first = false;
            // Output results
            out << bridges.size() << " critical links\n";
            for (auto &p : bridges) {
                out << p.first << " - " << p.second << "\n";
            }
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    CriticalLinksApp app;
    app.run(std::cin, std::cout);
    return 0;
}