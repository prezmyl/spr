#include <algorithm>
#include <iostream>
#include <limits.h>
#include <map>
#include <vector>
#include <vector>


class Stick {
private:
    int length;
    std::vector<int> cuts;

    std::vector<int> pos;
    std::vector<std::vector<int>> memo;

public:
    Stick(int length, std::vector<int> cuts) : length(length), cuts(cuts) {
        pos.clear(); //pro kazdy podkus znova npalnit
        pos.push_back(0); //aktualni 0
        for (auto cut : cuts) {
            pos.push_back(cut);
        }
        pos.push_back(length); //aktualni cela delka kusu
        std::sort(pos.begin(), pos.end()); //protridim at neukladam stejne jen jinak

        //
        int m_size = static_cast<int>(pos.size());
        std::vector<int> m(m_size, -1);
        memo.assign(m_size,m);
    }

    int cost(int start, int end) {
        int &result = memo[start][end];
        // mam ulozeno
        if (result != -1) {
            return result;
        }

        //neni ulozeno, ale nejde dal rezat tento kus -> cena tohole 0 rezani je 0
        if (end == start + 1) {
            return result = 0;
        }

        result = INT_MAX;
        for (int cut = start + 1; cut < end; cut++) {
            // cena odvozena od delky prna + dalsi ceny podcasti rekurzivne
            int actual_cost = (pos[end] - pos[start])
                            + cost(start, cut)
                            + cost(cut, end);
            if (actual_cost < result) {
                result = actual_cost;
            }
        }
        return result;
    }

    int solve_cost() {
        return cost(0, static_cast<int>(pos.size()) - 1);
    }

};

class CuttingSticksApp {
public:
    void run(std::istream& is, std::ostream& os) {
        int L;
        while (is >> L && L != 0) {
            int N;
            is >> N;
            std::vector<int> cuts(N);
            for (int i = 0; i < N; ++i) is >> cuts[i];

            Stick stick(L, cuts);
            int result = stick.solve_cost();
            os << "The minimum cutting is " << result << "." << std::endl;
        }
    }
};


int main() {
    CuttingSticksApp app;
    app.run(std::cin, std::cout);
    return 0;
}