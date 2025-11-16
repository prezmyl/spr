#include <iostream>
#include <sstream>
#include <vector>

class Writer{
private:
    std::ostream& out;
    int tabSize;
    int tab;

public:
    Writer(std::ostream& os, int tabSize = 2);
    void line(const std::string& s = "");
    void inward();
    void outward();

};

Writer::Writer(std::ostream &os, int tabSize): out(os), tabSize(tabSize), tab(0) {}

void Writer::line(const std::string &s) {
    for (int i = 0; i < tab; i++) {
        out << ' ';
    }
    out << s << '\n';
}

void Writer::inward() {
    tab += tabSize;
}

void Writer::outward() {
    tab -= tabSize;
    if (tab < 0) {
        tab = 0;
    }
}

struct PascalHeader {
    static std::vector<char> letters(int n) {
        std::vector<char> v; v.reserve(n);
        for (int i = 0; i < n; ++i) v.push_back(static_cast<char>('a' + i));
        return v;
    }
    static std::string joinVars(const std::vector<char>& v) {
        std::ostringstream oss;
        for (size_t i = 0; i < v.size(); ++i) {
            if (i) oss << ',';
            oss << v[i];
        }
        return oss.str();
    }
};



class MetaSort {
private:
    Writer& w;
    const std::vector<char>& all;


public:
    MetaSort(Writer& w, std::vector<char>& all);
    void generate_meta_sort();
    void insert_permutation(const std::vector<char>& sorted, char c, const std::vector<char>& tail);
    void core(const std::vector<char>& sorted, const std::vector<char>& rest);

};

MetaSort::MetaSort(Writer &w, std::vector<char> &all): w(w), all(all){}

void MetaSort::generate_meta_sort() {
    std::vector<char> sorted;
    std::vector<char> rest = all;
    core(sorted, rest);
}

// list
void MetaSort::core(const std::vector<char>& sorted, const std::vector<char>& rest) {
    // ukonceni rekurzivni vetve listem napr. (a,b,c,d) -> prazdne rest
    if (rest.empty()) {
        //vypis writln(sorted)  //pro danou vetev (list rekurz stromu)
        w.line("writeln(" + PascalHeader::joinVars(sorted) + ")");
        return;
    }

    //vyrob aktulni prvek(rest[0]) a aktualni tail(restp[1]..rest[n]) jako ve funkcionalnim programovani
    const char curr_char = rest.front();
    std::vector<char> tail(rest.begin() + 1, rest.end());

    // pro zavedeni prvniho prvku
    if (sorted.empty()) {
        std::vector<char> sorted_new(1,curr_char );
        core(sorted_new, tail);
        return;
    }

    // kdyz uz sorted neni prazdne -> vetveni v dalsi metode
    insert_permutation(sorted, curr_char, tail);

}



void MetaSort::insert_permutation(const std::vector<char> &sorted, char c, const std::vector<char> &tail) {
    //header? kde

    // if then
    {
        std::ostringstream os ;
        os << "if " << c << " < " << sorted.front() << " then";
        //inward
        w.line(os.str());
        w.inward();
        auto sorted2 = sorted;
        sorted2.insert(sorted2.begin(), c);
        core(sorted2, tail);
        // outward
        w.outward();
    }
    // else if then - vnitrni
    for (size_t i = 1; i < sorted.size(); i++) {
        std::ostringstream os;
        os << "else if " << c << " < " << sorted[i] << " then";
        w.line(os.str());
        w.inward();
        auto sorted2 = sorted;
        sorted2.insert(sorted2.begin() + static_cast<long>(i), c);
        core(sorted2, tail);
        // outward
        w.outward();
    }

    // else
    w.line("else");
    w.inward();
    auto sorted2 = sorted;
    sorted2.push_back(c);
    core(sorted2, tail);
    // outward
    w.outward();

}





class App {
public:
    void run(std::istream& is, std::ostream& os) {
        int T; if (!(is >> T)) return;
        for (int tc = 0; tc < T; ++tc) {
            int n; is >> n;
            if (tc) os << "\n";

            std::vector<char> vars = PascalHeader::letters(n);
            Writer w(os);


            w.line("program sort(input,output);");
            w.line("var");
            w.line(PascalHeader::joinVars(vars) + " : integer;");
            w.line("begin");
            w.line("");                            //
            w.line("readln(" + PascalHeader::joinVars(vars) + ");");


            MetaSort ms(w, vars);
            ms.generate_meta_sort();


            w.line("end.");
        }
    }
};



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    App app; app.run(std::cin, std::cout);
    return 0;
}
