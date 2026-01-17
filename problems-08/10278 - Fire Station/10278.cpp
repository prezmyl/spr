// Solution for UVa 10278 – Fire Station
//
// Given a set of existing fire stations and a road network, choose a new
// intersection to place an additional station so that the maximum
// distance from any intersection to its nearest station is minimized.
//
// The problem statement describes up to 500 intersections connected by
// two‑way roads of various lengths. Each existing and potential station
// lies at one of these intersections【54515911631964†L9-L17】.  For each test case,
// we read the existing station positions, then build an adjacency list
// for the road network.  We perform a multi‑source Dijkstra’s algorithm
// starting from all existing stations to compute the shortest distance to
// the nearest current station.  Then, for every intersection 1..i, we run
// a single‑source Dijkstra from that intersection and combine the
// resulting distances with the previously computed distances.  We keep
// track of the location that yields the smallest maximum distance and
// output the lowest such intersection number【54515911631964†L24-L45】.

#include <bits/stdc++.h>
using namespace std;

// Utility function to run a multi‑source Dijkstra on an undirected,
// weighted graph.  It accepts a list of source nodes and computes
// shortest distances from any of these sources to every other vertex.
static vector<int>
multi_source_dijkstra(int n,
                      const vector<vector<pair<int,int>>> &adj,
                      const vector<int> &sources) {
    const int INF = numeric_limits<int>::max();
    vector<int> dist(n + 1, INF);
    // Min‑heap keyed by distance
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    for (int s : sources) {
        // initialize all given sources with distance zero
        if (s >= 1 && s <= n) {
            dist[s] = 0;
            pq.emplace(0, s);
        }
    }
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
            continue;
        for (const auto &edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t))
        return 0;
    while (t--) {
        int f, n;
        cin >> f >> n;
        // Read the existing fire stations
        vector<int> fire(f);
        for (int i = 0; i < f; ++i)
            cin >> fire[i];
        // Adjacency list for intersections 1..n
        vector<vector<pair<int,int>>> adj(n + 1);
        string line;
        getline(cin, line); // consume the remainder of the line after reading numbers
        // Read edges until a blank line or EOF is encountered.  Each line
        // describes a two‑way road with endpoints u, v and length w【54515911631964†L31-L36】.
        while (true) {
            if (!getline(cin, line))
                break;
            if (line.empty())
                break;
            stringstream ss(line);
            int u, v, w;
            if (!(ss >> u >> v >> w))
                break;
            // Build undirected edge
            if (u >= 1 && u <= n && v >= 1 && v <= n) {
                adj[u].emplace_back(v, w);
                adj[v].emplace_back(u, w);
            }
        }
        // Precompute distances from existing fire stations
        vector<int> distExisting = multi_source_dijkstra(n, adj, fire);
        // Initialize best values
        int bestStation = 1;
        int bestMaxDist = numeric_limits<int>::max();
        // For every intersection, tentatively place a new station there
        // and compute the maximum distance to the nearest station.  The
        // nearest station may be the candidate or an existing one.
        for (int s = 1; s <= n; ++s) {
            // Compute distances from candidate station
            vector<int> distFromS = multi_source_dijkstra(n, adj, {s});
            int currentMax = 0;
            // Combine distances: for each intersection, choose the smaller
            // of the distance to an existing station and to the candidate
            for (int v = 1; v <= n; ++v) {
                int d = min(distExisting[v], distFromS[v]);
                currentMax = max(currentMax, d);
                // Early exit if this candidate is already worse than the best
                if (currentMax > bestMaxDist)
                    break;
            }
            if (currentMax < bestMaxDist) {
                bestMaxDist = currentMax;
                bestStation = s;
            }
        }
        cout << bestStation << '\n';
        // Separate consecutive test cases with a blank line
        if (t)
            cout << '\n';
    }
    return 0;
}