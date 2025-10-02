#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <fstream>


using namespace std;


// =========== Domenova vrstva =================
class Field {
private:
    int n;
    int m;
    vector<string> fieldLines;
    vector<vector<int>> hints;

    //8 - direction as pairs (di, dj) -> NEobsahuje (0,0)
    static constexpr array<pair<int,int>,8> DIRECTIONS;

    // --- helpers for elemBoudries and mineElem---
    bool is_in_bounds(int row, int col) const noexcept;
    bool is_mine(int row, int col) const noexcept;

    void increment_neghbors(int i, int j);

public:
    Field(int n, int m, vector<string> fieldLines);
    void compute_hints();
    void print(ostream& os) const;

};

// --- definition of static member ---> clenska, jedna pro tridu
constexpr std::array<std::pair<int,int>, 8> Field::DIRECTIONS{{
    { -1, -1 }, { -1,  0 }, { -1,  1 },
    {  0, -1 },                     {  0,  1 },
    {  1, -1 }, {  1,  0 }, {  1,  1 }
}};

// --- definition of helpers ---
bool Field::is_in_bounds(int row, int col) const noexcept {
    return 0 <= row && row < n && 0 <= col && col < m;
}

bool Field::is_mine(int row, int col) const noexcept {
    return fieldLines[row][col] == '*';
}

// --- methods definition ---
// consturctor -> pres init list
Field::Field(int n_, int m_, vector<string> lines)
  : n(n_), m(m_),
    fieldLines(std::move(lines)),
    hints(n, vector<int>(m, 0)) {}


void Field::increment_neghbors(int i, int j) {
    for (const auto& [di, dj] : DIRECTIONS) { //pro me: jde primo strucred bindings
        int ni = i + di;
        int nj = j + dj;
        if (is_in_bounds(ni, nj) && !is_mine(ni, nj)) {
            hints[ni][nj]++;
        }

    }
}

void Field::compute_hints() {
    for (int i = 0; i < n; i++) {
        const string& line = fieldLines[i];
        for (int j = 0; j < m; j++) {
            if (line[j] == '*') {
                increment_neghbors(i, j);
            }
        }
    }
}

void Field::print(std::ostream& os) const {
    for (int i = 0; i < n; ++i) {
        const std::string& row = fieldLines[i];
        for (int j = 0; j < m; ++j) {
            os << (row[j] == '*' ? '*' : static_cast<char>('0' + hints[i][j]));  // pro me: char('0' + hints[i][j]) je funkcionalni styl castovani
        }
        os << '\n';
    }
}



// ================= Aplikacni vrstva ===================
struct InputBlock {
    int n;
    int m;
    vector<string> lines;

};

class MineSweeperApp {
private:
    bool read_block(istream& is, InputBlock& block);

public:
    void run(istream& is, ostream& os);

};

// ---- method definitions ---
bool MineSweeperApp::read_block(istream &is, InputBlock &block) {
    int n;
    int m;

    is >> n >> m;
    if (n == 0 && m == 0) return false;  //terminator

    block.n = n;
    block.m = m;
    block.lines.resize(n);


    for (int i = 0; i < n; i++) {
        is >> block.lines[i];   //radek je bez bilych znaku -> nacte se cely
    }
    return true;

}

void MineSweeperApp::run(istream& is, ostream& os) {
    //input...
    //compute...
    //print...

    int idx = 0;
    InputBlock block;
    while (read_block(is, block)) {

        Field field(block.n, block.m, move(block.lines)); //zkop a zahodim
        field.compute_hints();

        if (idx > 0) os << endl;
        os << "Field #" << (++idx) << ":" << endl;
        field.print(os);

    }

}



int main() {

    MineSweeperApp app;
    app.run(std::cin, std::cout);

    return 0;
}