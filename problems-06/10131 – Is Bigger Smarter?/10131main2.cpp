#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Elephant {
    int weight;
    int iq;
    int id;  // 1-based
};

class ElephantsApp {
private:
    void read_input(istream& is, vector<Elephant>& elephants) {
        int w, iq;
        int line_id = 1;
        while (is >> w >> iq) {
            Elephant e;
            e.weight = w;
            e.iq = iq;
            e.id = line_id;
            elephants.push_back(e);
            ++line_id;
        }
    }

    void sort_elephants(vector<Elephant>& elephants) {
        // vaha up, pri stejne vaze iq dolu
        sort(elephants.begin(), elephants.end(),
             [](const Elephant& a, const Elephant& b) {
                 if (a.weight != b.weight) {
                     return a.weight < b.weight;
                 }
                 return a.iq > b.iq;
             });
    }

    vector<int> compute_longest_sequence(const vector<Elephant>& elephants) {
        int n = static_cast<int>(elephants.size());
        vector<int> dp(n, 1);      // nejdelsi posloupnost koncici slonem i
        vector<int> prev(n, -1);   // predhcuce

        int best_len = 0;
        int best_idx = 0;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (elephants[j].weight < elephants[i].weight &&
                    elephants[j].iq     > elephants[i].iq) {

                    if (dp[j] + 1 > dp[i]) {
                        dp[i] = dp[j] + 1;
                        prev[i] = j;
                    }
                }
            }

            if (dp[i] > best_len) {
                best_len = dp[i];
                best_idx = i;
            }
        }

        // reconstr. podle prev[]
        vector<int> seq;
        int idx = best_idx;
        while (idx != -1) {
            seq.push_back(elephants[idx].id);  // ulozit puvodni id radek
            idx = prev[idx];
        }
        // ted je to pozpatku -> reverse
        reverse(seq.begin(), seq.end());
        return seq;
    }

    void print_result(ostream& os, const vector<int>& sequence_ids) {
        os << sequence_ids.size() << "\n";
        for (int id : sequence_ids) {
            os << id << "\n";
        }
    }

public:
    void run(istream& is, ostream& os) {
        vector<Elephant> elephants;
        read_input(is, elephants);
        if (elephants.empty()) {
            return;
        }
        sort_elephants(elephants);
        vector<int> sequence_ids = compute_longest_sequence(elephants);
        print_result(os, sequence_ids);
    }
};

int main() {
    ElephantsApp app;
    app.run(cin, cout);
    return 0;
}
