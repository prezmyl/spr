#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <fstream>


using namespace std;



class Field {
private:
    int n;
    int m;
    vector<string> fieldLines;
    vector<vector<int>> hints;

    //8 - direction as pairs (di, dj) -> NEobsahuje (0,0)
    static array<pair<int,int>,8> DIRECTIONS;

    // --- helpers for elemBoudries and mineElem---
    bool is_in_bounds(int row, int col) const;
    bool is_mine(int row, int col) const;

    void increment_neghbors(int i, int j);

public:
    Field(int n, int m, vector<string> fieldLines);
    void compute_hints();
    void print(ostream& os) const;

};

// --- definition of static member ---> clenska, jedna pro tridu
array<std::pair<int,int>, 8> Field::DIRECTIONS{{
    { -1, -1 }, { -1,  0 }, { -1,  1 },
    {  0, -1 },              {  0,  1 },
    {  1, -1 }, {  1,  0 }, {  1,  1 }
}};

// --- definition of helpers ---
bool Field::is_in_bounds(int row, int col) const {
    return 0 <= row && row < n && 0 <= col && col < m;
}

bool Field::is_mine(int row, int col) const {
    return fieldLines[row][col] == '*';
}

// --- methods definition ---
// constr. -> pres init list
Field::Field(int n_, int m_, vector<string> lines)
  : n(n_), m(m_),
    fieldLines(std::move(lines)),
    hints(n, vector<int>(m, 0)) {}


void Field::increment_neghbors(int i, int j) {
    for (int k = 0; k < DIRECTIONS.size(); ++k) {
        int ni = i + DIRECTIONS[k].first;
        int nj = j + DIRECTIONS[k].second;
        if (is_in_bounds(ni, nj) && !is_mine(ni, nj)) {
            hints[ni][nj]++;
        }
    }
}

// problem resene pruchodem 2D pole a inkrementem sousodu pro kazdou minu
// casova slozitost je O(n*m*const), field se prochazi jednou a pro kazdou minu se pricte +1 vsem sousedum (max.8 = const loop v increment_negbors)
// nejvetsi naklady na rezii budou stejne spise v nacitani vstupu pro tuhle ulohu
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

        Field field(block.n, block.m, std::move(block.lines)); //zkop a zahodim
        field.compute_hints();

        if (idx > 0) os << '\n';
        os << "Field #" << (++idx) << ":\n";
        field.print(os);

    }

}



int main() {

    MineSweeperApp app;
    app.run(std::cin, std::cout);

    return 0;
}
