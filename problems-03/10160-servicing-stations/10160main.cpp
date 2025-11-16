#include <iostream>
#include <bits/stdc++.h>

using namespace std;

using U64 = uint64_t;
//PLAN:
// UP ->minmalni dominantni mnozina  MDS -> Upper Bound
// -> pak projdu moznosti vyberu z ni stromem rozhodnuti -> DFS
//      -> implementovanem DFS pruchodem pres vhodne vybrane sousedy N[u]
//          -> muzu najit lepsi UP
// mozne optimalizace -> LB a redukce(list a dominance)
// LB pripadne -> greedy packing disjunktnich uzaveru (aktualni_pocet + LB >= UP -> tudy ne)
//          -> kolik stanic jeste musim pridat vzhledem k tomu co jed doposud nepokryto
// Bitove pole pro reprezentaci MDS -> O(1) pro pristup


int lsb_index(U64 x) {
    return __builtin_ctzll(x);
}


//MDS -> bud je v ni sam vrchol nebo minimalne jeho soused
int greedy_upper_bound_MDS(const vector<U64>& mask, U64 allMask) {
    U64 covered = 0;
    int picks = 0;
    vector<char> used(mask.size(), 0);

    while (covered != allMask) {
        int best = -1;
        int bestGain = -1;
        for (int v = 0; v < (int)mask.size(); ++v) {
            if (used[v]) continue;
            U64 newly = mask[v] & ~covered;
            int gain = __builtin_popcountll(newly);
            if (gain > bestGain) { bestGain = gain; best = v; }
        }
        if (best == -1) break;
        used[best] = 1;
        covered |= mask[best];
        ++picks;
    }
    return picks;
}



//Rozhodovaci strom DFS
U64 gAllMask = 0;               // bity 0..n-1 = 1
vector<U64> gMask;              // N[v] jako bitgMasky
int gBest = INT_MAX;            //start: greedy_upper_bound(mask, gAllMask)
void decesion_tree_DFS(U64 covered, int picks) {
    /*
     *startovaci vrchol
     *najeho sousedy zavolam DFS
     * -> recursive function call stack bude simulovat zasobnik
     */

    //prune podle aktualniho UB -> ukonci aktulni rekurzi
    if (picks >= gBest) return;
    //konec vseho -> optimum nalezeno
    if (covered == gAllMask) { gBest = picks; return; }

    U64 left = gAllMask & ~covered;
    int u = lsb_index(left);

    //kandidati co maji nejaky smysl -> gain > 0
    vector<int> candidates;
    for (U64 b= gMask[u]; b; b &= b-1) {
        int v = __builtin_ctzll(b);
        U64 newly = gMask[v] & ~covered;
        if (newly) candidates.push_back(v);
    }
    // seradit kandidaty podle zisku
    sort(candidates.begin(), candidates.end(), [&](int a, int b){
        int ga = __builtin_popcountll(gMask[a] & ~covered);
        int gb = __builtin_popcountll(gMask[b] & ~covered);
        return ga > gb;
    });

    for (auto v : candidates) {
        U64 newCovered = covered | gMask[v];
        decesion_tree_DFS(newCovered, picks + 1);
    }


}



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && (n||m)) {
        vector<vector<int>> adj(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            --u; --v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        //bitmask pro sousedy N[v]
        vector<U64> mask(n, 0);
        for (int v = 0; v < n; ++v) {
            U64 mv = (1ULL << v);            // v sám sebe
            for (int w : adj[v]) mv |= (1ULL << w);
            mask[v] = mv;
        }
        //cela maska: bity: 0,1,..n-1 = 1
        uint64_t allMask = (1ULL << n) - 1ULL;

        gMask = mask;
        gAllMask = allMask;

        gBest = greedy_upper_bound_MDS(mask, gAllMask);
        decesion_tree_DFS(0ULL, 0);

        cout << gBest << '\n';
    }

    return 0;
}
