#include <iostream>
#include <vector>
#include <algorithm>

//podobne jako cuttinSticks

using namespace std;

struct Elephant {
    int weight;
    int iq;
    int inputIndex;
};


// hledani nejdelsi cesty v acyklickem grafu
class ElephantGraph {
private:
    vector<Elephant> elephants;
    vector<vector<int>> adj;
    vector<int> memo;
    vector<int> nextVertex; //drzi navaznost vrcholu v optimchain

    void sortElephants() {
        sort(elephants.begin(), elephants.end(),
             [](const Elephant& a, const Elephant& b) {
                 if (a.weight != b.weight) {
                     return a.weight < b.weight;
                 }
                 return a.iq > b.iq;
             });
    }

    void buildEdges() {
        int n = static_cast<int>(elephants.size());
        adj.assign(n, vector<int>());

        for (int i = 0; i < n; ++i) {
            const Elephant& ei = elephants[i];
            for (int j = i + 1; j < n; ++j) {
                const Elephant& ej = elephants[j];
                if (ei.weight < ej.weight && ei.iq > ej.iq) {
                    adj[i].push_back(j);
                }
            }
        }
    }
    // nejdelsi chains z u -> 1 + maxima pres vsechny vrcholu, kam vede hrana
    int dfs(int u) {
        int& res = memo[u]; // ukladam dfs , aby se pocitalo jen jednou
        if (res != -1) return res;

        res = 1;
        nextVertex[u] = -1;

        for (int v : adj[u]) {
            int candidate = 1 + dfs(v);
            if (candidate > res) {
                res = candidate;
                nextVertex[u] = v;
            }
        }
        return res;
    }

public:
    explicit ElephantGraph(vector<Elephant> elephants_)
        : elephants(std::move(elephants_)) {
        sortElephants();
        buildEdges();
    }

    // dfs pro vsechny u, nejlepsi vrchol jako zacatek, a pres nasledovniky posklada posloupnost
    vector<int> computeLongestChain() {
        int n = static_cast<int>(elephants.size());
        if (n == 0) return {};

        memo.assign(n, -1);
        nextVertex.assign(n, -1);

        int bestLen = 0;
        int bestStart = 0;

        for (int u = 0; u < n; ++u) {
            int len = dfs(u);
            if (len > bestLen) {
                bestLen = len;
                bestStart = u;
            }
        }

        vector<int> chain;
        int current = bestStart;
        while (current != -1) {
            chain.push_back(elephants[current].inputIndex);
            current = nextVertex[current];
        }
        return chain;
    }
};

class ElephantsApp {
private:
    void readInput(istream& is, vector<Elephant>& elephants) {
        int w, iq;
        int index = 1;
        while (is >> w >> iq) {
            Elephant e;
            e.weight = w;
            e.iq = iq;
            e.inputIndex = index;
            elephants.push_back(e);
            ++index;
        }
    }

    void printResult(ostream& os, const vector<int>& chain) {
        os << chain.size() << '\n';
        for (int id : chain) {
            os << id << '\n';
        }
    }

public:
    void run(istream& is, ostream& os) {
        vector<Elephant> elephants;
        readInput(is, elephants);
        if (elephants.empty()) {
            return;
        }

        ElephantGraph graph(std::move(elephants));
        vector<int> chain = graph.computeLongestChain();
        printResult(os, chain);
    }
};

int main() {
    ElephantsApp app;
    app.run(cin, cout);
    return 0;
}
