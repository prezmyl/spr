#include <algorithm>
#include <iostream>
#include <vector>

/*
 * pohyb jen jednim smerem -> acyklicky graf -> vhodne pro DP
 * zvolim bottom-up od konce(zprava doleva) cost je path to go (NE path so far)
 * pro c = n-1 dp[r][n-1] = a[r][n-1] -> base case
 * c = n-2, n-3,...,0 -> prvni sloupec
 * prechod: dp[r][c] = a[r][c] + min(dp[up][c+1], dp[mid][c+1], dp[down][c+1])
 */

// model vztupnich dat
class Matrix {
private:
    std::vector<std::vector<int>> a;
    int m, n;

public:

    static bool read(std::istream& in, Matrix& out) {
        if (!(in >> out.m >> out.n)) { return false; }

        out.a.assign(out.m, std::vector<int>(out.n));
        for (int r = 0; r < out.m; ++r) {
            for (int c = 0; c < out.n; ++c) {
                in >> out.a[r][c];
            }
        }
        return true;
    }

    int get_m() const { return m; }
    int get_n() const { return n; }
    int at(int r, int c) const { return a[r][c]; }


};

//dp data
struct Result {
    std::vector<int> pathRows;
    long long cost = 0;
};

//algo
class Solver {
private:
    static int wrap_around(int r, int m) { return (r % m + m) % m; }
    static std::vector<int> sorted_candidates(int r, int m) {
        int up = wrap_around(r - 1, m);
        int mid = r;
        int down = wrap_around(r + 1, m);

        std::vector<int> cand = {up, mid, down};
        std::sort(cand.begin(), cand.end());  //pro lexico tie-briek
        return cand;
    }

public:
    Result solve(const Matrix& mat) const {
        const int m = mat.get_m();
        const int n = mat.get_n();

        //min cena cesty z (r,c) do posledniho sloupce vcetne)
        std::vector<std::vector<long long>> dp(m, std::vector<long long>(n, 0));
        std::vector<std::vector<int>> nextRow(m, std::vector<int>(n, -1));

        //base-case
        for (int r = 0; r < m; r++) {
            dp[r][n -1] = mat.at(r, n - 1);
        }

        // bottom-up (zprava doleva)
        for (int c = n - 2; c >= 0; c--) {
            for (int r = 0; r < m; r++) {
                std::vector<int> cand = sorted_candidates(r,m);

                int best_idx = cand.at(0);
                long long bestCost_val = dp.at(best_idx)[(c + 1)];

                for (int i = 1; i < cand.size(); i++) {
                    int idx = cand.at(i);
                    long long idxCost_val = dp.at(idx)[(c + 1)];
                    if (idxCost_val < bestCost_val) {
                        best_idx = idx;
                        bestCost_val = idxCost_val;
                    }
                    //pri shode vyhrava mensi radek diky sortu
                }


                dp.at(r)[c] = (long long)mat.at(r, c) + bestCost_val;
                nextRow.at(r)[c] = best_idx;

            }
        }

        //start min dp[r][0], pri shode vah cesty v prvnim sloupci(co je jsou uz vahy minimalnich celych cest z prvniho do posledniho sloupce ) -> mensi r
        int start = 0;
        for (int r = 1; r < m; r++) {
            if (dp.at(r)[0] < dp.at(start)[0]) { start = r;}
        }

        //rekonstrukce cesty
        Result res;
        res.cost = dp.at(start)[0];
        res.pathRows.resize(n);

        int r = start;
        res.pathRows.at(0) = r;

        for (int c = 0; c < n - 1; c++) {
            r = nextRow.at(r)[c];
            res.pathRows.at(c + 1) = r;
        }
        return res;
    }

};


//orchestrace i/o
class App {
    private:
    static void printResult(std::ostream& out, const Result& res) {
        // cesta: 1-based
        for (size_t i = 0; i < res.pathRows.size(); ++i) {
            if (i) out << ' ';
            out << (res.pathRows[i] + 1);
        }
        out << "\n" << res.cost << "\n";
    }
    public:
        void run(std::istream& in, std::ostream& out) {
            Matrix mat;
            Solver solver;
            while (Matrix::read(in, mat)) {
                Result res = solver.solve(mat);
                printResult(out, res);
            }
        }

};

int main() {
    App app;
    app.run(std::cin, std::cout);
    return 0;
}